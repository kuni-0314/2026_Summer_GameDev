#include "SwordBase.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBase.h"

SwordBase::SwordBase(TYPE type, const Transform& ownerTransform)
	: type_(type)
	, ownerTransform_(ownerTransform)
{
}

void SwordBase::Update()
{
	

	// Transform の更新
	transform_.Update();
}

void SwordBase::Draw()
{
	ActorBase::Draw();

	// デバッグ用に攻撃判定位置の描画（ワールド座標に変換）
	if (!hitboxes_.empty())
	{
		// スケールを除外して回転と位置のみ適用
		MATRIX matRot = transform_.quaRot.ToMatrix();
		MATRIX matPos = MGetTranslate(transform_.pos);

		MATRIX weaponWorldMat = MGetIdent();
		weaponWorldMat = MMult(weaponWorldMat, matRot);
		weaponWorldMat = MMult(weaponWorldMat, matPos);

		VECTOR worldStart = VTransform(hitboxes_[0].localPosStart, weaponWorldMat);
		VECTOR worldEnd = VTransform(hitboxes_[0].localPosEnd, weaponWorldMat);

		DrawSphere3D(worldStart, 5.0f, 16, 0xff0000, 0xffffff, false);
		DrawSphere3D(worldEnd, 5.0f, 16, 0x0000ff, 0xffffff, true);
		//DrawCapsule3D(worldStart, worldEnd, hitboxes_[0].radius, 8, 0x00ff00, 0x00ff00, false);
	}
}

void SwordBase::BeginAttack(int attackType)
{
}

void SwordBase::EndAttack()
{
}