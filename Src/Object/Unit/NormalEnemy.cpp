#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Controller/AnimationController.h"
#include "../../Utility/Utility.h"
#include "../../Object/Unit/Player.h"
#include "NormalEnemy.h"


const char PATH_NORMALENEMY[] = "Enemy/Anim";
const VECTOR ENEMY_MODEL_SCALE = { 0.5f,0.5f,0.5f };

// アニメーションのスピード
const float NORMAL_ANIM_SPEED = 60.0f;

// 回転の制御
const float ROTATION_MIN = 0.001f;

// 回転の限界値
const float DOT_MIN = 0.99f;

// 
const float DEVIDE_STEPCOUNT = 8.0f;


NormalEnemy::NormalEnemy(std::weak_ptr<Player> player) : ActorBase(),
animationController_(std::make_unique<AnimationController>(transform_.modelId)),
diff_(Utility::VECTOR_ZERO),
rotationStep_(0.0f)
{
	player_ = player;
	stateChange_[STATE::IDLE] = std::bind(&NormalEnemy::ChangeIdle, this);
	stateChange_[STATE::WALK] = std::bind(&NormalEnemy::ChangeWalk, this);
	stateChange_[STATE::ATTACK] = std::bind(&NormalEnemy::ChangeAttack, this);
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Init(void)
{
	transform_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY);
	transform_.scl = ENEMY_MODEL_SCALE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		Utility::Deg2RadF(180.0f),
		0.0f
	);

	// アニメーションの初期化
	InitAnimation();

	ChangeState(STATE::IDLE);
}

void NormalEnemy::Update(void)
{
	// 関数ポインタ更新
	stateUpdate_();

	// アニメーションの更新
	animationController_->Update();

	// モデルの更新
	transform_.Update();
}

void NormalEnemy::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
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

VECTOR NormalEnemy::GetPos() const
{
	return transform_.pos;
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
}

void NormalEnemy::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateIdle, this);
}

void NormalEnemy::ChangeWalk(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateWalk, this);
}

void NormalEnemy::ChangeAttack(void)
{
	stateUpdate_ = std::bind(&NormalEnemy::UpdateAttack, this);
}

void NormalEnemy::UpdateIdle(void)
{
	// 回転
	rotationStep_ += ROTATION_MIN;

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
}

void NormalEnemy::UpdateWalk(void)
{
}

void NormalEnemy::UpdateAttack(void)
{
}
