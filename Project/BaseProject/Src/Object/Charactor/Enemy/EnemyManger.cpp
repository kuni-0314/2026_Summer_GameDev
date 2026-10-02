#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Scene/GameScene.h"
#include "Rat/EnemyRat.h"
#include "Robot/EnemyRobot.h"
#include "Rase/EnemyRase.h"
#include "Large/EnemyLarge.h"
#include "Dragon/EnemyDragon.h"
#include "../../Item/ItemBase.h"
#include "../../Item/ItemManger.h"
#include "../../Charactor/Player/Player.h"
#include "../../Actor/ActorBase.h"
#include "../../Collider/Sphere/ColliderSphere.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Effect/LoadEffekseer/EffekseerEffect.h"
#include "../../../Effect/EffectManager.h"
#include "../../../Common/Quaternion.h"
#include "../../../Manager/ResourceManager.h"
#include "EnemyManger.h"

// 長いのでnamespaceの省略
using json = nlohmann::json;

EnemyManager::EnemyManager(GameScene* gamescene, Player* player)
	:
	gameScene_(gamescene),
	player_(player)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	//敵生成座標
	InitEnemyPos();

	//Json形式の読み込み
	LoadJsonStatusData();
	LoadJsonWaveData();

	// WAVE1の敵を生成
	LoadWaveData(WAVE::WAVE1);
}

void EnemyManager::Update()
{
	//wave更新
	UpdateWave();
}

void EnemyManager::Draw()
{
	WAVE next = wave_;
	const char* name = "";
	if (next == WAVE::WAVE1) name = "WAVE1";
	else if (next == WAVE::WAVE2) name = "WAVE2";
	else if (next == WAVE::WAVE3) name = "WAVE3";
	else if (next == WAVE::BOSS) name = "BOSS";

	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}
void EnemyManager::Release()
{
	EffectManager::GetInstance().Clear();
	for (auto& enemy : enemies_)
	{
		if (enemy)
		{
			enemy->Release();
			delete enemy;
		}
		enemy = nullptr;
	}
	enemies_.clear();
}

void EnemyManager::AddHitCollider(const ColliderBase* hitCollider)
{
	//重複登録を避けるため、既存の敵に登録されているかの確認
	if (std::find(hitColliders_.begin(), hitColliders_.end(), hitCollider) == hitColliders_.end())
	{
		hitColliders_.push_back(hitCollider);
	}

	hitCollider_ = hitCollider;

	//衝突判定の追加
	for (auto& enemy : enemies_)
	{
		enemy->AddHitCollider(hitCollider);
	}
}

void EnemyManager::RemoveHitCollider(const ColliderBase* hitCollider)
{
	// hitColliders_ から削除
	auto it = std::find(hitColliders_.begin(), hitColliders_.end(), hitCollider);
	if (it != hitColliders_.end())
	{
		hitColliders_.erase(it);
	}

	// hitCollider_ が同じものを指していた場合はクリア
	if (hitCollider_ == hitCollider)
	{
		hitCollider_ = nullptr;
	}

	// 各敵からもコライダを削除
	for (auto& enemy : enemies_)
	{
		enemy->RemoveHitCollider(hitCollider);
	}
}

