#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/ColliderManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "../Common/Fader.h"
#include "../Object/SkyDome.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/NormalEnemy.h"
#include "../Object/Weapon.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// �v���C���[�̏�����
	player_ = std::make_shared<Player>();
	player_->Init();

	// �G�̏�����
	normalEnemy_ = std::make_shared<NormalEnemy>(player_);
	normalEnemy_->Init();

	// �R���C�_����
	colMng_ = std::make_unique<ColliderManager>();
	colMng_->RegisterActor(player_);
	colMng_->RegisterActor(normalEnemy_);

	// �R���C�_������
	InitCollider();

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	// �J�����̏�����
	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::InitCollider(void)
{
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		player_->GetPos(),
		Utility::VECTOR_ZERO,
		1.0f,
		2.0f,
		player_->GetTransform().modelId,
		true // �g���K�[
	));
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		normalEnemy_->GetPos(),
		Utility::VECTOR_ZERO,
		1.0f,
		2.0f,
		normalEnemy_->GetTransform().modelId,
		true // �g���K�[
	));
}

void GameScene::Update(void)
{
	// �R���g���[���^�C�v�̐ݒ�
	auto& insI = InputManager::GetInstance();
	auto pad = insI.GetJPadState(JOYPAD_NO::PAD1);
	insI.SetControlType(InputManager::CONTROL_TYPE::KEY);

	// �w�i�Ǐ]
	skyDome_->Update();

	// �v���C���[�̍X�V
	player_->Update();

	// �G�̍X�V
	normalEnemy_->Update();

	colMng_->Update();
	
}

void GameScene::Draw(void)
{
	// skydome�̕`��
	skyDome_->Draw();

	// �v���C���[�`��
	player_->Draw();

	// �G�`��
	normalEnemy_->Draw();

	// �e�X�g�p�ō���ɗ΂̎l�p��`��
	DrawBox(0, 0, 500, 50, GetColor(0, 255, 0), TRUE);
}
