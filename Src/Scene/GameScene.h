#include <memory>
#include "SceneBase.h"

class ColliderManager;
class SkyDome;
class Stage;
class Player;
class MidBoss;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	// 参照切れ防止用
	std::shared_ptr<Player> GetPlayer() const 
	{ 
		return player_; 
	}

	// 初期化
	void Init(void) override;

	// コライダの追加
	void AddColliders(void);

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

protected:

private:
	
	// プレイヤー
	std::shared_ptr<Player> player_;
	
	// 通常の敵
	std::shared_ptr<MidBoss> midBoss_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;
	
	// ステージ
	std::shared_ptr<Stage> stage_;

	// 衝突判定
	std::unique_ptr<ColliderManager> colMng_;

	// ゲーム内BGM
	int gameBgm_;

	// ゲーム内時間
	float gameTime_;

	// クリア判定
	bool isClear_;
};

