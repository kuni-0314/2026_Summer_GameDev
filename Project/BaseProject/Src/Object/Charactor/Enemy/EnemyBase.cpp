#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../Charactor/Player/Player.h"
#include "../Enemy/Shot/ShotBase.h"
#include "../Enemy/Shot/ShotStraight.h"
#include "../../Collider/ColliderBase.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Sound/AudioManager.h"
#include "../../../Effect/LoadEffekseer/EffekseerEffect.h"
#include "../../../Effect/EffectManager.h"
#include "../../../Common/Quaternion.h"
#include "EnemyBase.h"
#include "../../Collider/Sphere/ColliderSphere.h"


EnemyBase::EnemyBase(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:
CharactorBase(),
player_(player),
type_(data.type),
defaultPos_(data.defaultPos),
movableRange_(data.movableRange),
wave_(data.wave),
attackModle_(attackModel)
{

	// 初期座標の設定
	transform_.pos = data.defaultPos;

	hp_ = data.hp;
}

EnemyBase::~EnemyBase(void)
{

}

void EnemyBase::Update()
{
	CharactorBase::Update();
}

void EnemyBase::Draw()
{
	CharactorBase::Draw();

#ifdef _DEBUG
	for (auto col : hitColliders_)
	{
		if (col->GetTag() != ColliderBase::TAG::PLAYER_MAGIC) continue;
		auto colSphere = dynamic_cast<const ColliderSphere*>(col);
		DrawSphere3D(colSphere->GetPos(), colSphere->GetRadius(), 16, GetRand(0xffffff), GetColor(0, 255, 0), false);
	}


#endif
}

void EnemyBase::Release(void)
{
	CharactorBase::Release();
}

void EnemyBase::HitEffect(const VECTOR& pos, const VECTOR& normal, float size)
{
	//エフェクトの読み込み
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Star/Star.efkefc",
		transform_.pos
	);

	effect->Play(
		pos,
		Quaternion::LookRotation(normal)
	);

	//エフェクトの再生
	EffectManager::GetInstance().RegisterEffect(effect);
}

void EnemyBase::HitThunderEffect(const VECTOR& pos, const VECTOR& normal, float size)
{
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Thunder/N.efkefc", // hitエフェクト
		pos
	);

	effect->Play(
		pos,
		Quaternion::LookRotation(normal)
	);

	effect->SetScale(size * 30.0f);   // 必要なら
	effect->SetLifeTime(30);

	EffectManager::GetInstance().RegisterEffect(effect);
}

bool EnemyBase::InMovableRange(void) const
{
	bool ret = false;
	// 初期位置からの距離
	float dis = static_cast<float>(
		AsoUtility::SqrMagnitude(defaultPos_, transform_.pos));
	// 指定距離判定
	if (dis < movableRange_ * movableRange_)
	{
		return true;
	}
	return ret;

}

void EnemyBase::LookPlayer()
{

	VECTOR playerPos = player_->GetPos();

	//ベクトル計算
	VECTOR diff = VSub(playerPos, transform_.pos);
	diff.y = 0.0f;

	//ベクトルの正規化で単位ベクトルの取得
	moveDir_ = VNorm(diff);

	transform_.rot.y = atan2(moveDir_.x, moveDir_.z);
	transform_.rot.y += AsoUtility::Deg2RadF(180.0f);

	//回転はY軸のみにする
	transform_.rot.x = transform_.rot.z = 0.0f;

	MV1SetRotationXYZ(transform_.modelId, transform_.rot);
}

bool EnemyBase::PushOutSphere(
	VECTOR& posA,
	float radiusA,
	const VECTOR& posB,
	float radiusB,
	bool debugDraw)
{
#ifdef _DEBUG
	if (debugDraw)
	{
		// Aの球（赤）
		DrawSphere3D(posA,radiusA,16,GetColor(255, 0, 0),GetColor(255, 0, 0),FALSE);
		// Bの球（青）
		DrawSphere3D(posB,radiusB,16,GetColor(0, 0, 255),GetColor(0, 0, 255),FALSE);
		// 中心を結ぶ線（緑）
		DrawLine3D(posA,posB,GetColor(0, 255, 0));
	}
#endif

	VECTOR diff = VSub(posA, posB);

	float distance = VSize(diff);
	float minDistance = radiusA + radiusB + 20.0f;

	if (distance <= 0.001f)
		return false;

	if (distance >= minDistance)
		return false;

	VECTOR dir = VNorm(diff);

	float overlap = minDistance - distance;

	posA = VAdd(posA, VScale(dir, overlap));

	return true;
}




