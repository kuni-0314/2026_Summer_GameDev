#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "Transform.h"

Transform::Transform()
	:
	modelId(-1),
	scl(AsoUtility::VECTOR_ONE),
	rot(AsoUtility::VECTOR_ZERO),
	pos(AsoUtility::VECTOR_ZERO),
	localPos(AsoUtility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())

{
}

Transform::~Transform()
{
}

void Transform::Update()
{

	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

}

void Transform::Release()
{
}

void Transform::SetModel(int model)
{
	modelId = model;
}

VECTOR Transform::GetForward() const
{
	return GetDir(AsoUtility::DIR_F);
}

VECTOR Transform::GetBack() const
{
	return GetDir(AsoUtility::DIR_B);
}

VECTOR Transform::GetRight() const
{
	return GetDir(AsoUtility::DIR_R);
}

VECTOR Transform::GetLeft() const
{
	return GetDir(AsoUtility::DIR_L);
}

VECTOR Transform::GetUp() const
{
	return GetDir(AsoUtility::DIR_U);
}

VECTOR Transform::GetDown() const
{
	return GetDir(AsoUtility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& dir) const
{
	return quaRot.PosAxis(dir);
}
