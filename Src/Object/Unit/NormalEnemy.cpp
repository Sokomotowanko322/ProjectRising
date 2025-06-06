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

// �A�j���[�V�����̃X�s�[�h
const float NORMAL_ANIM_SPEED = 60.0f;

// ��]�̐���
const float ROTATION_MIN = 0.001f;

// ��]�̌��E�l
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

	// �A�j���[�V�����̏�����
	InitAnimation();

	ChangeState(STATE::IDLE);
}

void NormalEnemy::Update(void)
{
	// �֐��|�C���^�X�V
	stateUpdate_();

	// �A�j���[�V�����̍X�V
	animationController_->Update();

	// ���f���̍X�V
	transform_.Update();
}

void NormalEnemy::Draw(void)
{
	// ���f���̕`��
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

	// �A�j���[�V�����̏�����
	// IDLE���
	animationController_->Add("IDLE", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_IDLE), true, 0, false);
	// �ړ��A�j���[�V����
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
	// ��]
	rotationStep_ += ROTATION_MIN;

	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	diff_ = VSub(transform_.pos, pPos);
	diff_ = VNorm(diff_);

	//�v���C���[�̃x�N�g���Ǝ����̑O�����x�N�g���Ƃ̍���(����)
	dot_ = VDot(diff_, transform_.GetForward());

	// ���ʕ�Ԃ��s��
	transform_.quaRot.x = 0.0f;
	transform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		transform_.quaRot, Quaternion::LookRotation(diff_), rotationStep_ / DEVIDE_STEPCOUNT);
	transform_.quaRot = rot_;

	//����������Ȃ�1�ɋ߂�������WALK���Ȃ�
	if (dot_ <= DOT_MIN)
	{
		//�v���C���[�̕����������܂ł�WALK������
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
