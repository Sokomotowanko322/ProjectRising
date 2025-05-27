#pragma once
class SceneManager;
class Camera;
class Application;
//class Collision;
class ResourceManager;
class ColliderManager;
class Weapon;

class SceneBase
{

public:

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void) = 0;

	// ����������
	virtual void Init(void) = 0;

	// �X�V�X�e�b�v
	virtual void Update(void) = 0;

	// �`�揈��
	virtual void Draw(void) = 0;

protected:

	//�J����
	Camera& camera_;

	//�Փ˔���
	//Collision& collision_;

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

	// �V�[���Ǘ�
	SceneManager& scnMng_;

	// �V�[���Ǘ�
	//ColliderManager& colMng_;
};