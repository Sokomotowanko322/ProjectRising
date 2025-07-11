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
		ATTACK,
		FLINCH,
		BLOW,
		BLOW_AWAY,
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		//RUN,
		ATTACK,
		FLINCH,
		BLOW,
		BLOW_AWAY,
		ALL
	};

	//アニメーションKEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		/*"RUN",*/
		"ATTACK",
		"FLINCH",
		"BLOW",
		"BLOW_AWAY",
	};

	// コンストラクタ
	NormalEnemy(std::weak_ptr<Player> player);
	~NormalEnemy();
	
	// 初期化
	void Init(void);
	
	// 更新
	void Update(void) override;
	
	// 描画
	void Draw(void) override;

	// ダメージ
	void Damage(int damageAmount) override;

	// 状態遷移
	void ChangeState(STATE state);

	// 位置の取得
	VECTOR GetPos() const override;
	void SetPos(const VECTOR& pos)override;

protected:


	
private:

	// アニメーションコントローラ
	std::unique_ptr<AnimationController> animationController_;
	
	// アニメーションの初期化
	void InitAnimation(void);

	// アニメーション移動地無効化
	void DisableAnimMovePow(void);

	// アニメーション遷移用
	STATE state_;
	STATE preState_;

	// STATE内に格納するキー
	std::string animationKey_;
	std::string preAnimationKey_;

	// 直近の状態取得
	std::vector<std::string> currentState_;

	// STATEの変更、関数内で同時にUPDATEとアニメーションを呼び出す
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeClose(void);
	void ChangeAttack(void);
	void ChangeFlinch(void);
	void ChangeBlow(void);
	void ChangeBlowAway(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateClose(void);
	void UpdateAttack(void);
	void UpdateFlinch(void);
	void UpdateBlow(void);
	void UpdateBlowAway(void);

	// 回転の差分
	VECTOR diff_;
	
	// クォータニオン回転
	Quaternion rot_;

	// ブレンド用フレーム番号
	int frameNo_;

	// 回転の内積
	float dot_;
	
	// 回転のステップ
	float rotationStep_;
};

