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

const VECTOR PLAYER_WEAPON_TOP = { 0.0f, 180.0f, 0.0f };
const VECTOR PLAYER_WEAPON_BOTTOM = { 0.0f, 0.0f, 0.0f };

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
	colMng_->RegisterActor(player_->GetWeapon());

	// --- �����ň�xWeapon��modelId���m�F ---
	printfDx("Weapon modelId after Init: %d\n", player_->GetWeapon()->GetTransform().modelId);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	// �R���C�_�̃Z�b�g
	AddColliders();

	// �J�����̏�����
	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::AddColliders(void)
{
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		player_->GetPos(),
		{ 0.0f,5.0f,0.0f },
		20.0f,
		5.0f,
		player_->GetTransform().modelId,
		false	// �ђʂ��Ȃ�
	));
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		player_->GetRightHandPos(),
		{ 0.5f,0.5f,0.5f } ,
		1.0f,
		10.0f,
		player_->GetTransform().modelId,
		true,	// �ђʂ���
		true	// �E�r�R���C�_������
	));
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		normalEnemy_->GetPos(),
		{0.0f,5.0f,0.0f},
		20.0f,
		5.0f,
		normalEnemy_->GetTransform().modelId,
		false	// �ђʂ��Ȃ�
	));


	// �����Transform�擾
	Quaternion rot = player_->GetWeapon()->GetWeaponTransform().quaRot;
	VECTOR scl = player_->GetWeapon()->GetWeaponTransform().scl;
	VECTOR pos = player_->GetWeapon()->GetWeaponTransform().pos;

	// ���[�J�������[���h�ϊ�
	VECTOR tipWorld = VAdd(pos, Quaternion::PosAxis(rot, { PLAYER_WEAPON_TOP.x * scl.x, PLAYER_WEAPON_TOP.y * scl.y, PLAYER_WEAPON_TOP.z * scl.z }));
	tipWorld.y += 100.0f; // ����̐�[�����100�P�ʈړ�
	VECTOR baseWorld = VAdd(pos, Quaternion::PosAxis(rot, { PLAYER_WEAPON_BOTTOM.x * scl.x, PLAYER_WEAPON_BOTTOM.y * scl.y, PLAYER_WEAPON_BOTTOM.z * scl.z }));

	// �J�v�Z�����S�E�����E����
	VECTOR center = VScale(VAdd(tipWorld, baseWorld), 0.5f);
	VECTOR dir = VNorm(VSub(tipWorld, baseWorld));
	float height = VSize(VSub(tipWorld, baseWorld));

	// �R���C�_�ǉ�
	colMng_->AddCollider(ColliderData(
		ColliderType::Capsule,
		center,
		dir,
		height,
		2.0f, // ���a�͓K�X
		player_->GetWeapon()->GetWeaponTransform().modelId,
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

	// �R���C�_�[�̕`��
	colMng_->DrawColliders();

	// �e�X�g�p�ō���ɗ΂̎l�p��`��
	DrawBox(0, 0, 500, 50, GetColor(0, 255, 0), TRUE);
}
