#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "../Object/Transform.h"
//#include "../Object/Common/Collider/ColliderController.h"
#include "Camera.h"

using namespace std;

const float INIT_STEP_CAMERASHAKE = Utility::FLOAT_ZERO;
const float INIT_POW_CAMERASHAKE = Utility::FLOAT_ZERO;
const float INIT_TIMESCALE_CAMERASHAKE = Utility::FLOAT_ONE;

const float ROT_POW_CAMERA = 1.0f;		//�J�����X�s�[�h(�x)
const float CAMERACLIP_NEAR = 10.0f;		//�J�����N���b�v�FNEAR
const float CAMERACLIP_FAR = 30000.0f;		//�J�����N���b�v�FNEAR
const VECTOR INIT_CAMERA_POS = { 0.0f, 100.0f, -200.0f };		//�J�����̏������W

const float SPEED_FREECAMERA = 2.0f;
const float ROT_POW_FREECAMERA = 2.0f;

const VECTOR LOCAL_POS_FOLLOWER_CAMERA = { 30.0f, 3.0f, -120.0f };		//�Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
const VECTOR LOCAL_POS_FOLLOWER_DASH_CAMERA = { 30.0f, 1.0f, -150.0f };		//�Ǐ]�ʒu����J�����ʒu�܂ł̃_�b�V�������΍��W
const VECTOR LOCAL_POS_FOLLOWER_TARGET = { 0.0f, 0.0f, 500.0f };		//�Ǐ]�ʒu���璍���_�܂ł̑��΍��W

const VECTOR LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW = { 0.0f, 100.0f, 500.0f };		//�}�E�X�Ǐ]���̒����_����J�����܂ł̑��΍��W
const VECTOR LOCAL_POS_PLAYER_CAMERA_MOUSEFOLLOW = { 0.0f, 100.0f,200.0f };		//�}�E�X�Ǐ]���̎��@����J�����܂ł̑��΍��W
const Vector2 INIT_POS_MOUSE = { Application::SCREEN_SIZE_HALF_X ,Application::SCREEN_SIZE_HALF_Y };		//�}�E�X�Ǐ]���̃}�E�X�J�[�\���������W
const float MIN_CAMERA_DEG_Y_MOUSE = 0.0f;		//�}�E�X�Ǐ]���̃J�����̉���
const float MAX_CAMERA_DEG_Y_MOUSE = 70.0f;		//�}�E�X�Ǐ]���̃J�����̏��

const VECTOR LOCAL_C_POS_SUCCESSFUL = { 0.0f,1600.0f,1200.0f };		//�������̒Ǐ]�Ώۂ���J�������W���΍��W
const VECTOR LOCAL_POS_CAMERA_SUCCESSFUL = { 0.0f,300.0f,100.0f };		//�������̒Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W
const VECTOR LOCAL_C_POS_SUCCESSFUL_FIX = { 800.0f,1600.0f,-1200.0f };		//�������̒�_�J�����̑��΍��W

const float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);		//�J������X��]�p���
const float LIMIT_X_DW_RAD = -1.0f * (DX_PI_F / 180.0f);		//�J������X��]�p����

const Vector2f DEF_CAMERA_ROT_POW = { 10.0f,10.0f };		//�f�t�H���g�̃J�����̉�]���x

const float INIT_TIME_SUCCESSCAMERA = Utility::FLOAT_ZERO;		//�������̃J�������o������
const float MAX_TIME_SUCCESSCAMERA = 18.0f;		//�������̃J�������o����
const float TIME_SUCCESSCAMERA_CHANGEMOVE_Y = 12.0f;		//Y����]�ɐ؂�ւ�鎞��
const float TIME_SUCCESSCAMERA_CHANGEMOVE_X = 6.0f;		//X����]�ɐ؂�ւ�鎞��
const float INIT_SUCCESSCAMERA_DEG_XY = 0.0f;		//�������J�����̏������p
const float DECELERATE_RATE_SUCCESSCAMERA = 0.05f;		//�������J�����̌�����

