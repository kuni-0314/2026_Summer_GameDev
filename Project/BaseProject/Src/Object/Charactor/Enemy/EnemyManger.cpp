#include <string>
#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Scene/GameScene.h"
#include "Rat/EnemyRat.h"
#include "Robot/EnemyRobot.h"
#include "../../Item/ItemBase.h"
#include "../../Item/ItemManger.h"
#include "EnemyManger.h"




EnemyManager::EnemyManager(GameScene* gamescene, Player* player)
	:
	gameScene_(gamescene),
	player_(player)
{
}
EnemyManager::~EnemyManager(void)
{
}
void EnemyManager::Init(void)
{

	LoadCsvData();

}
void EnemyManager::Update(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Update();

		if (enemy->GetHp() <= 0 && enemy->IsAlive())
		{

			gameScene_->GetItemManger()->Create(ItemBase::TYPE::HP, enemy->GetTransform().pos,hitCollider_);
		

			enemy->SetAlive(false);
		}
	}

	for (int j = 0; j < enemies_.size(); j++)
	{
		if (enemies_[j]->IsAnimEnd() && !enemies_[j]->IsAlive())
		{
			enemies_[j]->Release();
			delete enemies_[j];
			enemies_[j] = nullptr;
			enemies_.erase(std::remove(enemies_.begin(), enemies_.end(), enemies_[j]), enemies_.end());

			j--;
		}
	}
}
void EnemyManager::Draw(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}
void EnemyManager::Release(void)
{
	for (auto& enemy : enemies_)
	{
		enemy->Release();
		delete enemy;
		enemy = nullptr;
	}
}
void EnemyManager::AddHitCollider(const ColliderBase* hitCollider)
{
	
	hitCollider_ = hitCollider;

	for (auto& enemy : enemies_)
	{
		enemy->AddHitCollider(hitCollider);
		
	}
}

void EnemyManager::LoadCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_CSV + "EnemyData.csv");
	if (!ifs)
	{
		// エラーが発生
		return;
	}
	// ファイルを１行ずつ読み込む
	std::string line;// 1行の文字情報
	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割
	bool isHeader = true;
	while (getline(ifs, line))
	{
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		// １行をカンマ区切りで分割
		strSplit = AsoUtility::Split(line, ',');
		EnemyBase* enemy = nullptr;
	
		// 構造体に合わせて読込データを格納
		EnemyBase::EnemyData data = EnemyBase::EnemyData();
		int idx = 0;
		// ID
		data.id = stoi(strSplit[idx++]);
		// 種別
		data.type = static_cast<EnemyBase::TYPE>(stoi(strSplit[idx++]));
		// HP
		data.hp = stoi(strSplit[idx++]);
		// 初期座標
		data.defaultPos =
		{
		stof(strSplit[idx++]),
		stof(strSplit[idx++]),
		stof(strSplit[idx++])
		};

		//移動範囲
		data.movableRange = stoi(strSplit[idx++]);

		// エネミー生成
		Create(data);
	}
	ifs.close();
}

EnemyBase* EnemyManager::Create(const EnemyBase::EnemyData& data)
{
	EnemyBase* enemy = nullptr;
	switch (data.type)
	{
	case EnemyBase::TYPE::RAT:
		enemy = new EnemyRat(data,player_);
		break;
	default:
		break;
	}

	if (enemy != nullptr)
	{
		enemy->Init();
		enemies_.emplace_back(enemy);
	}

	return enemy;

}

VECTOR EnemyManager::GetNearEnemyPos(const VECTOR& pos) const
{
	float minDist = FLT_MAX;
	VECTOR nearPos = { 0.0f, 0.0f, 0.0f };
	for (auto& enemy : enemies_)
	{
		VECTOR enemyPos = enemy->GetTransform().pos;
		VECTOR toEnemyVec = VSub(enemyPos, pos);
		float dist = VSize(toEnemyVec);
		if (dist < minDist)
		{
			minDist = dist;
			nearPos = enemyPos;
		}
	}
	return nearPos;
}

VECTOR EnemyManager::GetEnemyPos(int id) const
{
	if (id < 0) id = 0;
	else if (id >= enemies_.size()) id = enemies_.size() - 1;

	return enemies_[id]->GetTransform().pos;
}
