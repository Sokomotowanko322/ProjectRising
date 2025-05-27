#pragma once
#include <memory>
#include <DxLib.h>
#include "../Common/Quaternion.h"
//#include "Collider/Collider.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>
class Transform
{

public:

	static constexpr float DEBUG_DIR_LEN = 300.0f;

	// �R���X�g���N�^
	Transform(void);
	explicit Transform(int model);

	// �f�X�g���N�^
	~Transform(void);

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	//���f���̌����`��
	void DrawDirection(float len = DEBUG_DIR_LEN);

	void SetModel(int modelHId);

	// �O���������擾
	VECTOR GetForward(void) const;

	// ����������擾
	VECTOR GetBack(void) const;

	// �E�������擾
	VECTOR GetRight(void) const;

	// ���������擾
	VECTOR GetLeft(void) const;

	// ��������擾
	VECTOR GetUp(void) const;

	// ���������擾
	VECTOR GetDown(void) const;

	// �Ώە������擾
	VECTOR GetDir(const VECTOR& vec) const;

	// ���f���̃n���h��ID
	int modelId;

	// �傫��
	VECTOR scl;
	// ��]
	VECTOR rot;
	// �ʒu
	VECTOR pos;
	VECTOR localPos;

	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// ��]
	Quaternion quaRot;

	// ���[�J����]
	Quaternion quaRotLocal;
};
