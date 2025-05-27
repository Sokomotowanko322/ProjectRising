#include <DxLib.h>
#include "../Utility/Utility.h"
#include "Transform.h"

Transform::Transform(void)
	:
	modelId(-1),
	scl(Utility::VECTOR_ONE),
	rot(Utility::VECTOR_ZERO),
	pos(Utility::VECTOR_ZERO),
	localPos(Utility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{

}

Transform::Transform(int model)
	:
	modelId(model),
	scl(Utility::VECTOR_ONE),
	rot(Utility::VECTOR_ZERO),
	pos(Utility::VECTOR_ZERO),
	localPos(Utility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{

}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// �傫��
	matScl = MGetScale(scl);

	// ��]
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// �ʒu
	matPos = MGetTranslate(pos);

	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// �s������f���ɔ���
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

}

//���f���̌����`��
void Transform::DrawDirection(float len)
{
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());

	auto xDir = q.GetRight();
	auto zDir = q.GetForward();
	auto yDir = q.GetUp();


	DrawLine3D(pos, VAdd(pos, VScale(xDir, len)), 0xff0000);
	DrawLine3D(pos, VAdd(pos, VScale(yDir, len)), 0x00ff00);
	DrawLine3D(pos, VAdd(pos, VScale(zDir, len)), 0x0000ff);

}

void Transform::SetModel(int model)
{
	modelId = model;
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}