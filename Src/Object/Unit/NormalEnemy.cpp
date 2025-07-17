#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Controller/AnimationController.h"
#include "../../Utility/Utility.h"
#include "../../Object/Unit/Player.h"
#include "NormalEnemy.h"

// モデルのHips
char FRAME_ENEMY_HIPS[] = "mixamorig:Hips";

const char PATH_NORMALENEMY[] = "Enemy/Anim";
const VECTOR ENEMY_MODEL_SCALE = { 0.5f,0.5f,0.5f };

// アニメーションのスピード
const float NORMAL_ANIM_SPEED = 60.0f;

// 回転の制御
const float ROTATION_MIN = 0.001f;

// 回転の限界値
const float DOT_MIN = 0.99f;

// ステップ数分割
const float DEVIDE_STEPCOUNT = 8.0f;

// 遠距離判定用
const float LONG_RANGE = 70.0f;

NormalEnemy::NormalEnemy(std::weak_ptr<Player> player) : ActorBase(),
animationController_(std::make_unique<AnimationController>(transform_.modelId)),
diff_(Utility::VECTOR_ZERO),
rotationStep_(0.0f), frameNo_(-1),
enemyHp_(10)
{
	player_ = player;
	stateChange_[STATE::IDLE] = std::bind(&NormalEnemy::ChangeIdle, this);
	stateChange_[STATE::WALK] = std::bind(&NormalEnemy::ChangeClose, this);
	stateChange_[STATE::ATTACK] = std::bind(&NormalEnemy::ChangeAttack, this);
	stateChange_[STATE::FLINCH] = std::bind(&NormalEnemy::ChangeFlinch, this);
	stateChange_[STATE::BLOW] = std::bind(&NormalEnemy::ChangeBlow, this);
	stateChange_[STATE::BLOW_AWAY] = std::bind(&NormalEnemy::ChangeBlowAway, this);
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Init(void)
{
	transform_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY);
	transform_.scl = ENEMY_MODEL_SCALE;
	transform_.pos = { 100.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		0.0f,
		0.0f
	);

	// アニメーションの初期化
	InitAnimation();

	// 腰のフレーム
	waistFrame_ = MV1SearchFrame(transform_.modelId, "mixamorig:Head");

	// モーション値無効化のため取得
	frameNo_ = MV1SearchFrame(transform_.modelId, FRAME_ENEMY_HIPS);

	// カウント初期化
	cntDelay_ = 2.0f;
	attackDelay_ = 2.0f;

	// 初期状態をIDLEに設定
	ChangeState(STATE::IDLE);
}

void NormalEnemy::Update(void)
{
	// 関数ポインタ更新
	stateUpdate_();

	// アニメーションの更新
	animationController_->Update();

	waistPos_ = MV1GetFramePosition(transform_.modelId, waistFrame_);

	// 重力をかける
	/*isGrounded_ = false;
	CalculateGravity();*/

	// モデルの更新
	transform_.Update();
}

void NormalEnemy::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
}

void NormalEnemy::Damage(int damageAmount)
{
	enemyHp_ -= damageAmount;
}

void NormalEnemy::ChangeState(STATE state)
{
	preState_ = state_;

	state_ = state;

	stateChange_[state_]();

	preAnimationKey_ = animationKey_;

	animationKey_ = ANIM_DATA_KEY[(int)state];

	animationController_->ChangeAnimation(animationKey_);
}

int NormalEnemy::GetHP() const
{
	return enemyHp_;
}

VECTOR NormalEnemy::GetPos() const
{
	return transform_.pos;
}

VECTOR NormalEnemy::GetCenterPos() const
{
	return waistPos_;
}

void NormalEnemy::SetPos(const VECTOR& pos)
{
	transform_.pos = pos;
}

void NormalEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + PATH_NORMALENEMY;
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーションの初期化
	// IDLE状態
	animationController_->Add("IDLE", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_IDLE), true, 0, false);
	// 移動アニメーション
	animationController_->Add("WALK", path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_WALK), false, 0, false);
	
	// 攻撃アニメーション
	animationController_->Add("ATTACK", path + "Attack.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ATTACK), false, 0, false);

	// 通常ダメージアニメーション
	animationController_->Add("FLINCH", path + "Flinch.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_FLINCH), false, 0, false);
	// 打ち上げダメージアニメーション
	animationController_->Add("BLOW", path + "Blow.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_BLOW), false, 0, false);
	// 吹き飛びダメージアニメーション
	animationController_->Add("BLOW_AWAY", path + "BlowAway.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_BLOW_AWAY), false, 0, false);
}

void NormalEnemy::DisableAnimMovePow(void)
{
	// 対象フレームのローカル行列を初期値にリセット
	MV1ResetFrameUserLocalMatrix(transform_.modelId, frameNo_);

	// 対象フレームのローカル行列(大きさ、回転、位置)を取得
	auto mat = MV1GetFrameLocalMatrix(transform_.modelId, frameNo_);
	auto mScl = MGetSize(mat);
	auto mRot = MGetRotElem(mat);
	auto mPos = MGetTranslateElem(mat);

	// 大きさ、回転、位置をローカル座標に戻す
	MATRIX mix = MGetIdent();
	mix = MMult(mix, MGetScale(mScl));	// 大きさ
	mix = MMult(mix, mRot);				// 回転

	// 調整したローカル座標を行列に設定
	mix = MMult(mix, MGetTranslate({ 0.0f,79.0f,0.0f }));

	// 移動値を無効化
	MV1SetFrameUserLocalMatrix(transform_.modelId, frameNo_, mix);
}

void NormalEnemy::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateIdle, this);
}

void NormalEnemy::ChangeClose(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateClose, this);
}

