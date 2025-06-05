#include "../../Application.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Controller/AnimationController.h"
#include "../../Utility/Utility.h"
#include "../../Object/Weapon.h"
#include "Player.h"

// モデルのHips
char FRAME_HIPS[] = "mixamorig:Hips";

const VECTOR INIT_POS = {0.0f,0.0f,0.0f};
const VECTOR MODEL_SCALE = {1.0f,1.0f,1.0f};

// 回転完了までの時間
static constexpr float TIME_ROT = 1.0f;

// スピード
static constexpr float SPEED_MOVE = 3.0f;
static constexpr float RUN_SPEED_POW = 3.0f;

// 名前
const int LEGS_ANIM = 0;
const int ARM_ANIM = 1;

// コントローラー制御
const float	STICK_VALUE_MAX = 1000.0f;
const float	REVERSE_VALUE = -1.0f;
const float	TIMEROT_NORMAL = 0.5f;
const float	TIMEROT_FAST = 0.1f;
const float INPUT_RECEPTION_TIME = 1.4f;

// パラメーター
const int MAXHP = 100;
int HP = MAXHP;
const float NORMAL_ANIM_SPEED = 60.0f;
const float FAST_ANIM_SPEED = 80.0f;

Player::Player() : ActorBase(),
weapon_(std::make_shared<Weapon>()),
animationController_(nullptr),
moveForwardCount_(0),
moveDir_(Utility::VECTOR_ZERO),
movePow_(Utility::VECTOR_ZERO),
stepRotTime_(0.0f),
moveSpeed_(0.0f),
goalQuaRot_(Quaternion()),
playerRotY_(Quaternion()),
isAttack_(false),
readyHighTime_(false),
preForwardPressed_(false),
preBackPressed_(false),
armAnimId_(-1),
frameNo_(-1),
legsAnimId_(-1), 
rightHandFrame_(-1),
rightHandPos_(Utility::VECTOR_ZERO),
rotRad_(0.0f)
{
  
}

Player::~Player()
{
	// モデルの解放
	if (transform_.modelId != -1)
	{
		MV1DeleteModel(transform_.modelId);
	}
}

void Player::Init(void)  
{  
	// モデル情報の初期化
   transform_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER);  
   transform_.pos = INIT_POS;
   transform_.scl = MODEL_SCALE;
   transform_.quaRot = Quaternion();
   transform_.quaRotLocal =
	   Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });

   // アニメーションの初期化
   InitAnimation();

   // 武器の初期化
   weapon_->Init();

   // 武器追従のためフレームを取得
   rightHandFrame_ = MV1SearchFrame(transform_.modelId, "mixamorig:RightHandMiddle1");

   // 移動地無効化用
   frameNo_ = MV1SearchFrame(transform_.modelId, FRAME_HIPS);

   // ブレンド用アニメーションのアタッチ
   legsAnimId_ = MV1AttachAnim(transform_.modelId, (int)ANIM_TYPE::DASH, -1, false);
   armAnimId_ = MV1AttachAnim(transform_.modelId, (int)ANIM_TYPE::HASWEAPON, -1, false);

   // 立っているアニメーションのブレンド率をセット
   MV1SetAttachAnimBlendRate(transform_.modelId, legsAnimId_, 1.0f);

   // 手を前に出しているアニメーションのブレンド率をセット
   MV1SetAttachAnimBlendRate(transform_.modelId, armAnimId_, 0.0f);
}

void Player::Update(void)
{
	// プレイヤーの更新
	transform_.Update();

	// アニメーションの更新
	animationController_->Update();
	
	// 左手のフレーム位置を取得し続ける
	rightHandPos_ = MV1GetFramePosition(transform_.modelId, rightHandFrame_);

    // 移動量の初期化
    moveDir_ = Utility::VECTOR_ZERO;
    movePow_ = Utility::VECTOR_ZERO;

	// プレイヤーの移動制御
	ProcessInput();
	
    // 回転させる
    transform_.quaRot = playerRotY_;	

	// 武器の位置をプレイヤーの右手に設定
	weapon_->GameUpdate(transform_);

}

void Player::Draw(void)
{
	// プレイヤーの描画
	MV1DrawModel(transform_.modelId);
	
	// 武器の描画
	weapon_->Draw();
}

