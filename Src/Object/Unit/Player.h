#include "ActorBase.h"

class AnimationController;
class Weapon; 
class NormalEnemy; 

class Player : public ActorBase
{
public:

	enum class STATE
	{
		IDLE,
		MOVE,
		DODGE,
		FIRST_COMBO,
		SMASH,
		HIGHTIME,
		DAMAGE,
		DEATH,
	};

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
	void ProcessDodge(void);
	void MoveControl(void);

	// 取得系
	std::shared_ptr<Weapon> GetWeapon() const;
	ANIM_TYPE GetCurrentAnimType() const;
	const VECTOR& GetRightHandPos() const;
	VECTOR GetPos() const override;
	VECTOR GetCenterPos() const;

	// ヒット時のエフェクト
	void HitEffect(VECTOR pos);

	// ヒットストップ
	void HitStop(float time);

	// プレイヤーの位置をコライダと合わせる
	void SetPos(const VECTOR& pos)override;

	// スローモーション処理
	void StartSlow(float time,float speed);

	// 攻撃状態かどうか
	bool IsAttack() const;

	// 状態遷移
	void ChangeState(STATE state);

	// コライダ側で代入できるようにする
	bool isGrounded_;

private:

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// 衝突判定
	std::unique_ptr<ColliderManager> colMng_;

	// 武器(主にプレイヤー用として機能させる)
	std::shared_ptr<Weapon> weapon_;
	
	// 敵
	std::weak_ptr<NormalEnemy> normalEnemy_;

	// アニメーション遷移用
	STATE state_;
	STATE preState_;

	// STATE内に格納するキー
	std::string animationKey_;
	std::string preAnimationKey_;

	// アニメーションタイプ
	ANIM_TYPE currentAnimType_;

	// 右手の座標
	VECTOR rightHandPos_;

	// モデル衝突判定用の腰の位置
	VECTOR waistPos_;

	// 移動方向
	VECTOR moveDir_;

	// プレイヤーの移動量
	VECTOR movePow_;

	// 腰まわりのフレーム
	int waistFrame_;

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

	// エフェクトの再生用
	int hitEfResId_;
	int hitEfPlayId_;

	// ヒットストップ
	float hitStopTimer_;

	// スローモーション用
	float slowTimer_;
	bool isSlow_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float rotRad_;
	float stepRotTime_;

	// フラグ管理
	bool isAttack_;
	bool isInvincible_;
	bool isHitStop_;
	
	// アニメーションの初期化
	void InitAnimation(void);
	
	// ブレンドテスト
	void BlendAnimation(void);

	// アニメーション移動地無効化
	void DisableAnimMovePow(void);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// STATEの変更、関数内で同時にUPDATEとアニメーションを呼び出す
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeMove(void);
	void ChangeFirstAttack(void);
	void ChangeSmash(void);
	void ChangeHightime(void);
	void ChangeDodge(void);
	void ChangeDamage(void);
	void ChangeDeath(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateMove(void);
	void UpdateFirstAttack(void);
	void UpdateSmash(void);
	void UpdateHightime(void);
	void UpdateDodge(void);
	void UpdateDamage(void);
	void UpdateDeath(void);
};