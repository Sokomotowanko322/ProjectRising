#include <memory>
#include "SceneBase.h"

class ColliderManager;
class SkyDome;
class Player;
class NormalEnemy;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	// 初期化
	void Init(void) override;
	void InitCollider(void);

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

protected:

private:
	
	// プレイヤー
	std::shared_ptr<Player> player_;
	
	// 通常の敵
	std::shared_ptr<NormalEnemy> normalEnemy_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;
	
	//// 敵
	//std::shared_ptr<Enemy> enemy_;
	//// ステージ
	//std::shared_ptr<Stage> stage_;
	// 衝突判定
	std::unique_ptr<ColliderManager> colMng_;

};

