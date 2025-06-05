#pragma once
#include <memory>
#include <chrono>
#include <unordered_map>
#include <functional>
#include "../Object/Weapon.h"

// �������܂��񂪁A�ǂ����Ă��g����������
#define mainCamera SceneManager::GetInstance().GetCamera()

class SceneBase;
class Fader;
class Camera;
//class Collision;
class ColliderManager;

class SceneManager
{

public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		RESULT
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	// <summary>
	/// �Փ˔���o�^���̎擾
	/// </summary>
	/// <returns>ColliderManager�̎Q��</returns>
	/*inline ColliderManager& GetColManager(void) const
	{
		return *colMng_;
	}*/

	/// <summary>
	/// �Փ˔���̎擾
	/// </summary>
	/// <returns>Collision�̎Q��</returns>
	/*inline Collision& GetCollision(void) const
	{
		return *collision_;
	}*/

	/// <summary>
	/// �������Ԃ��Z�b�g����
	/// </summary>
	/// <param name="time">��������</param>
	void SetTime(float time);


	/// <summary>
	/// �������Ԃ��擾
	/// </summary>
	/// <returns>gameTime_</returns>
	inline const float GetTime(void) const
	{
		return gameTime_;
	}

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	Camera& GetCamera(void);

	Weapon* GetWeapon() {
		return weapon_.get();
	};

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �e��V�[��
	std::unique_ptr<SceneBase> scene_;

	// �e��V�[���p
	std::unique_ptr<Weapon> weapon_;

	// �t�F�[�h
	std::unique_ptr<Fader> fader_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	//�Q�[���J�E���g�p�ϐ�
	float gameTime_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager& manager) = default;
	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);
};
