#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../Common/Vector2.h"
#include "../Common/Vector2f.h"
#include "../Singleton.h"

//�R���g���[���[�̔ԍ�
enum class JOYPAD_NO
{
	KEY_PAD1,
	PAD1,
	PAD2,
	PAD3,
	PAD4,
	INPUT_KEY = 4096,
};


//�R���g���[���[�̃{�^��
enum class JOYPAD_BTN
{
	LEFT = 0,
	RIGHT,
	TOP,
	DOWN,
	R_TRIGGER,
	ZR_TRIGGER,
	R3_TRIGGER,
	L_TRIGGER,
	ZL_TRIGGER,
	L3_TRIGGER,
	START,
	CENTER,
	TEN_UP,
	TEN_DOWN,
	TEN_LEFT,
	TEN_RIGHT,
	MAX,
};

//�R���g���[���[�̃A�i���O�X�e�B�b�N
enum class JOYPAD_STICK
{
	L_STICK_UP = 0,
	L_STICK_DOWN,
	L_STICK_LEFT,
	L_STICK_RIGHT,

	R_STICK_UP,
	R_STICK_DOWN,
	R_STICK_LEFT,
	R_STICK_RIGHT,

	MAX,
};

//�R���g���[���[�̎��
enum class JOYPAD_TYPE
{
	OTHER = 0,
	XBOX_360,
	XBOX_ONE,			//XboxOne
	DUAL_SHOCK_3,
	DUAL_SHOCK_4,
	DUAL_SENCE,
	SWICH_JOY_CON_L,
	SWICH_JOY_CON_R,
	SWICH_JOY_PRO_CTRL,
	SWICH_HORI_PAD,
	MAX,
};

//�R���g���[���[�̓��͏��
struct JOYPAD_IN_STATE
{
	unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];		//1f�O�̃{�^������
	unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];		//���t���[���̃{�^������

	bool aKeyOld[static_cast<int>(JOYPAD_STICK::MAX)];		//1f�O�̃X�e�B�b�N����
	bool aKeyNew[static_cast<int>(JOYPAD_STICK::MAX)];		//���t���[���̃X�e�B�b�N����

	Vector2 AkeyL = {};
	Vector2 AkeyLOld = {};
	Vector2 AkeyR = {};
	Vector2 AkeyROld = {};

	JOYPAD_IN_STATE()
		:
		ButtonsOld{},
		ButtonsNew{},
		aKeyOld{},
		aKeyNew{}
	{};
};

