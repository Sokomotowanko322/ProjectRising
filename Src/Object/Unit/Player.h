#include "ActorBase.h"

class AnimationController;
class Weapon; 

class Player : public ActorBase//, public ColliderBase
{
public:

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		DASH,
		HIGHTIME,
		SMASH,
		FIRST_COMBO,
		DODGE,
		COUNTER,
		HASWEAPON,
		ALL
	};

	//�A�j���[�V����KEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"RUN",
		"DASH",
		"HIGHTIME",
		"SMASH",
		"FIRST_COMBO",
		"DODGE",
		"COUNTER",
		"HASWEAPON",
	};


	// �R���X�g���N�^
	Player();
	~Player() override;

	// ������
	void Init(void);

	// �X�V
	void Update(void) override;

	// �`��
	void Draw(void) override;

	// ����
	void ProcessInput(void);
	void MoveControl(void);
	void SetPos(const VECTOR& pos);

	// �擾�n
	VECTOR& GetPos() const override;
	const VECTOR& GetRightHandPos() const;
	const VECTOR& GetAngles() const;

private:

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// ����(��Ƀv���C���[�p�Ƃ��ċ@�\������)
	std::shared_ptr<Weapon> weapon_;

	// �E��̍��W
	VECTOR rightHandPos_;

	// �ړ�����
	VECTOR moveDir_;

	// �v���C���[�̈ړ���
	VECTOR movePow_;

	// �E��̃t���[��
	int rightHandFrame_;

	// �u�����h�p�t���[���ԍ�
	int frameNo_;

	// ���͐���p
	// -1�������疢����
	int moveForwardCount_;
	int backInputFrame_ = -1;
	bool preBackPressed_;
	bool preForwardPressed_;
	bool readyHighTime_;

	// �A�j���[�V������ێ�
	int armAnimId_;
	int legsAnimId_;

	// �v���C���[�̈ړ����x
	float moveSpeed_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float rotRad_;
	float stepRotTime_;

	// �t���O�Ǘ�
	bool isAttack_;

	// �A�j���[�V�����̏�����
	void InitAnimation(void);
	
	// �u�����h�e�X�g
	void BlendAnimation(void);

	// �A�j���[�V�����ړ��n������
	void DisableAnimMovePow(void);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);
};