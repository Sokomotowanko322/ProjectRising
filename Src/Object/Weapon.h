#pragma once
#include<vector>
#include "../Object/Transform.h"
#include "../Object/Unit/ActorBase.h"

enum class EFFECT
{
	FIRE,
	STORM,
	LIGHTNING,
	NONE,
};

class Weapon : public ActorBase
{
public:

	// �R���X�g���N�^
	Weapon(void);

	// �f�X�g���N�^
	~Weapon(void);

	// ������
	void Init(void);

	// �X�V(�^�C�g���p)
	void TitleUpdate(void);
	void Update()override;

	// �X�V(�Q�[���V�[���p)
	void GameUpdate(Transform follow);

	// �`��
	void Draw(void);
	void SetPos(const VECTOR& pos)override;
	
	// �f�o�b�O�\��
	void DrawDebug(void);

	const int GetIsEffect(void) const;

	const VECTOR GetTopPos(void) const;
	VECTOR GetPos(void) const;

	void SetEffect(int effect);

	void Fade(void);

	void SetIsHit(bool ishit);

	// �G�t�F�N�g�̈ʒu�𓯊�
	void SyncEffect(void);

	// �G�t�F�N�g�̍Đ�/��~
	void StartEffect(void);
	void StopEffect(void);
	
protected:

	// ����̈ʒu�����
	Quaternion titleSceneQuarot_;
	Quaternion handLocalquarot_;
	VECTOR handRPos_;

	// ����̒����Ⓒ�_�ʒu
	VECTOR swordLength_;
	VECTOR swordTopPos_;

	// �G�t�F�N�g�̍��W
	VECTOR effectPos_;
	VECTOR effectScale_;

	// �G�t�F�N�g��ID
	int effectLineResId_;
	int effectLinePlayId_;

	// ��̃t���[��
	int handAttachFrame_;

	// �G�t�F�N�g����p�t���O
	bool isEffect_;

private:

	// ���\�[�X�̎Q��
	ResourceManager& resMng_;

	// Transform
	Transform weaponTransform_;
	
};