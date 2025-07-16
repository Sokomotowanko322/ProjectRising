#include <memory>
#include "SceneBase.h"
#include "../Object/Transform.h"

class Weapon;

class ResultScene : public SceneBase
{
public:
	// コンストラクタ&デストラクタ
	ResultScene(void);
	~ResultScene(void);

	// 初期化
	void Init(void)override;

	// 描画
	void Draw(void)override;

	// 更新
	void Update(void)override;

protected:
private:
	VECTOR screenEffectPos_;

	// リザルト用ロゴなど
	int resultLogo_;
	int resultLogoLight_;

	// リザルトのエフェクト
	int resultLightningResId_;
	int resultLightningPlayId_;

	// エフェクトをかける用のスクリーン
	int effectScr_;

	// オブジェクト描画用のスクリーン
	int objectScr_;

	// 点滅制御用
	float logoLightTimer_;
	float logoLightInterval_;
	bool  isLogoLightVisible_;
};