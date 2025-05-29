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

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		//RUN,
		ATTACK,
		ALL
	};

	//アニメーションKEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		/*"RUN",*/
		"ATTACK",
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

	// 状態遷移
	void ChangeState(STATE state);

protected:


	
private:

	// アニメーションコントローラ
	std::unique_ptr<AnimationController> animationController_;
	
	// アニメーションの初期化
	void InitAnimation(void);

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
	void ChangeWalk(void);
	void ChangeAttack(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);

	// 回転の差分
	VECTOR diff_;
	
	// クォータニオン回転
	Quaternion rot_;

	// 回転の内積
	float dot_;
	
	// 回転のステップ
	float rotationStep_;
};

