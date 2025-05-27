#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	std::unique_ptr<Resource> res;

#pragma region �摜�t�@�C��

	// �^�C�g���摜
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Logo.png");
	resourcesMap_.emplace(SRC::TITLE_IMAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Logo_Light.png");
	resourcesMap_.emplace(SRC::TITLE_IMAGE_LIGHT, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Start.png");
	resourcesMap_.emplace(SRC::START_LOGO, std::move(res));

	// �v���C���[��HP�g
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpFrame.png");
	resourcesMap_.emplace(SRC::HP_FRAME, std::move(res));

	// �q�b�g���ɕ\������摜
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Hit.png");
	resourcesMap_.emplace(SRC::HIT_IMAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "NumImage.png");
	resourcesMap_.emplace(SRC::HIT_NUMIMAGE, std::move(res));

	// ���U���g�摜
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultImage.png");
	resourcesMap_.emplace(SRC::RESULT, std::move(res));

	// PushSpace
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	// �v���C���[�e
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

#pragma endregion

#pragma region ���f��

	// �v���C���[
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	// ����
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Weapon/katana2.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WEAPON, std::move(res));

	// �X�J�C�h�[��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	// �X�e�[�W
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Colliders.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLLIDER, std::move(res));

	// �G���f��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemy.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/EnemyBoss.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BOSS_TYPE1, std::move(res));

#pragma endregion

#pragma region �v���C���[�̃A�j���[�V����

	// �v���C���[
	// �ړ��n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, std::move(res));	
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/Walk.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WALK, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/Dash.mv1");
	resourcesMap_.emplace(SRC::PLAYER_DASH, std::move(res));
	
	// �U��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/HighTime.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIGH_TIME, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/SmashAttack.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SMASH_ATTACK, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/Combo1.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FIRSTCOMBO, std::move(res));

	// ���̑�
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Anim/HasWeapon.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HASARM, std::move(res));

#pragma endregion

#pragma region �G�̃A�j���[�V����

	// �G
	// �ړ��n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Anim/Idle.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_IDLE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Anim/Walk.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_WALK, std::move(res));

	// �U���n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/NormalAttack.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_NORMALATTACK, std::move(res));

	// ���S
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Death.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_DEATH, std::move(res));

#pragma endregion

#pragma region �G�t�F�N�g�S��

	// ��
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Lightning.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_TITLE_LIGHTNING, std::move(res));

	// �z���G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Inhale.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_INHALE, std::move(res));

	// �q�b�g���G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Slash_Hit.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, std::move(res));

	// ���ɒǏ]����G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SwordTrail.efkefc");
	resourcesMap_.emplace(SRC::PLAYER_WEAPON_SWORDLINE, std::move(res));

#pragma endregion

#pragma region �T�E���h

	// �{�^������
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, Application::PATH_SOUND + "TitleEnter.mp3");
	resourcesMap_.emplace(SRC::SOUND_ENTER, std::move(res));

	// �^�C�g�����BGM 
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, Application::PATH_SOUND + "TitleBgm.mp3");
	resourcesMap_.emplace(SRC::SOUND_TITLEBGM, std::move(res));

#pragma endregion

#pragma region �V�F�[�_�[
	
	// ���V�F�[�_�[
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_SHADER + "Lightning.cso");
	resourcesMap_.emplace(SRC::LIGHTNING_SHADER, std::move(res));

#pragma endregion
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;
}