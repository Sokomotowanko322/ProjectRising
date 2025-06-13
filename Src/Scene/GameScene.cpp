#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/ColliderManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "../Common/Fader.h"
#include "../Object/SkyDome.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/NormalEnemy.h"
#include "../Object/Weapon.h"
#include "GameScene.h"

const VECTOR PLAYER_WEAPON_TOP = { 0.0f, 180.0f, 0.0f };
const VECTOR PLAYER_WEAPON_BOTTOM = { 0.0f, 0.0f, 0.0f };

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// プレイヤーの初期化
	player_ = std::make_shared<Player>();
	player_->Init();

	// 敵の初期化
	normalEnemy_ = std::make_shared<NormalEnemy>(player_);
	normalEnemy_->Init();

	// コライダ生成
	colMng_ = std::make_unique<ColliderManager>();
	colMng_->RegisterActor(player_);
	colMng_->RegisterActor(normalEnemy_);
	colMng_->RegisterActor(player_->GetWeapon());

	// --- ここで一度WeaponのmodelIdを確認 ---
	printfDx("Weapon modelId after Init: %d\n", player_->GetWeapon()->GetTransform().modelId);

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	// コライダのセット
	AddColliders();

	// カメラの初期化
	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::AddColliders(void)
{
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		player_->GetPos(),
		{ 0.0f,5.0f,0.0f },
		20.0f,
		5.0f,
		player_->GetTransform().modelId,
		false	// 貫通しない
	));
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		player_->GetRightHandPos(),
		{ 0.5f,0.5f,0.5f } ,
		1.0f,
		10.0f,
		player_->GetTransform().modelId,
		true,	// 貫通する
		true	// 右腕コライダか判定
	));
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		normalEnemy_->GetPos(),
		{0.0f,5.0f,0.0f},
		20.0f,
		5.0f,
		normalEnemy_->GetTransform().modelId,
		false	// 貫通しない
	));


	// 武器のTransform取得
	Quaternion rot = player_->GetWeapon()->GetWeaponTransform().quaRot;
	VECTOR scl = player_->GetWeapon()->GetWeaponTransform().scl;
	VECTOR pos = player_->GetWeapon()->GetWeaponTransform().pos;

	// ローカル→ワールド変換
	VECTOR tipWorld = VAdd(pos, Quaternion::PosAxis(rot, { PLAYER_WEAPON_TOP.x * scl.x, PLAYER_WEAPON_TOP.y * scl.y, PLAYER_WEAPON_TOP.z * scl.z }));
	tipWorld.y += 100.0f; // 武器の先端を上に100単位移動
	VECTOR baseWorld = VAdd(pos, Quaternion::PosAxis(rot, { PLAYER_WEAPON_BOTTOM.x * scl.x, PLAYER_WEAPON_BOTTOM.y * scl.y, PLAYER_WEAPON_BOTTOM.z * scl.z }));

	// カプセル中心・向き・長さ
	VECTOR center = VScale(VAdd(tipWorld, baseWorld), 0.5f);
	VECTOR dir = VNorm(VSub(tipWorld, baseWorld));
	float height = VSize(VSub(tipWorld, baseWorld));

	// コライダ追加
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		center,
		dir,
		height,
		2.0f, // 半径は適宜
		player_->GetWeapon()->GetWeaponTransform().modelId,
		true // トリガー
	));
}

void GameScene::Update(void)
{
	// コントロールタイプの設定
	auto& insI = InputManager::GetInstance();
	auto pad = insI.GetJPadState(JOYPAD_NO::PAD1);
	insI.SetControlType(InputManager::CONTROL_TYPE::KEY);

	// 背景追従
	skyDome_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	normalEnemy_->Update();

	colMng_->Update();
	
}

void GameScene::Draw(void)
{
	// skydomeの描画
	skyDome_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 敵描画
	normalEnemy_->Draw();

	// コライダーの描画
	colMng_->DrawColliders();

	// テスト用で左上に緑の四角を描画
	DrawBox(0, 0, 500, 50, GetColor(0, 255, 0), TRUE);
}
