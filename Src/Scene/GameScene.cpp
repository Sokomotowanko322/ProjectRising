#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/ColliderManager.h"
#include "../Manager/SceneManager.h"
#include "../Common/Fader.h"
#include "../Object/SkyDome.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/NormalEnemy.h"
#include "../Object/Weapon.h"
#include "GameScene.h"

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

	// コライダ初期化
	InitCollider();

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	// カメラの初期化
	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::InitCollider(void)
{
	colMng_ = std::make_unique<ColliderManager>();

	ColliderData data;
	data.shape = ColliderShape::Capsule;
	data.capsule.playerPos = player_->GetPos();
	data.capsule.height = 80.0f;
	data.capsule.radius = 20.0f;

	colMng_->RegisterInstance(ObjectType::Player, VGet(10.0f, 0.0f, 0.0f));
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

	// GameScene::Update()
	ColliderData data;
	data.shape = ColliderShape::Capsule;
	data.capsule.playerPos = player_->GetPos();
	data.capsule.height = 80.0f;
	data.capsule.radius = 20.0f;

	colMng_->Update(ObjectType::Player, 0, data); // プレイヤー1体だけならindex=0で固定
}

void GameScene::Draw(void)
{
	// skydomeの描画
	skyDome_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 敵描画
	normalEnemy_->Draw();

	// テスト用で左上に緑の四角を描画
	DrawBox(0, 0, 500, 50, GetColor(0, 255, 0), TRUE);
}
