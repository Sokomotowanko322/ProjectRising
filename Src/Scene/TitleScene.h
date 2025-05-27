#include <memory>
#include "SceneBase.h"
#include "../Object/Transform.h"

class Weapon;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^&�f�X�g���N�^
	TitleScene(void);
	~TitleScene(void);

	// ������
	void Init(void)override;

	// �`��
	void Draw(void)override;

	// �X�V
	void Update(void)override;


protected:

private:

	VECTOR screenEffectPos_;

	// ���S�Ȃ�
	int titleLogo_;
	int titleLogoLight_;

	// �^�C�g���̃G�t�F�N�g
	int titleLightningResId_;
	int titleLightningPlayId_;

	// �G�t�F�N�g��������p�̃X�N���[��
	int effectScr_;
	
	// �I�u�W�F�N�g�`��p�̃X�N���[��
	int objectScr_;

	// �_�Ő���p
	float logoLightTimer_;
	float logoLightInterval_;
	bool  isLogoLightVisible_;
};

