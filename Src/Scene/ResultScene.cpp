#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Unit/Player.h"
#include "../Object/Weapon.h"
#include "ResultScene.h"

const int DRAW_RESULT_POS_X = 30;
const int DRAW_RESULT_POS_Y = 50;
const VECTOR DRAW_RESULT_EFFECT_POS = { -150.0f,100.0f,0.0f };
const VECTOR EFFECT_SCALE = { 50.0f,50.0f,0.0f };

ResultScene::ResultScene(void)
	: resultLogo_(-1)
{
}

ResultScene::~ResultScene(void)
{
	// ロゴの解放
	if (resultLogo_ != -1 && resultLogoLight_ != -1)
	{
		DeleteGraph(resultLogo_);
		DeleteGraph(resultLogoLight_);
		resultLogo_ = -1;
		resultLogoLight_ = -1;
	}

	// スクリーンの解放
	if (effectScr_ != -1 && objectScr_ != -1)
	{
		DeleteGraph(effectScr_);
		DeleteGraph(objectScr_);
		effectScr_ = -1;
		objectScr_ = -1;
	}

	// モデルの開放
	//SceneManager::GetInstance().GetWeapon()->~Weapon();

	// エフェクト解放
	DeleteEffekseerEffect(resultLightningResId_);

}

void ResultScene::Init(void)
{
	// それぞれ専用のスクリーンを作成
	effectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	objectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 武器の初期化
	SceneManager::GetInstance().GetWeapon()->Init();

	// ロゴの初期化
	resultLogo_ = resMng_.Load(ResourceManager::SRC:: TITLE_IMAGE).handleId_;
	resultLogoLight_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE_LIGHT).handleId_;

	// ロゴのエフェクト
	resultLightningResId_ = resMng_.Load(ResourceManager::SRC::EFFECT_TITLE_LIGHTNING).handleId_;
	SetPosPlayingEffekseer3DEffect(resultLightningPlayId_, DRAW_RESULT_EFFECT_POS.x, DRAW_RESULT_EFFECT_POS.y, DRAW_RESULT_EFFECT_POS.z);
	SetScalePlayingEffekseer3DEffect(resultLightningPlayId_, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);
	SetRotationPlayingEffekseer3DEffect(resultLightningPlayId_, 0.0f, 0.0f, 0.0f);

	// 点滅初期化
	logoLightTimer_ = 0.0f;
	logoLightInterval_ = 1.0f + (rand() % 1000) / 1000.0f * 1.5f; // 1.0～2.5秒の間隔
	isLogoLightVisible_ = false;
}

void ResultScene::Update(void)
{
	// Weaponの更新
	SceneManager::GetInstance().GetWeapon()->TitleUpdate();

	// ロゴ点滅制御
	float delta = SceneManager::GetInstance().GetDeltaTime();
	logoLightTimer_ += delta;

	// フラグがオフの時は非表示
	if (!isLogoLightVisible_)
	{
		if (logoLightTimer_ >= logoLightInterval_)
		{
			isLogoLightVisible_ = true;
			logoLightTimer_ = 0.0f;

			// 点滅時間は短く
			logoLightInterval_ = 0.08f + (rand() % 1000) / 1000.0f * 0.07f;
		}
	}
	else
	{
		if (logoLightTimer_ >= logoLightInterval_)
		{
			isLogoLightVisible_ = false;
			logoLightTimer_ = 0.0f;

			// 次の点滅までの間隔を再設定
			logoLightInterval_ = 1.0f + (rand() % 1000) / 1000.0f * 1.5f;
		}
	}

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SELECT_ENTER))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	// エフェクト用スクリーン
	SetDrawScreen(effectScr_);
	ClearDrawScreen();

	// エフェクトの描画
	if (isLogoLightVisible_)
	{
		SetPosPlayingEffekseer3DEffect(resultLightningPlayId_, DRAW_RESULT_EFFECT_POS.x, DRAW_RESULT_EFFECT_POS.y, DRAW_RESULT_EFFECT_POS.z);
		SetScalePlayingEffekseer3DEffect(resultLightningPlayId_, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);
		SetRotationPlayingEffekseer3DEffect(resultLightningPlayId_, 0.0f, 0.0f, 0.0f);
		resultLightningPlayId_ = PlayEffekseer3DEffect(resultLightningResId_);
	}
	else
	{
		StopEffekseer3DEffect(resultLightningPlayId_);
	}

	// オブジェクト用スクリーン
	SetDrawScreen(objectScr_);
	ClearDrawScreen();

	// ロゴの描画
	DrawGraph(DRAW_RESULT_POS_X, DRAW_RESULT_POS_Y, resultLogo_, true);

	// 
}