void NormalEnemy::ChangeAttack(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateAttack, this);
}

void NormalEnemy::ChangeFlinch(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateFlinch, this);
}

void NormalEnemy::ChangeBlow(void)
{
	// 上昇処理
	DisableAnimMovePow();
	transform_.pos.y += 1.0f; // 上昇量は調整してください
	stateUpdate_ = std::bind(&NormalEnemy::UpdateBlow, this);
}

void NormalEnemy::ChangeBlowAway(void)
{
	// 吹き飛ばし方向（モデルの背後方向）を取得
	VECTOR back = transform_.GetBack();

	// 吹き飛ばし速度
	float blowSpeed = 1.0f;

	// 背後方向に移動
	transform_.pos.x += back.x * blowSpeed;
	transform_.pos.y += back.y * blowSpeed;
	transform_.pos.z += back.z * blowSpeed;

	stateUpdate_ = std::bind(&NormalEnemy::UpdateBlowAway, this);
}

void NormalEnemy::UpdateIdle(void)
{
	// 回転
	rotationStep_ += ROTATION_MIN;
	attackDelay_ += SceneManager::GetInstance().GetDeltaTime();

	// プレイヤーの座標を取得
	VECTOR pPos = player_.lock()->GetPos();

	// エネミーからプレイヤーまでのベクトル
	diff_ = VSub(transform_.pos, pPos);
	diff_ = VNorm(diff_);

	//プレイヤーのベクトルと自分の前方向ベクトルとの差分(内積)
	dot_ = VDot(diff_, transform_.GetForward());

	// 球面補間を行う
	transform_.quaRot.x = 0.0f;
	transform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		transform_.quaRot, Quaternion::LookRotation(diff_), rotationStep_ / DEVIDE_STEPCOUNT);
	transform_.quaRot = rot_;

	//差分が限りなく1に近かったらWALKしない
	if (dot_ <= DOT_MIN)
	{
		//プレイヤーの方向を向くまではWALKをする
		animationController_->ChangeAnimation("WALK");
	}
	else
	{
		animationController_->ChangeAnimation("IDLE");
	}

	// エネミーからプレイヤーまでのベクトル
	VECTOR diff = VSub(pPos, transform_.pos);

	// XZ距離
	float distance = diff.x * diff.x + diff.z * diff.z;

	VECTOR vec;
	vec = VSub(pPos, transform_.pos);
	VECTOR direction = VNorm(vec);

	float length = Utility::MagnitudeF(vec);

	if (attackDelay_ >= cntDelay_)
	{
		attackDelay_ = 0.0f;
		rotationStep_ = 0.0f;
	}

	// プレイヤーに近づける
	if (distance >= LONG_RANGE * LONG_RANGE)
	{
		ChangeClose();
	}
}

void NormalEnemy::UpdateClose(void)
{
	// プレイヤーの座標を取得
	auto playerPtr = player_.lock();
	
	VECTOR pPos = playerPtr->GetPos();

	// エネミーからプレイヤーへのXZベクトル
	VECTOR toPlayer = VSub(pPos, transform_.pos);
	toPlayer.y = 0.0f;

	// XZ距離
	float distanceXZ = toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z;

	// 近すぎたら攻撃状態に遷移
	if (distanceXZ < 8.0f * 8.0f)
	{
		ChangeState(STATE::ATTACK);
		return;
	}

	// 進行方向
	VECTOR direction = VNorm(toPlayer);

	// 球面補間を行う
	transform_.quaRot.x = 0.0f;
	transform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		transform_.quaRot, Quaternion::LookRotation(diff_), rotationStep_ / DEVIDE_STEPCOUNT);
	transform_.quaRot = rot_;

	// 移動速度
	constexpr float MOVE_SPEED = 1.0f;
	VECTOR pos = transform_.pos;
	pos.x += direction.x * MOVE_SPEED;
	pos.z += direction.z * MOVE_SPEED;
	SetPos(pos);
}

void NormalEnemy::UpdateAttack(void)
{
	if (animationController_->IsEndPlayAnimation())
	{
		// 攻撃アニメーションが終わったらIDLEに戻す
		ChangeState(STATE::IDLE);
	}
}

void NormalEnemy::UpdateFlinch(void)
{
	if (animationController_->IsEndPlayAnimation())
	{
		// アニメーションが終わったらIDLEに戻る
		ChangeState(STATE::IDLE);
	}
}

void NormalEnemy::UpdateBlow(void)
{
	// 少しずつ下におろしていく
	if (transform_.pos.y >= 0.0f)
	{
		transform_.pos.y -= 0.1f;
	}

	if (transform_.pos.y <= 0.0f)
	{
		transform_.pos.y = 0.0f;
		// アニメーションが終わったらIDLEに戻し、アニメーションの移動値を元に戻す
		MV1ResetFrameUserLocalMatrix(transform_.modelId, frameNo_);
		ChangeState(STATE::IDLE);
	}
}

void NormalEnemy::UpdateBlowAway(void)
{
}
