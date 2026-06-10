#include <string>
#include <fstream>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Scene/GameScene.h"
#include "Rat/EnemyRat.h"
#include "Robot/EnemyRobot.h"
#include "Rase/EnemyRase.h"
#include "../../Item/ItemBase.h"
#include "../../Item/ItemManger.h"
#include "../../Charactor/Player/Player.h"
#include "../../Actor/ActorBase.h"
#include "../../Collider/Sphere/ColliderSphere.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
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
			VECTOR hpPos = enemy->GetTransform().pos;

			int randNum = rand() % 500;

			hpPos.x += randNum;

			VECTOR skillPos = enemy->GetTransform().pos;

			randNum = rand() % 500;

			skillPos.z += randNum;


			gameScene_->GetItemManger()->Create(ItemBase::TYPE::HP, hpPos, hitCollider_,
				static_cast<int>(Player::COLLIDER_TYPE::CAPSULE), player_);


			enemy->SetAlive(false);
		}
	}
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

	// 攻撃コライダとの衝突チェック
	CheckAttackCollision();
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
		Create(data, player_);
	}
	ifs.close();
}

EnemyBase* EnemyManager::Create(const EnemyBase::EnemyData& data, const Player* player)
{
	EnemyBase* enemy = nullptr;
	switch (data.type)
	{
	case EnemyBase::TYPE::RAT:
		enemy = new EnemyRat(data, const_cast<Player*>(player));
		break;
	case EnemyBase::TYPE::RASE:
		enemy = new EnemyRase(data, const_cast<Player*>(player));
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

bool EnemyManager::GetEnemyDead(void)
{
	return isDead_;
}

void EnemyManager::AddAttackCollider(const ColliderBase* attackCollider)
{
	if (attackCollider != nullptr)
	{
		attackColliders_.push_back(attackCollider);
	}
}

void EnemyManager::ClearAttackColliders(void)
{
	attackColliders_.clear();
}

void EnemyManager::CheckAttackCollision(void)
{
	// 攻撃コライダが登録されていない場合は処理しない
	if (attackColliders_.empty())
	{
		return;
	}

	for (auto& enemy : enemies_)
	{
		// 死亡している敵はスキップ
		if (!enemy->IsAlive())
		{
			continue;
		}

		// 敵のコライダを取得(CAPSULEタイプを想定)
		const auto& enemyColliders = enemy->GetOwnColliders();
		const ColliderBase* enemyCollider = nullptr;

		// 敵のカプセルコライダを検索
		for (const auto& [key, collider] : enemyColliders)
		{
			if (collider->GetShape() == ColliderBase::SHAPE::CAPSULE)
			{
				enemyCollider = collider;
				break;
			}
		}

		// 敵のコライダが見つからない場合はスキップ
		if (enemyCollider == nullptr)
		{
			continue;
		}

		// 登録された攻撃コライダとの衝突判定
		for (const auto* attackCollider : attackColliders_)
		{
			if (attackCollider == nullptr)
			{
				continue;
			}

			// 攻撃コライダのタイプに応じた当たり判定
			bool isHit = false;

			// 球体同士の当たり判定
			if (attackCollider->GetShape() == ColliderBase::SHAPE::SPHERE &&
				enemyCollider->GetShape() == ColliderBase::SHAPE::CAPSULE)
			{
				const ColliderSphere* sphere = dynamic_cast<const ColliderSphere*>(attackCollider);
				const ColliderCapsule* capsule = dynamic_cast<const ColliderCapsule*>(enemyCollider);

				if (sphere != nullptr && capsule != nullptr)
				{
					// 球体とカプセルの当たり判定
					VECTOR spherePos = sphere->GetFollow()->pos;
					float sphereRadius = sphere->GetRadius();

					VECTOR capsuleTop = capsule->GetPosTop();
					VECTOR capsuleDown = capsule->GetPosDown();
					float capsuleRadius = capsule->GetRadius();

					// カプセルの線分と球体の最近接点を計算
					VECTOR capsuleVec = VSub(capsuleTop, capsuleDown);
					VECTOR toSphere = VSub(spherePos, capsuleDown);
					float t = VDot(toSphere, capsuleVec) / VDot(capsuleVec, capsuleVec);
					t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

					VECTOR closestPoint = VAdd(capsuleDown, VScale(capsuleVec, t));
					float distance = VSize(VSub(spherePos, closestPoint));

					isHit = (distance < sphereRadius + capsuleRadius);
				}
			}

			// ヒットした場合、ダメージ処理
			if (isHit)
			{
				// プレイヤーの攻撃力を取得してダメージを与える
				// ここではデフォルト値として10を使用
				int damage = 10;
				if (player_ != nullptr)
				{
					// プレイヤーのステータスから攻撃力を取得する場合
					// damage = player_->GetStatus().physAtk;
				}

				enemy->Damege(damage);
			}
		}
	}
}