/// <summary>
/// ���͐�����s���N���X
/// </summary>
class InputManager
	: public Singleton<InputManager>
{

public:

	static constexpr int STICK_NEUTRAL_XBOX_ONE = 0;		//XBOX�X�e�B�b�N : �j���[�g����
	static constexpr int STICK_X_LEFT_XBOX_ONE = -1000;		//XBOX�X�e�B�b�N : ��
	static constexpr int STICK_X_RIGHT_XBOX_ONE = 1000;		//XBOX�X�e�B�b�N : �E
	static constexpr int STICK_Y_UP_XBOX_ONE = -1000;		//XBOX�X�e�B�b�N : ��
	static constexpr int STICK_Y_DOWN_XBOX_ONE = 1000;		//XBOX�X�e�B�b�N : ��

	/// <summary>
	/// ���̓^�C�v
	/// </summary>
	enum class InputType
	{
		KEYBOARD,		//�L�[�{�[�h
		GAMEPAD,		//�Q�[���p�b�h
		MOUSE,		//�}�E�X
		GAMEPAD_STICK,		//�X�e�B�b�N
	};


	/// <summary>
	/// ���͏��
	/// </summary>
	struct InputMapInfo
	{
		InputType type;		//���͎��
		int buttonID_;		//�L�[����
		std::string KBname = "null";		//text�\��
	};

	/// <summary>
	/// �A�N�V�����e�[�u��
	/// </summary>
	enum class ACTION
	{
		ENTER,		//����
		PAUSE,		//�|�[�Y���
		SCENE_CHANGE,		//��ʑJ�ڗp

		//�I����ʗp
		SELECT_UP,		//�J�[�\����
		SELECT_DOWN,		//�J�[�\����
		SELECT_LEFT,		//�J�[�\����
		SELECT_RIGHT,		//�J�[�\���E
		SELECT_ENTER,		//����
		SELECT_CANCEL,		//�L�����Z��


		//�v���C���[����
		ATTACK,		//�U��
		BLOCK,		//�K�[�h
		DODGE,		//���
		RUN,		//����
		PULL,		//
		DRAW_UNDRAW,		//
		CAST,		//
		MOVE_PAD,		//
		MOVE_FORWARD,		//�O���ړ�
		MOVE_BACK,		//����ړ�
		MOVE_LEFT,		//���ړ�
		MOVE_RIGHT,		//�E�ړ�

		DEBUG,		//
		DEBUGCON,		//

		//�A�C�e��
		ITEMCALL,		//�A�C�e�����ďo
		DEBUG_EFFECT,		//
		NEXTITEM,		//�A�C�e�����O�ؑ�
		PREITEM,		//�A�C�e������ؑ�
		USEITEM,		//�g�p
		SELECT,		//�I��
		INHALE,		//�I���L�����Z��

		//�J����
		CAM_UP,		//���]
		CAM_DOWN,		//����]
		CAM_LEFT,		//����]
		CAM_RIGHT,		//�E��]

		MAX,		//�ő吔�J�E���g�p
	};

	/// <summary>
	/// �R���g���[���[�^�C�v
	/// </summary>
	enum class CONTROL_TYPE
	{
		KEY,
		CONTROLLER,
	};

	/// <summary>
	/// Dxlib_End()�O�̃��\�[�X���
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">�A�N�V�����̖��O</param>
	/// <returns>
	/// true : ������Ă���
	/// false : ������ĂȂ�
	/// </returns>
	bool IsPressed(const ACTION& action) const;

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">���ׂ����A�N�V����</param>
	/// <returns>
	/// true : �������ꂽ 
	/// false : ������ĂȂ����������ςȂ�
	/// </returns>
	bool IsTriggered(const ACTION& action) const;

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">���ׂ����A�N�V����</param>
	/// <returns>
	/// true : �������ꂽ
	/// false : ������Ă���
	/// </returns>
	bool IsReleased(const ACTION& action) const;

	/// <summary>
	/// �Ώۂ̃R���g���[���[�̃^�C�v���擾
	/// </summary>
	/// <param name="no">�R���g���[���[�ԍ�</param>
	/// <returns>JOYPAD_TYPE</returns>
	const JOYPAD_TYPE GetJPadType(const JOYPAD_NO no);

	/// <summary>
	/// //���݂̃p�b�h�̓��͏����擾
	/// </summary>
	/// <param name="no">�W���C�p�b�h�̔ԍ�</param>
	/// <returns>JOYPAD_IN_STATE</returns>
	const JOYPAD_IN_STATE GetJPadState(const JOYPAD_NO no);

	/// <summary>
	/// �}�E�X���W�̎擾
	/// </summary>
	/// <param name="pos">�}�E�X���W����������Vector2</param>
	void GetMousePos(Vector2& pos);

	/// <summary>
	/// UI�\��
	/// </summary>
	/// <param name="action">�A�N�V�����̖��O</param>
	/// <returns>ACTION�̕������Ԃ�</returns>
	const std::string& GetName(const ACTION& action) const;

	/// <summary>
	/// ���݂̓��̓^�C�v���擾
	/// </summary>
	/// <returns>InputType</returns>
	inline const InputType GetInputType(void) const
	{
		return currentInputType_;
	}

	/// <summary>
	/// �L�[���R���g���[���[���̎�ʂ��Z�b�g
	/// </summary>
	/// <param name="type">�Z�b�g������CONTROL_TYPE</param>
	void SetControlType(const CONTROL_TYPE type);

	/// <summary>
	///�L�[���R���g���[���[���̎�ʂ��Z�b�g
	/// </summary>
	/// <param name="type">CONTROL_TYPE�ɑΉ�����int�^</param>
	void SetControlType(const int type);

	/// <summary>
	/// �L�[���R���g���[���[���̎�ʂ��擾
	/// </summary>
	/// <returns>CONTROL_TYPE</returns>
	inline const CONTROL_TYPE GetControlType(void) const
	{
		return controlType_;
	}

	/// <summary>
	///�@�L�[���͏�Ԃ�
	/// </summary>
	/// <returns>
	/// true : �L�[����
	/// false : �L�[���͂ł͂Ȃ�
	/// </returns>
	const bool IsControlType_Key(void) const;

	/// <summary>
	/// �R���g���[���[���͏�Ԃ�
	/// </summary>
	/// <returns>
	/// true : �R���g���[���[���͂�
	/// false : �R���g���[���[���͂ł͂Ȃ�
	/// </returns>
	const bool IsControlType_Controller(void) const;

	/// <summary>
	/// �R���g���[���[�̊��x���Z�b�g
	/// </summary>
	/// <param name="sens">�R���g���[���[�̊��x(����)</param>
	void SetControllerSensitivity(const float sens);

	/// <summary>
	/// �R���g���[���[�̊��x���擾
	/// </summary>
	/// <returns>controllerSensitivity_��const�Q��</returns>
	inline const float& GetControllerSensitivity(void) const
	{
		return controllerSensitivity_;
	}

private:

	//�V���O���g���e���v���[�g�ɃC���X�^���X�̐���������
	friend class Singleton<InputManager>;

	//���͂ƃA�N�V�����̑Ή��e�[�u��
	std::unordered_map<ACTION, std::vector<InputMapInfo>> inputActionMaps_;

	//���݂̓��͏��
	std::unordered_map<ACTION, bool> currentInput_;

	//���݂̓��̓^�C�v
	InputType currentInputType_;

	//���O�̓��͏��
	std::unordered_map<ACTION, bool> lastInput_;

	//�R���g���[���[�̎��
	JOYPAD_TYPE joyPadType_;

	//�R���g���[���[�̓��͏��
	JOYPAD_IN_STATE joyPadState_;

	//DirectInput�Ŏ擾�o����R���g���[���[�̓��͏��
	DINPUT_JOYSTATE dstate_;

	//XInput�Ŏ擾�o����R���g���[���[�̓��͏��
	XINPUT_STATE xstate_;

	//�L�[���͂��R���g���[���[�̓��͂�
	CONTROL_TYPE controlType_;

	//�X�e�B�b�N�̓��͏��
	JOYPAD_STICK stick_;

	//�R���g���[���[�̊��x(����)
	float controllerSensitivity_;

	InputManager(void);
	~InputManager(void);

	/// <summary>
	/// DirectInput()����擾�o������͏��̎擾
	/// </summary>
	/// <param name="no">�R���g���[���[�̔ԍ�</param>
	/// <returns>DINPUT_JOYSTATE</returns>
	DINPUT_JOYSTATE GetJPadDInputState(const int no);

	/// <summary>
	/// XInput()����擾�o������͏��̎擾
	/// </summary>
	/// <param name="no">�R���g���[���[�̔ԍ�</param>
	/// <returns>XINPUT_STATE</returns>
	XINPUT_STATE GetJPadXInputState(const int no);

	//�w�肵���R���g���[���[�̓��͏��̎擾
	void SetJPadInState(const JOYPAD_NO joypadNo);

	//�ڑ������R���g���[���[�̐����擾
	int GetJPadNum(void);

	//�X�e�B�b�N�̓��͔̔�
	const bool GetHitJPadStick(const InputMapInfo& info, const int& pad);

	//�L�[�{�[�h�̓��͔���
	const bool GetHitKey(const InputMapInfo& info, const char* key, const int& pad, const int& mouse);

	//�R���g���[���[�{�^���̓��͔���
	const bool GetHitJPad(const InputMapInfo& info, const int& pad);

	//�X�e�B�b�N�̓��͔���
	const bool IsInputStick(const int stick, const int max);
};