#pragma once
#include "../Collider/ColliderBase.h"
#include <vector>
#include "ItemBase.h"

class ColliderBase;
class EnemyBase;
class Player;

class ItemManger
{

public:
	// コンストラクタ
	// デストラクタ
	ItemManger();
	~ItemManger();
	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 解放
	void Release();
	//エネミー
	const std::vector<ItemBase*>& GetItems() const { return items_; }
	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// アイテム生成
	ItemBase* Create(const ItemBase::TYPE& type, VECTOR pos, const ColliderBase* hitCollider, const int key, const Player* player);




private:


	//アイテム
	std::vector<ItemBase*> items_;
	//enemy
	EnemyBase* enemys_;

	Player* player_;
	const ColliderBase* hitCollider_;
};




