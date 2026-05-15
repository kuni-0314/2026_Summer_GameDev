#include <string>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "ItemManger.h"


ItemManger::ItemManger(Player* player)
	:
	player_(player)
{
}
ItemManger::~ItemManger(void)
{
}
void ItemManger::Init(void)
{


	//LoadCsvData();

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
	for (auto& item : items_)
	{
		item->AddHitCollider(hitCollider);

	}
}

//void ItemManger::LoadCsvData(void)
//{
//	// ファイルの読込
//	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "EnemyData.csv");
//	if (!ifs)
//	{
//		// エラーが発生
//		return;
//	}
//	// ファイルを１行ずつ読み込む
//	std::string line;// 1行の文字情報
//	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割
//	bool isHeader = true;
//	while (getline(ifs, line))
//	{
//		if (isHeader)
//		{
//			isHeader = false;
//			continue;
//		}
//		// １行をカンマ区切りで分割
//		strSplit = AsoUtility::Split(line, ',');
//		ItemBase* item = nullptr;
//
//		// 構造体に合わせて読込データを格納
//		ItemBase::EnemyData data = EnemyBase::EnemyData();
//		int idx = 0;
//		// ID
//		data.id = stoi(strSplit[idx++]);
//		// 種別
//		data.type = static_cast<EnemyBase::TYPE>(stoi(strSplit[idx++]));
//		// HP
//		data.hp = stoi(strSplit[idx++]);
//		// 初期座標
//		data.defaultPos =
//		{
//		stof(strSplit[idx++]),
//		stof(strSplit[idx++]),
//		stof(strSplit[idx++])
//		};
//
//		//移動範囲
//		data.movableRange = stoi(strSplit[idx++]);
//
//		// エネミー生成
//		Create(data);
//	}
//	ifs.close();
//}

//ItemBase* ItemManger::Create(const ItemBase::EnemyData& data)
//{
//	ItemBase* item = nullptr;
//	switch (data.type)
//	{
//	case ItemBase::TYPE::HP:
//		item = new EnemyRat(data, player_);
//		break;
//	default:
//		break;
//	}
//
//	if (item != nullptr)
//	{
//		item->Init();
//		items_.emplace_back(item);
//	}
//
//	return item;
//
//}

VECTOR ItemManger::GetNearEnemyPos(const VECTOR& pos) const
{
	float minDist = FLT_MAX;
	VECTOR nearPos = { 0.0f, 0.0f, 0.0f };
	for (auto& item : items_)
	{
		VECTOR itemPos = item->GetTransform().pos;
		VECTOR toItemVec = VSub(itemPos, pos);
		float dist = VSize(toItemVec);
		if (dist < minDist)
		{
			minDist = dist;
			nearPos = itemPos;
		}
	}
	return nearPos;
}