const float INIT_CAMERA_DEF_X = 30.0f;
const Vector2f DEF_MOUSECAMERA_DEG = { 0.0f,20.0f };

const float INIT_COLLISION_LINE_STAGE = Utility::FLOAT_ZERO;

const float LOCAL_POS_Y_CAMERACOLLISION = 200.0f;
const float LEN_CAMERACOLLISION_LINE = 500.0f;
const float MIN_LEN_CAMERACOLLISION_LINE = 1.0f;
const int NUM_NOTHIT_POLY = 0;


const float CAMERA_SENSITIVITY = 10000.0f;

Camera::Camera(void)
	:
	followTransform_(nullptr),
	mode_(MODE::FIXED_POINT),
	pos_{},
	prePos_{},
	angles_{},
	rotOutX_(Quaternion()),
	rot_(Quaternion()),
	targetPos_{},
	cameraUp_{},
	preMousePos_{},
	nowMousePos_{},
	currentLocalPos_{ LOCAL_POS_FOLLOWER_CAMERA },
	radXY_{},
	cameraRotPow_(DEF_CAMERA_ROT_POW),
	defSetRot_(Quaternion()),
	isShake_(false),
	shakeStep_(INIT_STEP_CAMERASHAKE),
	shakePow_(INIT_POW_CAMERASHAKE),
	timeScale_(INIT_TIMESCALE_CAMERASHAKE),
	successCameraTimeStep_(INIT_TIME_SUCCESSCAMERA),
	successCameraEnd_{},
	stagemodelId_(Utility::HANDLEID_NULL),
	localPosMouseFollow_(LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW),
	collitionLineStageCamera_(INIT_COLLISION_LINE_STAGE),
	modeChanges_{},
	modeUpdate_{},
	setBeforeDrawMode_{}
{

	modeChanges_.emplace(MODE::FIXED_POINT, bind(&Camera::ChangeFixedPoint, this));
	modeChanges_.emplace(MODE::FREE, bind(&Camera::ChangeFree, this));
	modeChanges_.emplace(MODE::FOLLOW, bind(&Camera::ChangeFollow, this));
	modeChanges_.emplace(MODE::FOLLOW_FIX, bind(&Camera::ChangeFollowFix, this));
	modeChanges_.emplace(MODE::FOLLOW_MOUSE, bind(&Camera::ChangeFollowMouse, this));
	modeChanges_.emplace(MODE::SUCCESSFUL, bind(&Camera::ChangeSuccessful, this));

}

Camera::~Camera(void)
{

	modeChanges_.clear();

}

void Camera::Init(void)
{

	//colliderController_ = make_unique<ColliderController>();
	//colliderController_->SetCollision(OBJECT_TYPE::STAGE);
	localPosMouseFollow_ = LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW;

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{

	modeUpdate_();

	CameraShake();

}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERACLIP_NEAR, CAMERACLIP_FAR);


	setBeforeDrawMode_();

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}

void Camera::SetCameraRotPow(const Vector2f& pow)
{

	cameraRotPow_ = pow;

}

const VECTOR Camera::GetMousePos(void)
{

	VECTOR ret = {};
	Vector2f retPos = ToVector2f(nowMousePos_);
	ret = { retPos.x,retPos.y,Utility::FLOAT_ZERO };

	return ret;

}

void Camera::SetSuccessCameraMoveEndFunc(std::function<void(void)> func)
{
	successCameraEnd_ = func;
}

void Camera::SetCameraShake(float time, float shakePow, float timeScale)
{

	if (isShake_) { return; }

	isShake_ = true;
	shakeStep_ = time;
	shakePow_ = shakePow;
	timeScale_ = timeScale;

}

