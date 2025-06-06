#pragma once
#include<vector>
#include "../Object/Transform.h"
#include "../Object/Unit/ActorBase.h"

enum class EFFECT
{
	FIRE,
	STORM,
	LIGHTNING,
	NONE,
};

class Weapon : public ActorBase
{
public:

	// コンストラクタ
	Weapon(void);

	// デストラクタ
	~Weapon(void);

	// 初期化
	void Init(void);

	// 更新(タイトル用)
	void TitleUpdate(void);
	void Update()override;

	// 更新(ゲームシーン用)
	void GameUpdate(Transform follow);

	// 描画
	void Draw(void);
	void SetPos(const VECTOR& pos)override;
	
	// デバッグ表示
	void DrawDebug(void);

	const int GetIsEffect(void) const;

	const VECTOR GetTopPos(void) const;
	VECTOR GetPos(void) const;

	void SetEffect(int effect);

	void Fade(void);

	void SetIsHit(bool ishit);

	// エフェクトの位置を同期
	void SyncEffect(void);

	// エフェクトの再生/停止
	void StartEffect(void);
	void StopEffect(void);
	
protected:

	// 武器の位置や向き
	Quaternion titleSceneQuarot_;
	Quaternion handLocalquarot_;
	VECTOR handRPos_;

	// 武器の長さや頂点位置
	VECTOR swordLength_;
	VECTOR swordTopPos_;

	// エフェクトの座標
	VECTOR effectPos_;
	VECTOR effectScale_;

	// エフェクトのID
	int effectLineResId_;
	int effectLinePlayId_;

	// 手のフレーム
	int handAttachFrame_;

	// エフェクト制御用フラグ
	bool isEffect_;

private:

	// リソースの参照
	ResourceManager& resMng_;

	// Transform
	Transform weaponTransform_;
	
};