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

	// デバッグ用に攻撃判定位置の描画
	DrawSphere3D(hitboxes_[0].localPosStart, 5.0f, 16, 0xff0000, 0xffffff, false);
	DrawSphere3D(hitboxes_[0].localPosEnd, 5.0f, 16, 0x0000ff, 0xffffff, true);
}

void SwordBase::BeginAttack(int attackType)
{
}

void SwordBase::EndAttack()
{
}