#pragma once
class SceneManager;
class Camera;
class Application;
//class Collision;
class ResourceManager;
class ColliderManager;
class Weapon;

class SceneBase
{

public:

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

protected:

	//カメラ
	Camera& camera_;

	//衝突判定
	//Collision& collision_;

	// リソース管理
	ResourceManager& resMng_;

	// シーン管理
	SceneManager& scnMng_;

	// シーン管理
	//ColliderManager& colMng_;
};