void Player::ProcessInput(void)
{
	auto& ins = InputManager::GetInstance();

	if (!isAttack_)
	{
		weapon_->StopEffect();
	}
	// 攻撃アニメーション中は他の入力を受け付けないようにする
	if (isAttack_)
	{
		weapon_->StartEffect();
		// アニメーションが終わったか判定
		if (animationController_->IsEndBlendingPlayAnimation("SMASH") ||
			animationController_->IsEndBlendingPlayAnimation("HIGHTIME")||
			animationController_->IsEndBlendingPlayAnimation("FIRST_COMBO"))
		{
			isAttack_ = false;
		}
		else
		{
			// アニメーション終了まで遷移できなくする
			return;
		}
	}

	// 前後方向入力の検出
	bool nowForwardPressed = ins.IsPressed(InputManager::ACTION::MOVE_FORWARD);
	bool nowBackPressed = ins.IsPressed(InputManager::ACTION::MOVE_BACK);

	// 後方入力の立ち上がり検出
	if (nowBackPressed && !preBackPressed_) 
	{
		// カウント開始
		backInputFrame_ = 0;
	}
	preBackPressed_ = nowBackPressed;

	// カウント中ならフレームを進める
	if (backInputFrame_ >= 0) 
	{
		backInputFrame_++;
		// フレームを超えたらリセット
		if (backInputFrame_ > 15)
		{
			backInputFrame_ = -1;
		}
	}

	// 受付フレーム以内に同時押しでSMASH発動
	if (backInputFrame_ > 0 && backInputFrame_ <= 10) 
	{
		if (ins.IsPressed(InputManager::ACTION::MOVE_FORWARD) &&
			ins.IsPressed(InputManager::ACTION::ATTACK))
		{
			rotRad_ = static_cast<float>(Utility::Deg2RadD(0.0f));
			animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::SMASH]);
			isAttack_ = true;
			backInputFrame_ = -1;
			return;
		}
	}

	// 2回目の前方向入力中にATTACKが押されたらHIGHTIME発動
	if (nowForwardPressed && !preForwardPressed_)
	{
		moveForwardCount_++;
		// 2回目の立ち上がりでHIGHTIME発動の準備状態に
		if (moveForwardCount_ == 2)
		{
			readyHighTime_ = true;
		}
	}
	preForwardPressed_ = nowForwardPressed;

	if (readyHighTime_ && nowForwardPressed && ins.IsPressed(InputManager::ACTION::ATTACK))
	{
		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::HIGHTIME]);
		isAttack_ = true;
		moveForwardCount_ = 0;
		readyHighTime_ = false;
		return;
	}

	// MOVE_FORWARDが離されたら準備状態解除
	if (!nowForwardPressed && readyHighTime_)
	{
		moveForwardCount_ = 0;
		readyHighTime_ = false;
	}

	// 移動入力の制御
	MoveControl();
	if (ins.IsPressed(InputManager::ACTION::RUN))
	{
		movePow_ = VScale(moveDir_, SPEED_MOVE * RUN_SPEED_POW);
		SetGoalRotate(rotRad_);
		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::DASH]);
		//BlendAnimation();
		transform_.pos = VAdd(transform_.pos, movePow_);
	}
	else if (!Utility::EqualsVZero(moveDir_))
	{
		movePow_ = VScale(moveDir_, SPEED_MOVE);
		SetGoalRotate(rotRad_);
		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::WALK]);
		transform_.pos = VAdd(transform_.pos, movePow_);
	}
	else
	{
		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::IDLE]);
	}

	// 攻撃
	if (ins.IsTriggered(InputManager::ACTION::ATTACK))
	{
		animationController_->ChangeAnimation(ANIM_DATA_KEY[(int)ANIM_TYPE::FIRST_COMBO]);
		isAttack_ = true;
		return;
	}

}

