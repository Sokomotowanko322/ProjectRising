#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
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
		
#pragma region �G�t�F�N�g

		EFFECT_TITLE_LIGHTNING,
		EFFECT_INHALE,
		EFFECT_HIT,

#pragma endregion

#pragma region �T�E���h

		SOUND_TITLEBGM,
		SOUND_ENTER,

#pragma endregion

#pragma region �V�F�[�_�[
		
		LIGHTNING_SHADER

#pragma endregion
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
