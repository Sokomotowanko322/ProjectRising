#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		RESULT_RANK_B,
		RESULT_RANK_A,
		RESULT_RANK_S,
		CHOISE_YES,
		CHOISE_NO,
		HIT_IMAGE,
		HIT_NUMIMAGE,
		TITLE_IMAGE,
		TITLE_IMAGE_LIGHT,
		START_LOGO,
		HP_FRAME,
		RESULT,
		PUSH_SPACE,
		SPEECH_BALLOON,
		PLAYER,
		PLAYER_IDLE,
		PLAYER_WALK,
		PLAYER_RUN,
		PLAYER_DASH,
		PLAYER_HIGH_TIME,
		PLAYER_SMASH_ATTACK,
		PLAYER_DODGE,
		PLAYER_HASARM,
		PLAYER_FIRSTCOMBO,
		PLAYER_SECONDCOMBO,
		PLAYER_THIRDCOMBO,
		PLAYER_COUNTER,
		PLAYER_INHALE,
		PLAYER_SHADOW,
		PLAYER_WEAPON,
		PLAYER_WEAPON_SWORDLINE,
		SKY_DOME,
		STAGE,
		STAGE_COLLIDER,
		FOOT_SMOKE,
		CLEAR,
		NORMAL_ENEMY,
		NORMAL_ENEMY_IDLE,
		NORMAL_ENEMY_WALK,
		NORMAL_ENEMY_NORMALATTACK,
		NORMAL_ENEMY_DEATH,
		ENEMY_BOSS_TYPE1,
		
#pragma region エフェクト

		EFFECT_TITLE_LIGHTNING,
		EFFECT_INHALE,
		EFFECT_HIT,

#pragma endregion

#pragma region サウンド

		SOUND_TITLEBGM,
		SOUND_ENTER,

#pragma endregion

#pragma region シェーダー
		
		LIGHTNING_SHADER

#pragma endregion
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
