#pragma once
#include "../Collider/ColliderBase.h"
#include <vector>
#include "ItemBase.h"

class ColliderBase;
class EnemyBase;

class ItemManger
{

public:
	// コンストラクタ
	// デストラクタ
	ItemManger();
	~ItemManger(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);
	//エネミー
	const std::vector<ItemBase*>& GetItems(void) const { return items_; }
	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);
	
	// アイテム生成
	ItemBase* Create(const ItemBase::TYPE&type, VECTOR pos, const ColliderBase* hitCollider);

	//void AddItem(ItemBase* item);

private:


	//アイテム
	std::vector<ItemBase*> items_;
	//enemy
	EnemyBase* enemys_;
	const ColliderBase* hitCollider_;
};