void Player::MoveControl(void)
{
	rotRad_ = 0.0f;
	auto& ins = InputManager::GetInstance();

	auto pad = ins.GetJPadState(JOYPAD_NO::PAD1);
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetQuaRotOutX();

	Rotate();

	if (ins.IsPressed(InputManager::ACTION::MOVE_FORWARD))
	{
		moveDir_ = cameraRot.GetForward();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(0.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_BACK))
	{
		moveDir_ = cameraRot.GetBack();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(180.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_LEFT))
	{
		moveDir_ = cameraRot.GetLeft();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(270.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_RIGHT))
	{
		moveDir_ = cameraRot.GetRight();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(90.0f));
	}
	if (pad.AkeyL.x != 0 || pad.AkeyL.y != 0)
	{
		moveDir_ = { static_cast<float>(pad.AkeyL.x) / STICK_VALUE_MAX, 0.0f ,
			static_cast<float>(pad.AkeyL.y) / STICK_VALUE_MAX * REVERSE_VALUE };

		rotRad_ = atan2f(moveDir_.x, moveDir_.z);

		VECTOR CameraAngle = SceneManager::GetInstance().GetCamera().GetAngles();

		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(CameraAngle.y));

		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(moveDir_, mat);

		stepRotTime_ = TIMEROT_FAST;
	}
	else
	{
		stepRotTime_ = TIMEROT_NORMAL;
	}
}

const VECTOR& Player::GetPos() const
{
	return transform_.pos;
}

const VECTOR& Player::GetRightHandPos() const
{
	return rightHandPos_;
}

void Player::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/Anim/";
	// アニメーションコントローラーの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// IDLE状態
	animationController_->Add("IDLE", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_IDLE), true, 0, false);
    animationController_->Add("WALK", path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_WALK), true, 0, false);
	animationController_->Add("RUN", path + "Run.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);
	animationController_->Add("DASH", path + "Dash.mv1",
		0.0f, FAST_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_DASH), true, 0, false);
	
	animationController_->Add("HIGHTIME", path + "HighTime.mv1",
		0.0f, FAST_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_HIGH_TIME), false, 0, false);
	animationController_->Add("SMASH", path + "SmashAttack.mv1",
		0.0f, FAST_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_SMASH_ATTACK), false, 0, false);
	animationController_->Add("FIRST_COMBO", path + "Combo1.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIRSTCOMBO), false, 0, false);
	animationController_->Add("DODGE", path + "Dodge.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_DODGE), false, 0, false);
	animationController_->Add("HASWEAPON", path + "HasWeapon.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_HASARM), true, 0, false);
}

void Player::BlendAnimation(void)
{
	float stepDashAnim = animationController_->GetAnimData("DASH").stepAnim;
	float stepArmAnim = animationController_->GetAnimData("HASWEAPON").stepAnim;

	// アニメーション進行
	MV1SetAttachAnimTime(transform_.modelId, legsAnimId_, 1.0f);
	MV1SetAttachAnimTime(transform_.modelId, armAnimId_, 1.0f);

	// 上半身だけ動く
	// ------------------------------------------------------------------------
	// 両方ROOT以下ノーブレンド
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 3, 0.0f, true);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 0.0f, true);
	
	// ROOTと背骨0は、IDLE 100%
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 3, 1.0f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 1.0f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 4, 1.0f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 1.0f, false);
	
	// 上半身
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 1.0f, true);
	
	// 下半身
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 42, 1.0f, true);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 47, 1.0f, true);

	// 下半身だけ動く
	// ------------------------------------------------------------------------
	// 両方ROOT以下ノーブレンド
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 3, 0.0f, true);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 3, 0.0f, true);
	// 走る 50%、腕50%
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 3, 0.5f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 0.5f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 4, 0.5f, false);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 0.5f, false);
	// 上半身
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, legsAnimId_, 66, 1.0f, true);
	// 下半身(ダンス)
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 1.0f, true);
	MV1SetAttachAnimBlendRateToFrame(transform_.modelId, armAnimId_, 33, 1.0f, true);
	// ------------------------------------------------------------------------
}

void Player::DisableAnimMovePow(void)
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

void Player::SetGoalRotate(double rotRad)
{
    VECTOR cameraRot = mainCamera.GetAngles();
    Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, Utility::AXIS_Y);

    // 現在設定されている回転との角度差を取る
    double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

    // しきい値
    if (angleDiff > 0.1)
    {
        stepRotTime_ = TIME_ROT;
    }

    goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
    stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

    // 回転の球面補間
    playerRotY_ = Quaternion::Slerp(
        playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}