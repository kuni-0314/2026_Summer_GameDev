#pragma once
#include "../Collider/ColliderBase.h"
#include <vector>
#include "ItemBase.h"

class ColliderBase;
class Player;


class ItemManger
{

public:
	// コンストラクタ
	// デストラクタ
	ItemManger(Player* player);
	~ItemManger(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);
	// エネミー
	//const std::vector<ItemManger*>& GetItems(void) const { return items_; }
	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVから敵情報の読取を行う
	//void LoadCsvData(void);
	
	// エネミー生成
	//ItemManger* Create(const ItemBase::TYPE);

	// 指定座標に最も近いエネミーの座標を取得
	VECTOR GetNearEnemyPos(const VECTOR& pos) const;


private:
	//プレイヤー
	Player* player_;

	// エネミー
	std::vector<ItemBase*> items_;
};