void Camera::CameraShake(void)
{

	const int mod = 3;
	const int subVal = 1;

	if (!isShake_) { return; }

	Vector2f shakePos = { pos_.x,pos_.y };

	Vector2 pow = Vector2(
		(int)(shakeStep_ * timeScale_) % mod - subVal,
		(int)(shakeStep_ * timeScale_) % mod - subVal
	);

	Vector2f shakeMove = ToVector2f(pow) * shakePow_;
	shakePos += shakeMove;

	pos_.x = shakePos.x;
	pos_.y = shakePos.y;

	shakeStep_ -= TimeManager::GetInstance().GetDeltaTime();
	if (shakeStep_ < INIT_STEP_CAMERASHAKE)
	{
		isShake_ = false;
	}

}

void Camera::SetDefQuaRot(const Quaternion& qua)
{

	defSetRot_ = qua;

	auto& insI = InputManager::GetInstance();
	if (insI.IsControlType_Key())
	{
		radXY_ = Vector2f();
		radXY_.y = Utility::Deg2RadF(DEF_MOUSECAMERA_DEG.y);
	}
	if (insI.IsControlType_Controller())
	{
		angles_ = qua.ToEuler();
	}

}

void Camera::SetAngle(const VECTOR& angle)
{

	angles_ = angle;

}

const VECTOR Camera::GetForward(void) const
{

	return VNorm(VSub(targetPos_, pos_));

}

void Camera::ChangeMode(const MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	mode_ = mode;
	modeChanges_[mode_]();

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = INIT_CAMERA_POS;

	// �����_
	targetPos_ = Utility::VECTOR_ZERO;

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(INIT_CAMERA_DEF_X);
	angles_.y = Utility::FLOAT_ZERO;
	angles_.z = Utility::FLOAT_ZERO;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{
	VECTOR pos = followTransform_->pos;

	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	// �����_
	VECTOR localTarget = rotOutX_.PosAxis(LOCAL_POS_FOLLOWER_TARGET);
	targetPos_ = VAdd(pos, localTarget);

	// �ڕW�̃J�������΍��W������
	VECTOR targetLocalPos;
	if (InputManager::GetInstance().IsPressed(InputManager::ACTION::RUN)) 
	{
		targetLocalPos = LOCAL_POS_FOLLOWER_DASH_CAMERA;
	}
	else 
	{
		targetLocalPos = LOCAL_POS_FOLLOWER_CAMERA;
	}

	// ���`��ԁiLerp�j�Ŋ��炩�Ɉړ�
	const float lerpSpeed = 0.1f; // �l��傫������Ƒ����؂�ւ��
	currentLocalPos_.x += (targetLocalPos.x - currentLocalPos_.x) * lerpSpeed;
	currentLocalPos_.y += (targetLocalPos.y - currentLocalPos_.y) * lerpSpeed;
	currentLocalPos_.z += (targetLocalPos.z - currentLocalPos_.z) * lerpSpeed;

	VECTOR localPos = rot_.PosAxis(currentLocalPos_);
	pos_ = VAdd(pos, localPos);

	cameraUp_ = Utility::DIR_U;
}

void Camera::SyncFollowFix(void)
{

	//auto& gIns = GravityManager::GetInstance();

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	//�p�x�ω�
	Quaternion gRot = Quaternion::Euler(Utility::VECTOR_ZERO);

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_POS_FOLLOWER_TARGET);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	if (mode_ == MODE::SUCCESSFUL)
	{
		localPos = rot_.PosAxis(LOCAL_C_POS_SUCCESSFUL_FIX);
	}
	else
	{
		localPos = rot_.PosAxis(LOCAL_POS_FOLLOWER_CAMERA);
	}

	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = gRot.GetUp();

}

