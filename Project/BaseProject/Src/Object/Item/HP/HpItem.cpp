#include "../../Charactor/Enemy/EnemyBase.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../Collider/Line/ColliderLine.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Utility/AsoUtility.h"
#include "HpItem.h"


HpItem::HpItem(EnemyBase* enemy)
	: ItemBase(enemy)
{
	enemyBase_ = enemy;

}

//HpItem::HpItem(const VECTOR& pos)
//{
//}

HpItem::~HpItem(void)
{
}

void HpItem::Draw(void)
{
	ItemBase::Draw();

	DrawFormatString(0, 300, 0xffffff, "pos:%f", transform_.pos.y);
}

void HpItem::InitLoad(void)
{
	//基底クラスのリソースロード
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ITEM_HP));

}

void HpItem::InitTransform(void)
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.pos = { 0,30,0 };
	jumpPow_ = AsoUtility::VECTOR_ZERO;
	transform_.Update();
}

void HpItem::InitCollider(void)
{
	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ITEM, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ITEM, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}

void HpItem::InitAnimation(void)
{
}

void HpItem::InitPost(void)
{
}

void HpItem::UpdateProcess(void)
{
	
}

void HpItem::UpdateProcessPost(void)
{
}