EnemyBase* EnemyManager::Create(const EnemyBase::EnemyData& data, const Player* player)
{

	EnemyBase* enemy = nullptr;
	switch (data.type)
	{
	case EnemyBase::TYPE::RAT:
		enemy = new EnemyRat(data, -1, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::RASE:
		enemy = new EnemyRase(data, -1, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::LARGE:
		enemy = new EnemyLarge(data, -1, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::DRAGON:
		enemy = new EnemyDragon(data, -1, const_cast<Player*>(player));
		break;
	default:
		break;
	}

	if (enemy != nullptr)
	{
		enemy->Init();

		//新たに生成される敵に対して、コライダを追加
		for (const auto* collider : hitColliders_)
		{
			if (collider != nullptr)
			{
				enemy->AddHitCollider(collider);
			}
		}

		enemies_.emplace_back(enemy);

		SpawnEffect(enemy->GetTransform().pos);


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
	// 敵が存在しない
	if (enemies_.empty())
	{
		return VGet(0.0f, 0.0f, 0.0f);
	}
	if (enemies_.empty())
	{
		return { 0.0f, 0.0f, 0.0f };
	}
	if (id < 0) id = 0;
	else if (id >= enemies_.size()) id = enemies_.size() - 1;

	return enemies_[id]->GetTransform().pos;
}

bool EnemyManager::GetEnemyDead()
{
	return isDead_;
}

void EnemyManager::SpawnEffect(const VECTOR& pos)
{
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Ribbon/Ribbon.efkefc",
		pos
	);

	effect->SetLifeTime(45);   // 約0.75秒

	effect->Play(
		pos,
		Quaternion()
	);

	EffectManager::GetInstance().RegisterEffect(effect);
}

void EnemyManager::DeadEffect(const VECTOR& pos)
{
	VECTOR effectPos = pos;
	effectPos.y += 80.0f;

	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Death/Death.efkefc",
		effectPos
	);

	effect->Play(
		effectPos,
		Quaternion()
	);
	EffectManager::GetInstance().RegisterEffect(effect);
}

void EnemyManager::CheckHit(const VECTOR& pos, float radius, int damage)
{
	for (auto& enemy : enemies_)
	{
		if (!enemy->IsAlive())
		{
			continue;
		}
		VECTOR enemyPos = enemy->GetTransform().pos;
		float dist = VSize(VSub(enemyPos, pos));
		if (dist <= radius)
		{
			VECTOR dir = VSub(enemyPos, pos);
			dir = VNorm(dir);
			enemy->Damage(damage, dir);
		}
	}
}

void EnemyManager::SetWave(const WAVE wave)
{
	wave_ = wave;
}

void EnemyManager::CreateHpItem()
{
	for (auto& enemy : enemies_)
	{
		enemy->Update();

		if (enemy->GetHp() <= 0 && enemy->IsAlive())
		{
			// 死亡エフェクト
			DeadEffect(enemy->GetTransform().pos);

			// HPアイテム生成位置を敵の上に出す
			VECTOR hpPos = enemy->GetTransform().pos;

			//左右に広がるように調整
			int randX = (rand() % 201) - 100; // -100 .. +100
			int randZ = (rand() % 201) - 100; // -100 .. +100

			hpPos.x += static_cast<float>(randX);
			hpPos.z += static_cast<float>(randZ);

			// ここで確実に地面より上に出す
			hpPos.y += 80.0f;

			// 安全チェック：ItemManager とステージコライダが有効か確認してから生成
			auto itemMgr = gameScene_ ? gameScene_->GetItemManger() : nullptr;

			//HPアイテムの生成
			if (itemMgr != nullptr && hitCollider_ != nullptr)
			{
				itemMgr->Create(ItemBase::TYPE::HP, hpPos, hitCollider_,
					static_cast<int>(Player::COLLIDER_TYPE::CAPSULE), player_);
			}

			//生存フラグ（オフ）
			enemy->SetAlive(false);
		}
	}
}

void EnemyManager::EnemysDelete()
{
	//エネミー削除
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

void EnemyManager::EnemysCollision()
{
	//敵同士の衝突判定（敵ががぶらないように）
	for (auto& enemy1 : enemies_)
	{
		//自身を探して自身のコライダー情報を渡す
		for (auto& enemy2 : enemies_)
		{
			if (enemy1 == enemy2)
			{
				continue;
			}

			//enemy自身の衝突判定を取得
			const ColliderBase* enemyCollider =
				enemy2->GetOwnCollider(static_cast<int>(ActorBase::COLLIDER_TYPE::CAPSULE));

			enemy1->AddHitCollider(enemyCollider);

		}
	}
}

void EnemyManager::ChangeWave(WAVE wave)
{
	switch (wave)
	{
	case EnemyManager::WAVE::WAVE1:
		break;
	case EnemyManager::WAVE::WAVE2:
		break;
	case EnemyManager::WAVE::WAVE3:
		break;
	case EnemyManager::WAVE::BOSS:
		break;
	case EnemyManager::WAVE::END:
		break;
	default:
		break;
	}
}

void EnemyManager::LoadWaveData(WAVE wave)
{
	wave_ = wave;

	spawnTimer_ = 0.0f;
	spawnIndex_ = 0;
	currentWaveEnemyCount_ = 0;

	// 現在のWAVEの敵数を取得
	for (const auto& data : enemyWaveData_)
	{
		if (data.wave == static_cast<int>(wave_))
		{
			currentWaveEnemyCount_ =
				static_cast<int>(data.enemies.size());

			break;
		}
	}

	ChangeWave(wave_);

	// 1体目を生成
	SpawnNextEnemy();
}

void EnemyManager::UpdateWave()
{
	switch (wave_)
	{
	case EnemyManager::WAVE::WAVE1:
		UpdateWave1();
		break;
	case EnemyManager::WAVE::WAVE2:
		UpdateWave2();
		break;
	case EnemyManager::WAVE::WAVE3:
		UpdateWave3();
		break;
	case EnemyManager::WAVE::BOSS:
		UpdateWaveBoss();
		break;
	case EnemyManager::WAVE::END:
		break;
	default:
		break;
	}
}

void EnemyManager::UpdateWave1()
{
	CreateHpItem();
	EnemysDelete();

	// 敵生成タイマー
	spawnTimer_++;

	// まだ生成する敵が残っている
	if (spawnIndex_ < currentWaveEnemyCount_)
	{
		if (spawnTimer_ >= DEF_SPAWN_INTERVAL)
		{
			spawnTimer_ = 0.0f;

			// 次の敵を生成
			SpawnNextEnemy();
		}
	}

	// WAVE1の敵をすべて生成したか確認
	if (spawnIndex_ >= currentWaveEnemyCount_)
	{
		// 全滅しているか確認
		wave1Clear_ = true;

		for (const auto enemy : enemies_)
		{
			if (enemy->IsAlive())
			{
				wave1Clear_ = false;
				break;
			}
		}

		// 全生成済み ＆ 全滅
		if (wave1Clear_)
		{
			LoadWaveData(WAVE::WAVE2);
		}
	}
}

void EnemyManager::UpdateWave2()
{
	CreateHpItem();
	EnemysDelete();

	// 敵生成タイマー
	spawnTimer_++;

	// まだ生成する敵が残っている
	if (spawnIndex_ < currentWaveEnemyCount_)
	{
		if (spawnTimer_ >= DEF_SPAWN_INTERVAL)
		{
			spawnTimer_ = 0.0f;

			// 次の敵を生成
			SpawnNextEnemy();
		}
	}

	// WAVE1の敵をすべて生成したか確認
	if (spawnIndex_ >= currentWaveEnemyCount_)
	{
		// 全滅しているか確認
		wave2Clear_ = true;

		for (const auto enemy : enemies_)
		{
			if (enemy->IsAlive())
			{
				wave2Clear_ = false;
				break;
			}
		}

		// 全生成済み ＆ 全滅
		if (wave2Clear_)
		{
			LoadWaveData(WAVE::WAVE3);
		}
	}
}

void EnemyManager::UpdateWave3()
{
	CreateHpItem();
	EnemysDelete();

	// 敵生成タイマー
	spawnTimer_++;

	// まだ生成する敵が残っている
	if (spawnIndex_ < currentWaveEnemyCount_)
	{
		if (spawnTimer_ >= DEF_SPAWN_INTERVAL)
		{
			spawnTimer_ = 0.0f;

			// 次の敵を生成
			SpawnNextEnemy();
		}
	}

	// WAVE1の敵をすべて生成したか確認
	if (spawnIndex_ >= currentWaveEnemyCount_)
	{
		// 全滅しているか確認
		wave3Clear_ = true;

		for (const auto enemy : enemies_)
		{
			if (enemy->IsAlive())
			{
				wave3Clear_ = false;
				break;
			}
		}

		// 全生成済み ＆ 全滅
		if (wave3Clear_)
		{
			LoadWaveData(WAVE::BOSS);
		}
	}
}


void EnemyManager::UpdateWaveBoss()
{
	CreateHpItem();
	EnemysDelete();
	bossSpawnTimer_++;

	if (bossSpawnTimer_ >= BOSS_SPAWN_INTERVAL)
	{
		bossSpawnTimer_ = 0.0f;

		SpawnBossEnemy();
	}

	//エネミー全滅フラグ
	isDead_ = true;
	for (const auto enemy : enemies_)
	{
		if (enemy->IsAlive())
		{
			isDead_ = false;
			break;
		}
	}
}


void EnemyManager::LoadJsonStatusData()
{
	// 外部ファイルの読み込み
	std::ifstream ifs;
	ifs.open(Application::PATH_JSON + "Enemy.json");
	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	// ファイルストリームからjsonオブジェクトに変換
	json enemyData = json::parse(ifs);
	// jsonオブジェクトから、enemyオブジェクトを取得
	const auto& enemyDatas = enemyData["enemy"];

	// enemyオブジェクトは複数あるはずなので、繰り返し処理
	for (const json& enemyData : enemyDatas)
	{
		EnemyBase::EnemyStatus data{};

		//敵の種類
		data.type = static_cast<EnemyBase::TYPE>(enemyData["type"]);
		//HP
		data.hp = enemyData["hp"];

		enemyStatusData_.push_back(data);
	}
	//ファイルを閉じる
	ifs.close();

}

void EnemyManager::LoadJsonWaveData()
{
	// 外部ファイルの読み込み
	std::ifstream ifs;
	ifs.open(Application::PATH_JSON + "Wave.json");
	if (!ifs)
	{
		return;// 外部ファイルの読み込み失敗
	}

	// ファイルストリームからjsonオブジェクトに変換
	json waveData = json::parse(ifs);
	// jsonオブジェクトから、enemyオブジェクトを取得
	const auto& waveDatas = waveData["WAVE"];


	// WAVEごとに使用済み座標を管理
	std::map<int, std::vector<int>> usedPositions;
	std::map<int, int> largeCount;

	// enemyオブジェクトは複数あるはずなので、繰り返し処理
	for (const json& waveData : waveDatas)
	{
		EnemyBase::EnemyWave data{};

		data.wave = waveData["wave"];

		for (const auto& enemyType : waveData["enemies"])
		{
			data.enemies.push_back(enemyType);
		}

		enemyWaveData_.push_back(data);
		
	}
	//ファイルを閉じる
	ifs.close();
}

void EnemyManager::InitEnemyPos()
{
	EnemyPos_.resize(11);

	EnemyPos_[0] = { 600,  40,  2500 };
	EnemyPos_[1] = { 1200, 40, 800 };
	EnemyPos_[2] = { 2500, 40, 300 };
	EnemyPos_[3] = { 800,  40, 1800 };
	EnemyPos_[4] = { 2000, 40, 1500 };
	EnemyPos_[5] = { 300,  40, 2500 };
	EnemyPos_[6] = { 1500, 40, 2800 };
	EnemyPos_[7] = { 2800, 40, 2200 };
	EnemyPos_[8] = { 2400, 40, 2700 };
	EnemyPos_[9] = { 1000, 40, 1200 };

	// LARGE専用座標
	LargePos_.resize(2);

	LargePos_[0] = { 0, 40, 600 };
	LargePos_[1] = { 600, 40, 0 };
}

void EnemyManager::SpawnBossEnemy()
{
	// BOSS自身は追加生成しない
	// 通常敵だけを追加生成する

	EnemyBase::EnemyData data{};

	// 生成する敵の種類
	data.id = -1;
	data.type = EnemyBase::TYPE::RAT;
	data.hp = 3;
	data.wave = static_cast<int>(WAVE::BOSS);
	data.movableRange = 1000.0f;

	// ランダムな通常敵座標を取得
	int randIndex = GetRand(
		static_cast<int>(EnemyPos_.size()) - 1
	);

	data.defaultPos = EnemyPos_[randIndex];

	// 敵生成
	Create(data, player_);
}

void EnemyManager::SpawnNextEnemy()
{
	const EnemyBase::EnemyWave* waveData = nullptr;

	for (const auto& data : enemyWaveData_)
	{
		if (data.wave == static_cast<int>(wave_))
		{
			waveData = &data;
			break;
		}
	}

	if (waveData == nullptr)
	{
		return;
	}

	// 全敵生成済み
	if (spawnIndex_ >= waveData->enemies.size())
	{
		return;
	}

	// 今から生成する敵のtype
	int type = waveData->enemies[spawnIndex_];

	// Enemy.jsonからステータスを取得
	if (type < 0 || type >= enemyStatusData_.size())
	{
		return;
	}

	const auto& status = enemyStatusData_[type];

	// EnemyDataを作る
	EnemyBase::EnemyData data{};

	data.id = spawnIndex_;
	data.type = status.type;
	data.hp = status.hp;

	data.wave = static_cast<int>(wave_);

	data.movableRange = 1000.0f;

	// 座標
	int randIndex = GetRand(
		static_cast<int>(EnemyPos_.size()) - 1
	);

	data.defaultPos = EnemyPos_[randIndex];

	// 生成
	Create(data, player_);

	// 次の敵へ
	spawnIndex_++;
}