void Camera::SyncMouse(void)
{
	auto dT = SceneManager::GetInstance().GetDeltaTime();

	//�p�x�ω�
	Quaternion gRot = defSetRot_;

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	Vector2f vec;
	Vector2f vecN;
	//float len;

	preMousePos_ = nowMousePos_;
	GetMousePoint(&nowMousePos_.x, &nowMousePos_.y);
	radXY_.x += Utility::Deg2RadF(
		float(nowMousePos_.x - INIT_POS_MOUSE.x) * cameraRotPow_.x * dT
	);

	if (Utility::Rad2DegF(radXY_.y) <= MAX_CAMERA_DEG_Y_MOUSE && Utility::Rad2DegF(radXY_.y) >= MIN_CAMERA_DEG_Y_MOUSE)
	{
		radXY_.y -= Utility::Deg2RadF(
			float(nowMousePos_.y - INIT_POS_MOUSE.y) * cameraRotPow_.y * dT
		);

		if (Utility::Rad2DegF(radXY_.y) < MIN_CAMERA_DEG_Y_MOUSE)
		{
			radXY_.y = Utility::Deg2RadF(MIN_CAMERA_DEG_Y_MOUSE);
		}
		if (Utility::Rad2DegF(radXY_.y) > MAX_CAMERA_DEG_Y_MOUSE)
		{
			radXY_.y = Utility::Deg2RadF(MAX_CAMERA_DEG_Y_MOUSE);
		}
	}

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(radXY_.x, Utility::AXIS_Y));

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(radXY_.y, Utility::AXIS_X));

	angles_ = Quaternion::ToEuler(rot_);

	VECTOR localPos;
	localPos = rotOutX_.PosAxis(LOCAL_POS_PLAYER_CAMERA_MOUSEFOLLOW);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	//localPos = rot_.PosAxis(LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW);
	localPos = rot_.PosAxis(localPosMouseFollow_);
	prePos_ = pos_;
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = gRot.GetUp();

	SetMousePoint(INIT_POS_MOUSE.x, INIT_POS_MOUSE.y);

	ProcessRot();

}

void Camera::SyncSuccessCamera(void)
{

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

	// ���ʂ���ݒ肳�ꂽX�����A��]������

	rot_ = followTransform_->quaRot;
	rot_ = rot_.Mult(rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X)));



	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_POS_CAMERA_SUCCESSFUL);
	targetPos_ = VAdd(pos, localPos);

	localPos = rot_.PosAxis(LOCAL_C_POS_SUCCESSFUL);
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::ProcessRot(void)
{
	auto& ins = InputManager::GetInstance();

	auto padRstick = ins.GetJPadState(JOYPAD_NO::PAD1).AkeyR;

	float rotPow = ROT_POW_CAMERA;

	//�E��]
	if (ins.IsPressed(InputManager::ACTION::CAM_RIGHT))
	{
		angles_.y += Utility::Deg2RadF(rotPow);
	}
	if (ins.IsPressed(InputManager::ACTION::CAM_LEFT))
	{
		angles_.y += Utility::Deg2RadF(-rotPow);
	}


	if (angles_.x <= LIMIT_X_UP_RAD)
	{
		if (ins.IsPressed(InputManager::ACTION::CAM_UP))
		{
			angles_.x += Utility::Deg2RadF(rotPow);
		}
	}

	if (angles_.x >= -LIMIT_X_DW_RAD)
	{
		if (ins.IsPressed(InputManager::ACTION::CAM_DOWN))
		{
			angles_.x += Utility::Deg2RadF(-rotPow);
		}
	}

	if (padRstick.x != InputManager::STICK_NEUTRAL_XBOX_ONE || padRstick.y != InputManager::STICK_NEUTRAL_XBOX_ONE)
	{
		if (angles_.x >= -LIMIT_X_DW_RAD && padRstick.y < InputManager::STICK_NEUTRAL_XBOX_ONE)
		{
			angles_.x += padRstick.y / CAMERA_SENSITIVITY;
		}

		if (angles_.x <= LIMIT_X_UP_RAD && padRstick.y > InputManager::STICK_NEUTRAL_XBOX_ONE)
		{
			angles_.x += padRstick.y / CAMERA_SENSITIVITY;
		}

		angles_.y += padRstick.x / CAMERA_SENSITIVITY;
	}

}

void Camera::ProcessRotMouse(void)
{

}

