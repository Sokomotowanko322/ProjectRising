#include <map>
#include <unordered_map>
#include <functional>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Common/Geometry2D.h"

class Transform;
//class ColliderController;

/// <summary>
/// �J����
/// </summary>
class Camera
{

public:

	/// <summary>
	/// �J�������[�h
	/// </summary>
	enum class MODE
	{
		FIXED_POINT,
		FREE,
		FOLLOW,
		FOLLOW_FIX,		//�p�x�ω��Ȃ�
		FOLLOW_MOUSE,
		SUCCESSFUL,
	};

	Camera(void);
	~Camera(void);

	/// <summary>
	/// ������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`��O�̃J�����̐ݒ�
	/// </summary>
	void SetBeforeDraw(void);

	/// <summary>
	/// �J�������W�̎擾
	/// </summary>
	/// <returns>�J�������W</returns>
	inline const VECTOR& GetPos(void) const
	{
		return pos_;
	}

	/// <summary>
	/// �J�����̑���p�x
	/// </summary>
	/// <returns>�J�����̊p�x</returns>
	inline const VECTOR& GetAngles(void) const
	{
		return angles_;
	}

	/// <summary>
	/// �J�����̃f�t�H���g�̊p�x��ݒ�
	/// </summary>
	/// <param name="qua">�J�����̃f�t�H���g�̊p�x : Quaternion</param>
	void SetDefQuaRot(const Quaternion& qua);

	/// <summary>
	/// �J�����̒����_���擾
	/// </summary>
	/// <returns>�J�����̒����_ : VECTOR</returns>
	inline const VECTOR& GetTargetPos(void) const
	{
		return targetPos_;
	}

	/// <summary>
	/// �J�����̊p�x��ݒ�
	/// </summary>
	/// <param name="angle">�J�����̊p�x(�f�O���[�p) : VECTOR</param>
	void SetAngle(const VECTOR& angle);

	/// <summary>
	/// �J�����p�x�̎擾
	/// </summary>
	/// <returns>�J�����p�x : Quaternion</returns>
	inline const Quaternion& GetQuaRot(void) const
	{
		return rot_;
	}

	/// <summary>
	/// X��]�𔲂����J�����p�x�̎擾
	/// </summary>
	/// <returns>Quaternion</returns>
	inline const Quaternion& GetQuaRotOutX(void) const
	{
		return rotOutX_;
	}

	/// <summary>
	/// �J�����̑O���������擾
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetForward(void) const;

	/// <summary>
	/// �J�������[�h�̕ύX
	/// </summary>
	/// <param name="mode">�ύX����J�������[�h</param>
	void ChangeMode(const MODE mode);

	/// <summary>
	/// �Ǐ]�Ώۂ̐ݒ�
	/// </summary>
	/// <param name="follow">�Ǐ]�Ώۂ̐�����</param>
	void SetFollow(const Transform* follow);

	/// <summary>
	/// �����蔻����Ƃ�X�e�[�W���f�����Z�b�g
	/// </summary>
	/// <param name="modelId">�X�e�[�W���f���̃n���h��</param>
	void SetHitModel(const int modelId);

	/// <summary>
	/// �J�����̉�]�͂�ݒ�
	/// </summary>
	/// <param name="pow">�c�������̃J�����̉�]�� : Vector2f</param>
	void SetCameraRotPow(const Vector2f& pow);

	/// <summary>
	/// ���݂̃}�E�X���W��VECTOR�^�Ŏ擾
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetMousePos(void);

	/// <summary>
	/// �������J�����I�����Ɏ��s����֐���o�^
	/// </summary>
	/// <param name="func">�������J�����I�����Ɏ��s����֐�</param>
	void SetSuccessCameraMoveEndFunc(std::function<void(void)> func);

	/// <summary>
	/// �J�����V�F�C�N��ݒ�
	/// </summary>
	/// <param name="time">�J�����V�F�C�N�̎���</param>
	/// <param name="shakePow">�h�炷��</param>
	/// <param name="timeScale">�o�ߎ��Ԃ̉�����</param>
	void SetCameraShake(float time, float shakePow, float timeScale = 10.0f);

