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
	//開始ウェーブ
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
	else if (next == WAVE::WAVE4) name = "WAVE4";
	else if (next == WAVE::BOSS) name = "BOSS";

	DrawFormatString(0, 100, GetColor(255, 255, 255), "WAVE: %s", name);

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
		enemy->Release();
		delete enemy;
		enemy = nullptr;
	}
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

void EnemyManager::LoadCsvData()
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

		//生成ウェーブ
		data.wave = stoi(strSplit[idx++]);

		if (data.wave == static_cast<int>(wave_))
		{
			EnemyBase* newEnemy = Create(data, player_);

		}
	}
	ifs.close();
}

EnemyBase* EnemyManager::Create(const EnemyBase::EnemyData& data, const Player* player)
{
	
	EnemyBase* enemy = nullptr;
	switch (data.type)
	{
	case EnemyBase::TYPE::RAT:
		enemy = new EnemyRat(data,-1, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::RASE:
		enemy = new EnemyRase(data, -1, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::LARGE:
		enemy = new EnemyLarge(data, -1, const_cast<Player*>(player));
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
	case EnemyManager::WAVE::START:
		break;
	case EnemyManager::WAVE::WAVE1:
		break;
	case EnemyManager::WAVE::WAVE2:
		break;
	case EnemyManager::WAVE::WAVE3:
		break;
	case EnemyManager::WAVE::WAVE4:
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

	//WAVE切り替え
	ChangeWave(wave_);
	//敵生成
	LoadCsvData();
}


void EnemyManager::UpdateWave()
{
	switch (wave_)
	{
	case EnemyManager::WAVE::START:
		UpdateWaveStart();
		break;
	case EnemyManager::WAVE::WAVE1:
		UpdateWave1();
		break;
	case EnemyManager::WAVE::WAVE2:
		UpdateWave2();
		break;
	case EnemyManager::WAVE::WAVE3:
		UpdateWave3();
		break;
	case EnemyManager::WAVE::WAVE4:
		UpdateWave4();
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



void EnemyManager::UpdateWaveStart()
{
}

void EnemyManager::UpdateWave1()
{
	CreateHpItem();
	EnemysDelete();

	//エネミー全滅フラグ
	wave1Clear_ = true;
	for (const auto enemy : enemies_)
	{
		if (enemy->IsAlive())
		{
			wave1Clear_ = false;
			break;
		}
	}

	//wave1クリア時にwave2の敵を生成
	if (wave1Clear_)
	{
		LoadWaveData(WAVE::WAVE2);
	}
}

void EnemyManager::UpdateWave2()
{
	CreateHpItem();
	EnemysDelete();

	//エネミー全滅フラグ
	wave2Clear_ = true;
	for (const auto enemy : enemies_)
	{
		if (enemy->IsAlive())
		{
			wave2Clear_ = false;
			break;
		}
	}

	//wave1クリア時にwave2の敵を生成
	if (wave2Clear_)
	{
		LoadWaveData(WAVE::WAVE3);
	}
}

void EnemyManager::UpdateWave3()
{
	CreateHpItem();
	EnemysDelete();

	//エネミー全滅フラグ
	wave3Clear_ = true;
	for (const auto enemy : enemies_)
	{
		if (enemy->IsAlive())
		{
			wave3Clear_ = false;
			break;
		}
	}

	//wave1クリア時にwave2の敵を生成
	if (wave3Clear_)
	{
		LoadWaveData(WAVE::WAVE4);
	}
}

void EnemyManager::UpdateWave4()
{
	CreateHpItem();
	EnemysDelete();

	//エネミー全滅フラグ
	wave4Clear_ = true;
	for (const auto enemy : enemies_)
	{
		if (enemy->IsAlive())
		{
			wave4Clear_ = false;
			break;
		}
	}

	//wave1クリア時にwave2の敵を生成
	if (wave4Clear_)
	{
		LoadWaveData(WAVE::BOSS);
	}
}

void EnemyManager::UpdateWaveBoss()
{
	CreateHpItem();
	EnemysDelete();

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