void Camera::SuccessCameraMove(void)
{

	float rotPow = ROT_POW_CAMERA * DECELERATE_RATE_SUCCESSCAMERA;

	if (successCameraTimeStep_ < TIME_SUCCESSCAMERA_CHANGEMOVE_X)
	{
		//�����Ȃ�
	}
	else if (
		successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_X
		&& successCameraTimeStep_ < TIME_SUCCESSCAMERA_CHANGEMOVE_Y
		)
	{
		angles_.x += Utility::Deg2RadF(rotPow);
	}
	else if (successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_Y)
	{
		angles_.x = Utility::FLOAT_ZERO;
		angles_.y += Utility::Deg2RadF(-rotPow);
	}

}

void Camera::ChangeFixedPoint(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFixedPoint, this);
	modeUpdate_ = bind(&Camera::UpdateFixedPoint, this);

}

void Camera::ChangeFree(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFree, this);
	modeUpdate_ = bind(&Camera::UpdateFree, this);

}

void Camera::ChangeFollow(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollow, this);
	modeUpdate_ = bind(&Camera::UpdateFollow, this);

}

void Camera::ChangeFollowFix(void)
{

	modeUpdate_ = bind(&Camera::UpdateFollowFix, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollowFix, this);

}

void Camera::ChangeFollowMouse(void)
{

	modeUpdate_ = bind(&Camera::UpdateFollowMouse, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollowMouse, this);
	radXY_.y = Utility::Deg2RadF(DEF_MOUSECAMERA_DEG.y);

	SetMousePoint(INIT_POS_MOUSE.x, INIT_POS_MOUSE.y);

}

void Camera::ChangeSuccessful(void)
{

	successCameraTimeStep_ = INIT_TIME_SUCCESSCAMERA;
	angles_.x = INIT_SUCCESSCAMERA_DEG_XY;
	angles_.y = INIT_SUCCESSCAMERA_DEG_XY;

	modeUpdate_ = bind(&Camera::UpdateSuccessful, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawSuccessful, this);

}

void Camera::UpdateFixedPoint(void)
{

}

void Camera::UpdateFree(void)
{

	auto& insI = InputManager::GetInstance();

	VECTOR dir = Utility::VECTOR_ZERO;

	//�O�㍶�E�㉺
	if (insI.IsPressed(InputManager::ACTION::MOVE_FORWARD))
	{
		dir = Utility::DIR_F;
	}
	if (insI.IsPressed(InputManager::ACTION::MOVE_LEFT))
	{
		dir = Utility::DIR_L;
	}
	if (insI.IsPressed(InputManager::ACTION::MOVE_BACK))
	{
		dir = Utility::DIR_B;
	}
	if (insI.IsPressed(InputManager::ACTION::MOVE_RIGHT))
	{
		dir = Utility::DIR_R;
	}

	if (insI.IsPressed(InputManager::ACTION::DEBUGCON))
	{
		dir = Utility::DIR_D;
	}

	if (insI.IsPressed(InputManager::ACTION::RUN))
	{
		dir = Utility::DIR_U;
	}


	//�p�x
	float rotPow = Utility::Deg2RadF(ROT_POW_FREECAMERA);
	if (insI.IsPressed(InputManager::ACTION::CAM_UP))
	{
		angles_.x -= rotPow;
	}
	if (insI.IsPressed(InputManager::ACTION::CAM_DOWN))
	{
		angles_.x += rotPow;
	}
	if (insI.IsPressed(InputManager::ACTION::CAM_LEFT))
	{
		angles_.y -= rotPow;
	}
	if (insI.IsPressed(InputManager::ACTION::CAM_RIGHT))
	{
		angles_.y += rotPow;
	}

	Quaternion qua = Quaternion::Euler(angles_);

	VECTOR moveDir = qua.PosAxis(dir);

	pos_ = VAdd(pos_, VScale(moveDir, SPEED_FREECAMERA));

}

void Camera::UpdateFollow(void)
{

	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

	CameraCollision();

}

void Camera::UpdateFollowFix(void)
{

	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollowFix();

}