	/// <summary>
	/// �J�����V�F�C�N���s��
	/// </summary>
	void CameraShake(void);

private:

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// ��t���[���O�̃J�����̈ʒu
	VECTOR prePos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion rotOutX_;

	// �J�����p�x
	Quaternion rot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// ���̃J�������W
	VECTOR currentLocalPos_;

	//1f�O�̃J�������Ǐ]����}�E�X���W
	Vector2 preMousePos_;

	//���݂̃J�������Ǐ]����}�E�X���W
	Vector2 nowMousePos_;

	//�}�E�X���쎞�̃J�����̉�]�p�x
	Vector2f radXY_;

	//�J�����̉�]��
	Vector2f cameraRotPow_;

	//�f�t�H���g�̃J�����p�x
	Quaternion defSetRot_;

	//�J�����V�F�C�N���Ă��邩
	bool isShake_;

	//�J�����V�F�C�N���̌o�ߎ���
	float shakeStep_;

	//�J�����V�F�C�N���̈ړ���
	float shakePow_;

	//�o�ߎ��Ԃ̉�����
	float timeScale_;

	//�������J�����̌o�ߎ���
	float successCameraTimeStep_;

	//�N���A���̃J�������[�N���I������ۂɎ��s����֐�
	std::function<void(void)> successCameraEnd_;

	//�X�e�[�W���f��ID
	int stagemodelId_;

	//�}�E�X���쎞�̒����_�̃��[�J�����W
	VECTOR  localPosMouseFollow_;

	//�ǏՓˌ��m�p�̐����̒���
	float  collitionLineStageCamera_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollow(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollowFix(void);

	// �}�E�X���쎞:�Ǐ]�ΏۂƂ̈ʒu���������
	void SyncMouse(void);

	// �N���A�����o:�Ǐ]�ΏۂƂ̈ʒu���������
	void SyncSuccessCamera(void);

	// �J��������
	void ProcessRot(void);

	// �}�E�X���쎞�̃J��������
	void ProcessRotMouse(void);

	//�N���A���J�����ړ�
	void SuccessCameraMove(void);

	//��ԑJ��
	std::unordered_map<MODE, std::function<void(void)>> modeChanges_;
	void ChangeFixedPoint(void);		//��_
	void ChangeFree(void);		//���R���_
	void ChangeFollow(void);		//���R���_�Ǐ]
	void ChangeFollowFix(void);		//��_�Ǐ]
	void ChangeFollowMouse(void);		//�}�E�X�Ǐ]
	void ChangeSuccessful(void);		//�N���A���J�������o

	// ���[�h�ʍX�V�X�e�b�v
	std::function<void(void)> modeUpdate_;
	void UpdateFixedPoint(void);	//��_	
	void UpdateFree(void);		//���R���_
	void UpdateFollow(void);		//���R���_�Ǐ]
	void UpdateFollowFix(void);		//��_�Ǐ]
	void UpdateFollowMouse(void);		//�}�E�X�Ǐ]
	void UpdateSuccessful(void);		//�N���A���J�������o

	// ���[�h�ʕ`��O�J�����ݒ�X�e�b�v
	std::function<void(void)> setBeforeDrawMode_;
	void SetBeforeDrawFixedPoint(void);		//��_
	void SetBeforeDrawFree(void);		//���R���_
	void SetBeforeDrawFollow(void);		//���R���_�Ǐ]
	void SetBeforeDrawFollowFix(void);			//��_�Ǐ]	
	void SetBeforeDrawFollowMouse(void);		//�}�E�X�Ǐ]
	void SetBeforeDrawSuccessful(void);		//�N���A���J�������o

	//�f�o�b�O�p
	void DrawDebug(void);

	//�J�����̕ǏՓ�
	void CameraCollision(void);

	//�J�����̕ǏՓ˔���p
	bool HitCheck(VECTOR campos, VECTOR Ppos, float size);

};