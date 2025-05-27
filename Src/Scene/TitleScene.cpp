#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Unit/Player.h"
#include "../Object/Weapon.h"
#include "TitleScene.h"

const int DRAW_LOGO_POS_X = 30;
const int DRAW_LOGO_POS_Y = 50;
const VECTOR DRAW_LOGO_EFFECT_POS = { -150.0f,100.0f,0.0f };
const VECTOR EFFECT_SCALE = { 50.0f,50.0f,0.0f };

TitleScene::TitleScene(void)
	: titleLogo_(-1)
{
}

TitleScene::~TitleScene(void)
{
	// タイトルロゴの解放
	if (titleLogo_ != -1 && titleLogoLight_ != -1)
	{
		DeleteGraph(titleLogo_);
		DeleteGraph(titleLogoLight_);
		titleLogo_ = -1;
		titleLogoLight_ = -1;
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
	SceneManager::GetInstance().GetWeapon()->~Weapon();

	// エフェクト解放
	DeleteEffekseerEffect(titleLightningResId_);
	
}

void TitleScene::Init(void)
{
	// それぞれ専用のスクリーンを作成
	effectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	objectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 武器の初期化
	SceneManager::GetInstance().GetWeapon()->Init();

	// タイトルロゴの初期化
	titleLogo_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE).handleId_;
	titleLogoLight_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE_LIGHT).handleId_;

	// タイトルロゴのエフェクト
	titleLightningResId_ = resMng_.Load(ResourceManager::SRC::EFFECT_TITLE_LIGHTNING).handleId_;
	SetPosPlayingEffekseer3DEffect(titleLightningPlayId_, DRAW_LOGO_EFFECT_POS.x, DRAW_LOGO_EFFECT_POS.y, DRAW_LOGO_EFFECT_POS.z);
	SetScalePlayingEffekseer3DEffect(titleLightningPlayId_, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);
	SetRotationPlayingEffekseer3DEffect(titleLightningPlayId_, 0.0f, 0.0f, 0.0f);
	
	// 点滅初期化
	logoLightTimer_ = 0.0f;
	logoLightInterval_ = 1.0f + (rand() % 1000) / 1000.0f * 1.5f; // 1.0～2.5秒の間隔
	isLogoLightVisible_ = false;
}

void TitleScene::Update(void)
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
		/*PlaySoundMem(enterSe_, DX_PLAYTYPE_BACK, true);
		count_ = BLINK_RATE;*/
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{
	// エフェクト用スクリーン
	SetDrawScreen(effectScr_);
	ClearDrawScreen();
	
	// エフェクトの描画
	if (isLogoLightVisible_)
	{
		SetPosPlayingEffekseer3DEffect(titleLightningPlayId_, DRAW_LOGO_EFFECT_POS.x, DRAW_LOGO_EFFECT_POS.y, DRAW_LOGO_EFFECT_POS.z);
		SetScalePlayingEffekseer3DEffect(titleLightningPlayId_, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);
		SetRotationPlayingEffekseer3DEffect(titleLightningPlayId_, 0.0f, 0.0f, 0.0f);
		titleLightningPlayId_ = PlayEffekseer3DEffect(titleLightningResId_);
	}
	else
	{
		StopEffekseer3DEffect(titleLightningPlayId_);
	}

	// オブジェクト用スクリーン
	SetDrawScreen(objectScr_);
	ClearDrawScreen();

	// ロゴの描画
	DrawGraph(DRAW_LOGO_POS_X, DRAW_LOGO_POS_Y, titleLogo_, true);

	// 武器の描画
	SceneManager::GetInstance().GetWeapon()->Draw();

	// 点滅
	if (isLogoLightVisible_)
	{
		// 透過しておく
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); 
		DrawGraph(DRAW_LOGO_POS_X, DRAW_LOGO_POS_Y, titleLogoLight_, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	// まずオブジェクト用スクリーンを描画
	DrawGraph(0, 0, objectScr_, true);

	// エフェクト用スクリーンを合成
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawGraph(0, 0, effectScr_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 背景をいったん青にする
	SetBackgroundColor(0, 0, 100);
}