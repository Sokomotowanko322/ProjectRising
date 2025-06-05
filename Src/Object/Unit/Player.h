#include "ActorBase.h"

class AnimationController;
class Weapon; 

class Player : public ActorBase//, public ColliderBase
{
public:

	// アニメーション種別
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

	//アニメーションKEY
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


	// コンストラクタ
	Player();
	~Player() override;

	// 初期化
	void Init(void);

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 操作
	void ProcessInput(void);
	void MoveControl(void);
	void SetPos(const VECTOR& pos);

	// 取得系
	VECTOR& GetPos() const override;
	const VECTOR& GetRightHandPos() const;
	const VECTOR& GetAngles() const;

private:

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// 武器(主にプレイヤー用として機能させる)
	std::shared_ptr<Weapon> weapon_;

	// 右手の座標
	VECTOR rightHandPos_;

	// 移動方向
	VECTOR moveDir_;

	// プレイヤーの移動量
	VECTOR movePow_;

	// 右手のフレーム
	int rightHandFrame_;

	// ブレンド用フレーム番号
	int frameNo_;

	// 入力制御用
	// -1だったら未入力
	int moveForwardCount_;
	int backInputFrame_ = -1;
	bool preBackPressed_;
	bool preForwardPressed_;
	bool readyHighTime_;

	// アニメーションを保持
	int armAnimId_;
	int legsAnimId_;

	// プレイヤーの移動速度
	float moveSpeed_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float rotRad_;
	float stepRotTime_;

	// フラグ管理
	bool isAttack_;

	// アニメーションの初期化
	void InitAnimation(void);
	
	// ブレンドテスト
	void BlendAnimation(void);

	// アニメーション移動地無効化
	void DisableAnimMovePow(void);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);
};