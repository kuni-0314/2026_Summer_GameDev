#include <string>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Item/HP/HpItem.h"
#include "../Charactor/Enemy/EnemyBase.h"
#include "ItemManger.h"


ItemManger::ItemManger()
{
}
ItemManger::~ItemManger(void)
{
}
void ItemManger::Init(void)
{

	//Create(ItemBase::TYPE::HP, enemys_);

}
void ItemManger::Update(void)
{
	for (auto& item : items_)
	{
		item->Update();
	}
}
void ItemManger::Draw(void)
{
	for (auto& item : items_)
	{
		item->Draw();
	}
}
void ItemManger::Release(void)
{
	for (auto& item : items_)
	{
		item->Release();
		delete item;
		item = nullptr;
	}
}
void ItemManger::AddHitCollider(const ColliderBase* hitCollider)
{
	hitCollider_ = hitCollider;
}

ItemBase* ItemManger::Create(const ItemBase::TYPE& type, VECTOR pos , const ColliderBase* hitCollider)
{
	ItemBase* item = nullptr;
	switch (type)
	{
	case ItemBase::TYPE::HP:
		item = new HpItem();
		item->Init();
		item->SetPos(pos);
		item->AddHitCollider(hitCollider);
		break;
	default:
		break;
	}

	if (item != nullptr)
	{
		
		items_.emplace_back(item);
	}

	return item;
}



//void ItemManger::AddItem(ItemBase* item)
//{
//	items_.emplace
//}






