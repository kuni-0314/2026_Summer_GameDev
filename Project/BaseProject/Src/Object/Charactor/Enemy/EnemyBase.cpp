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
	// 移動可能範囲のデバッグ描画
	//DrawSphere3D(defaultPos_, 500.0f, 16, 0x000099, 0x000099, false);
#endif // _DEBUG
}

void EnemyBase::Release(void)
{
	CharactorBase::Release();
}

void EnemyBase::HitEffect(const VECTOR& pos, const VECTOR& normal, float size)
{
	//エフェクトの読み込み
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Ster/Ster.efkefc",
		transform_.pos
	);

	effect->Play(
		pos,
		Quaternion::LookRotation(normal)
	);

	//エフェクトの再生
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

void EnemyBase::CheckPlayerSwordCollision()
{
	// 死亡状態なら処理しない
	if (!isAlive_) return;

	// プレイヤーが剣を振っていない場合は処理しない
	if (!player_->IsAttacking()) 
	{
		wasHit_ = false; // 攻撃していないなら被ダメージフラグをリセット
		return;
	}

	if (wasHit_) return;


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
				Damege(1);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_HIT);

				// エフェクト再生
				VECTOR hitPos = VAdd(
					ownColCapsule->GetCenter(),
					swordColCapsule->GetCenter());

				hitPos = VScale(hitPos, 0.5f);

				HitEffect(hitPos, VNorm(VSub(hitPos, transform_.pos)), 1.0f);

				// 一度あったらフラグ
				wasHit_ = true;
			}
		}
	}


}

void EnemyBase::CheckEnemy()
{
}