void Camera::UpdateFollowMouse(void)
{

	VECTOR mousePos = {};

	//�J��������
	ProcessRotMouse();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncMouse();

	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollowFix();

	CameraCollision();

}

void Camera::UpdateSuccessful(void)
{

	// �J��������
	SuccessCameraMove();

	if (successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_X)
	{
		// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
		SyncSuccessCamera();
	}
	else
	{
		SyncFollowFix();
	}

	if (successCameraTimeStep_ > MAX_TIME_SUCCESSCAMERA)
	{
		successCameraEnd_();
	}

	successCameraTimeStep_ += TimeManager::GetInstance().GetDeltaTime();

}

void Camera::SetBeforeDrawFixedPoint(void)
{

	// �������Ȃ�
		// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFree(void)
{

	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);

}

void Camera::SetBeforeDrawFollow(void)
{

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFollowFix(void)
{

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFollowMouse(void)
{

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	SetMouseDispFlag(false);

}

void Camera::SetBeforeDrawSuccessful(void)
{
	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::DrawDebug(void)
{

}

void Camera::CameraCollision(void)
{

	VECTOR Ppos = followTransform_->pos;
	Ppos.y += LOCAL_POS_Y_CAMERACOLLISION;

	//float Radius = 1.0f;

	auto hit = MV1CollCheck_Line(
		stagemodelId_, -1,
		pos_, Ppos);

	if (hit.HitFlag)
	{
		// �R���W�����p�|���S�������݂��Ȃ����W�܂ŃJ�������W�𒍎��_�ɋ߂Â���

		// �����_���W����J�������W�ւ̃x�N�g�����Z�o
		VECTOR Direction = VNorm(VSub(pos_, Ppos));

		// �|���S���ɓ�����Ȃ��������Z�b�g
		float NotHitDistance = Utility::FLOAT_ZERO;

		// �|���S���ɓ����鋗�����Z�b�g

		VECTOR TestPosition = pos_;

		float HitDistance = LEN_CAMERACOLLISION_LINE;
		while (HitDistance - NotHitDistance > MIN_LEN_CAMERACOLLISION_LINE)
		{
			// �����邩�ǂ����e�X�g���鋗�����Z�o( ������Ȃ������Ɠ����鋗���̒��� )
			collitionLineStageCamera_ = NotHitDistance + (HitDistance - NotHitDistance) / 2.0f;

			// �e�X�g�p�̃J�������W���Z�o
			TestPosition =
				VAdd(Ppos, VScale(Direction, collitionLineStageCamera_));

			hit = MV1CollCheck_Line(
				stagemodelId_, -1,
				TestPosition, Ppos);

			// �V�������W�ŃR���W�����p�|���S���ɓ����邩�e�X�g
			if (hit.HitFlag)
			{
				// �����鋗���� collitionLineStageCamera_ �ɕύX����
				HitDistance = collitionLineStageCamera_;
			}
			else
			{
				// ������Ȃ������瓖����Ȃ������� collitionLineStageCamera_ �ɕύX����
				NotHitDistance = collitionLineStageCamera_;
			}

			// HitDistance �� NoHitDistance ���\���ɋ߂Â��Ă��Ȃ������烋�[�v
		}

		// �J�����̍��W���Z�b�g
		pos_ = TestPosition;
	}

}

bool Camera::HitCheck(VECTOR campos, VECTOR Ppos, float size)
{
	// �X�e�[�W�p�R���W�����R�c���f���ɓ����邩���`�F�b�N
	auto HitDim = MV1CollCheck_Capsule(stagemodelId_, -1, campos, Ppos, size);

	auto HitNum = HitDim.HitNum;
	MV1CollResultPolyDimTerminate(HitDim);

	// �������Ă����炱���ŏI��
	if (HitNum > NUM_NOTHIT_POLY)
	{
		return true;
	}

	return false;

}

void Camera::SetHitModel(const int modelId)
{
	stagemodelId_ = modelId;
}