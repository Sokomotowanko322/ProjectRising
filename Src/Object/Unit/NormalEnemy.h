#pragma once
#include <memory>
#include <vector>
#include "ActorBase.h"

class Player;

class NormalEnemy : public ActorBase
{
public:
	
	enum class STATE
	{
		IDLE,
		WALK,
		//RUN,
		ATTACK
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		//RUN,
		ATTACK,
		ALL
	};

	//�A�j���[�V����KEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		/*"RUN",*/
		"ATTACK",
	};

	// �R���X�g���N�^
	NormalEnemy(std::weak_ptr<Player> player);
	~NormalEnemy();
	
	// ������
	void Init(void);
	
	// �X�V
	void Update(void) override;
	
	// �`��
	void Draw(void) override;

	// ��ԑJ��
	void ChangeState(STATE state);

protected:


	
private:

	// �A�j���[�V�����R���g���[��
	std::unique_ptr<AnimationController> animationController_;
	
	// �A�j���[�V�����̏�����
	void InitAnimation(void);

	// �A�j���[�V�����J�ڗp
	STATE state_;
	STATE preState_;

	// STATE���Ɋi�[����L�[
	std::string animationKey_;
	std::string preAnimationKey_;

	// ���߂̏�Ԏ擾
	std::vector<std::string> currentState_;

	// STATE�̕ύX�A�֐����œ�����UPDATE�ƃA�j���[�V�������Ăяo��
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeAttack(void);

	// �X�V
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);

	// ��]�̍���
	VECTOR diff_;
	
	// �N�H�[�^�j�I����]
	Quaternion rot_;

	// ��]�̓���
	float dot_;
	
	// ��]�̃X�e�b�v
	float rotationStep_;
};

