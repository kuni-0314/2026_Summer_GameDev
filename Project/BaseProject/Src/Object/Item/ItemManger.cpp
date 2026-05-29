#include <string>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Item/HP/HpItem.h"
#include "../Charactor/Enemy/EnemyBase.h"
#include "../Charactor/Player/Player.h"
#include "ItemManger.h"


ItemManger::ItemManger()
{
}
ItemManger::~ItemManger(void)
{
}
void ItemManger::Init(void)
{



}
void ItemManger::Update(void)
{
	for (auto& item : items_)
	{
		item->Update();
	}

	for (int j = 0; j < items_.size(); j++)
	{
		if (items_[j]->IsAlive() == false)
		{
			items_[j]->Release();
			delete items_[j];

			items_.erase(items_.begin() + j);

			j--;
		}
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

ItemBase* ItemManger::Create(const ItemBase::TYPE& type, VECTOR pos, const ColliderBase* hitCollider, const int key, const Player* player)
{
	ItemBase* item = nullptr;
	switch (type)
	{
	case ItemBase::TYPE::HP:

		item = new HpItem(const_cast<Player*>(player));
		item->Init();
		item->SetPos(pos);
		item->AddHitCollider(hitCollider);
		const ColliderBase* playerCollide = player->GetOwnCollider(key);

		break;
	}

	if (item != nullptr)
	{

		items_.emplace_back(item);
	}

	return item;
}







