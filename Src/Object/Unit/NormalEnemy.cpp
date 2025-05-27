#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Controller/AnimationController.h"
#include "../../Utility/Utility.h"
#include "NormalEnemy.h"


const char PATH_NORMALENEMY[] = "Enemy/Anim";
const VECTOR ENEMY_MODEL_SCALE = { 0.5f,0.5f,0.5f };


const float NORMAL_ANIM_SPEED = 60.0f;


NormalEnemy::NormalEnemy() : ActorBase(),
animationController_(std::make_unique<AnimationController>(transform_.modelId))
{
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
}

void NormalEnemy::UpdateWalk(void)
{
}

void NormalEnemy::UpdateAttack(void)
{
}
