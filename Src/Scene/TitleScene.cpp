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
	// �^�C�g�����S�̉��
	if (titleLogo_ != -1 && titleLogoLight_ != -1)
	{
		DeleteGraph(titleLogo_);
		DeleteGraph(titleLogoLight_);
		titleLogo_ = -1;
		titleLogoLight_ = -1;
	}
	
	// �X�N���[���̉��
	if (effectScr_ != -1 && objectScr_ != -1)
	{
		DeleteGraph(effectScr_);
		DeleteGraph(objectScr_);
		effectScr_ = -1;
		objectScr_ = -1;
	}

	// ���f���̊J��
	SceneManager::GetInstance().GetWeapon()->~Weapon();

	// �G�t�F�N�g���
	DeleteEffekseerEffect(titleLightningResId_);
	
}

void TitleScene::Init(void)
{
	// ���ꂼ���p�̃X�N���[�����쐬
	effectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	objectScr_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// ����̏�����
	SceneManager::GetInstance().GetWeapon()->Init();

	// �^�C�g�����S�̏�����
	titleLogo_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE).handleId_;
	titleLogoLight_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE_LIGHT).handleId_;

	// �^�C�g�����S�̃G�t�F�N�g
	titleLightningResId_ = resMng_.Load(ResourceManager::SRC::EFFECT_TITLE_LIGHTNING).handleId_;
	SetPosPlayingEffekseer3DEffect(titleLightningPlayId_, DRAW_LOGO_EFFECT_POS.x, DRAW_LOGO_EFFECT_POS.y, DRAW_LOGO_EFFECT_POS.z);
	SetScalePlayingEffekseer3DEffect(titleLightningPlayId_, EFFECT_SCALE.x, EFFECT_SCALE.y, EFFECT_SCALE.z);
	SetRotationPlayingEffekseer3DEffect(titleLightningPlayId_, 0.0f, 0.0f, 0.0f);
	
	// �_�ŏ�����
	logoLightTimer_ = 0.0f;
	logoLightInterval_ = 1.0f + (rand() % 1000) / 1000.0f * 1.5f; // 1.0�`2.5�b�̊Ԋu
	isLogoLightVisible_ = false;
}

void TitleScene::Update(void)
{
	// Weapon�̍X�V
	SceneManager::GetInstance().GetWeapon()->TitleUpdate();
	
	// ���S�_�Ő���
	float delta = SceneManager::GetInstance().GetDeltaTime();
	logoLightTimer_ += delta;

	// �t���O���I�t�̎��͔�\��
	if (!isLogoLightVisible_) 
	{
		if (logoLightTimer_ >= logoLightInterval_) 
		{
			isLogoLightVisible_ = true;
			logoLightTimer_ = 0.0f;

			// �_�Ŏ��Ԃ͒Z��
			logoLightInterval_ = 0.08f + (rand() % 1000) / 1000.0f * 0.07f;
		}
	}
	else 
	{
		if (logoLightTimer_ >= logoLightInterval_) 
		{
			isLogoLightVisible_ = false;
			logoLightTimer_ = 0.0f;

			// ���̓_�ł܂ł̊Ԋu���Đݒ�
			logoLightInterval_ = 1.0f + (rand() % 1000) / 1000.0f * 1.5f;
		}
	}

	// �V�[���J��
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
	// �G�t�F�N�g�p�X�N���[��
	SetDrawScreen(effectScr_);
	ClearDrawScreen();
	
	// �G�t�F�N�g�̕`��
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

	// �I�u�W�F�N�g�p�X�N���[��
	SetDrawScreen(objectScr_);
	ClearDrawScreen();

	// ���S�̕`��
	DrawGraph(DRAW_LOGO_POS_X, DRAW_LOGO_POS_Y, titleLogo_, true);

	// ����̕`��
	SceneManager::GetInstance().GetWeapon()->Draw();

	// �_��
	if (isLogoLightVisible_)
	{
		// ���߂��Ă���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); 
		DrawGraph(DRAW_LOGO_POS_X, DRAW_LOGO_POS_Y, titleLogoLight_, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	// �܂��I�u�W�F�N�g�p�X�N���[����`��
	DrawGraph(0, 0, objectScr_, true);

	// �G�t�F�N�g�p�X�N���[��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawGraph(0, 0, effectScr_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �w�i����������ɂ���
	SetBackgroundColor(0, 0, 100);
}