void EnemyBase::ChangeState(int state)
{
	stateBase_ = state;


	auto it = stateChanges_.find(stateBase_);

	if (it != stateChanges_.end())
	{
		// 登録された初期処理を実行（EnemyRat::ChangeStateXXX が stateUpdate_ を設定する）
		it->second();
	}
	else
	{
		// マッピングがない場合は明示的に空にする（保険）
		stateUpdate_ = std::function<void()>{};
	}
}

float EnemyBase::GetCollRadius()
{
	return pushOutRadius_;
}

VECTOR& EnemyBase::GetPos()
{
	return transform_.pos;
}



void EnemyBase::CheckPlayerSwordCollision()
{
	// 死亡状態なら処理しない
	if (!isAlive_) return;

	// プレイヤーが剣を振っていない場合は処理しない
	if (!player_->IsAttacking()) 
	{
		wasHitSword_ = false; // 攻撃していないなら被ダメージフラグをリセット
		return;
	}

	if (wasHitSword_) return;


	// 自身のカプセルコライダを取得
	ColliderCapsule* ownColCapsule = nullptr;
	for (const auto& ownCol : ownColliders_)
	{
		if (ownCol.second->GetTag() == ColliderBase::TAG::ENEMY)
		{
			ownColCapsule =
				dynamic_cast<ColliderCapsule*>(ownCol.second);
			//if (ownColCapsule == nullptr) return;
		}
	}

	// プレイヤーの剣コライダはhitColliders_に登録されているはずなので、全てチェック
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() == ColliderBase::TAG::PLAYER_SWORD)
		{
			// 剣はカプセルコライダ
			// 敵もカプセルコライダ
			// カプセルコライダ同士で衝突判定
			const ColliderCapsule* swordColCapsule =
				dynamic_cast<const ColliderCapsule*>(hitCol);

			if (swordColCapsule == nullptr) return;

			// 衝突判定
			if (ownColCapsule->IsHit(swordColCapsule))
			{
				// ダメージ処理
				Damage(1, player_->GetTransform().GetForward());

				AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_HIT);

				// エフェクト再生
				VECTOR hitPos = VAdd(
					ownColCapsule->GetCenter(),
					swordColCapsule->GetCenter());

				hitPos = VScale(hitPos, 0.5f);

				HitEffect(hitPos, VNorm(VSub(hitPos, transform_.pos)), 1.0f);

				// 一度あったらフラグ
				wasHitSword_ = true;

				InputManager::GetInstance()->VibrateGamepad(1, 500, 100);
			}
		}
	}


}

void EnemyBase::CheckPlayerMagicCollision()
{
	// 死亡状態なら処理しない
	if (!isAlive_) return;

	// プレイヤーの魔法が残存していないなら処理しない
	if (!player_->IsAliveMagic())
	{
		wasHitMagic_ = false; // 残存していないなら被ダメージフラグをリセット
		return;
	}

	if (wasHitMagic_) return;

	// 自身のカプセルコライダを取得
	ColliderCapsule* ownColCapsule = nullptr;
	for (const auto& ownCol : ownColliders_)
	{
		if (ownCol.second->GetTag() == ColliderBase::TAG::ENEMY)
		{
			ownColCapsule = dynamic_cast<ColliderCapsule*>(ownCol.second);
		}
	}

	if (ownColCapsule == nullptr) return;

	// プレイヤーの雷魔法コライダをチェック
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() == ColliderBase::TAG::PLAYER_MAGIC)
		{
			// 雷魔法は球体コライダ
			const ColliderSphere* magicColSphere =
				dynamic_cast<const ColliderSphere*>(hitCol);

			if (magicColSphere == nullptr) continue;

			VECTOR magicSpherePos = magicColSphere->GetPos();
			float magicSphereRadius = magicColSphere->GetRadius();
			VECTOR capsuleTop = ownColCapsule->GetPosTop();
			VECTOR capsuleBottom = ownColCapsule->GetPosDown();
			float capsuleRadius = ownColCapsule->GetRadius();

			// カプセルと球体の衝突判定
			if (HitCheck_Sphere_Capsule(
				magicSpherePos,
				magicSphereRadius,
				capsuleTop,
				capsuleBottom,
				capsuleRadius) == true)
			{
				// ダメージ処理
				//Damage(1);
				Damage(1, player_->GetTransform().GetForward());


				AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_HIT);

				// エフェクト再生
				VECTOR hitPos = VAdd(ownColCapsule->GetCenter(), magicColSphere->GetPos());
				hitPos = VScale(hitPos, 0.5f);

				HitThunderEffect(hitPos, VNorm(VSub(hitPos, transform_.pos)), 1.0f);

				// 一度あったらフラグ
				wasHitMagic_ = true;
				RemoveHitCollider(hitCol);
			}
		}
	}
}
void EnemyBase::CheckEnemy()
{
}

