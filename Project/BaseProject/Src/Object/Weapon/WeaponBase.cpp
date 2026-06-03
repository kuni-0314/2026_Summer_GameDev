#include "WeaponBase.h"
#include "../Common/Transform.h"
#include "../Collider/ColliderBase.h"

WeaponBase::WeaponBase(TYPE type, Transform* ownerTransform)
	: ActorBase()
	, type_(type)
	, ownerTransform_(ownerTransform)
	, baseDamage_(0)
	, attackRange_(0.0f)
	, isAttacking_(false)
	, currentAttackType_(-1)
	, currentFrame_(0)
	, weaponBaseFrameIndex_(-1)
	, weaponTipFrameIndex_(-1)
{
}

WeaponBase::~WeaponBase(void)
{
}

void WeaponBase::InitLoad(void)
{
}

void WeaponBase::InitTransform(void)
{
	transform_.pos = ownerTransform_->pos;
	transform_.rot = ownerTransform_->rot;
	transform_.scl = ownerTransform_->scl;
}

void WeaponBase::InitCollider(void)
{
}

void WeaponBase::InitAnimation(void)
{
}

void WeaponBase::InitPost(void)
{
	// 攻撃判定情報の初期化
	InitHitboxes();
}

void WeaponBase::Update(void)
{
	// 攻撃中の場合
	if (isAttacking_)
	{
		// フレームを進める
		currentFrame_++;

		// 攻撃判定の更新
		UpdateHitboxes();
	}

	// 武器の座標を更新（ボーンに追従）
	UpdateTransformWeapon();

	// Transform の更新
	transform_.Update();
}

void WeaponBase::Draw(void)
{
	ActorBase::Draw();

	// デバッグ描画
	#ifdef _DEBUG
	DrawDebug();
	#endif
}

void WeaponBase::BeginAttack(int attackType)
{
	// 攻撃開始
	isAttacking_ = true;
	currentAttackType_ = attackType;
	currentFrame_ = 0;

	// 攻撃判定情報をクリア
	activeHitboxes_.clear();
}

void WeaponBase::EndAttack(void)
{
	// 攻撃終了
	isAttacking_ = false;
	currentAttackType_ = -1;
	currentFrame_ = 0;

	// アクティブな判定をクリア
	for (auto& hitbox : activeHitboxes_)
	{
		hitbox.isActive = false;
	}
	activeHitboxes_.clear();
}

const std::vector<WeaponBase::HitboxInfo>& WeaponBase::GetActiveHitboxes(void) const
{
	return activeHitboxes_;
}

VECTOR WeaponBase::GetWeaponTipPos(void) const
{
	// 武器先端のボーン座標を取得
	if (weaponTipFrameIndex_ >= 0 && ownerTransform_ != nullptr)
	{
		MATRIX tipMatrix = MV1GetFrameLocalWorldMatrix(
			ownerTransform_->modelId,
			weaponTipFrameIndex_
		);
		return VGet(tipMatrix.m[3][0], tipMatrix.m[3][1], tipMatrix.m[3][2]);
	}

	// ボーン情報がない場合は所有者の前方に配置
	if (ownerTransform_ != nullptr)
	{
		VECTOR forward = ownerTransform_->GetForward();
		return VAdd(ownerTransform_->pos, VScale(forward, attackRange_));
	}

	return VGet(0.0f, 0.0f, 0.0f);
}

VECTOR WeaponBase::GetWeaponBasePos(void) const
{
	// 武器根元のボーン座標を取得
	if (weaponBaseFrameIndex_ >= 0 && ownerTransform_ != nullptr)
	{
		MATRIX baseMatrix = MV1GetFrameLocalWorldMatrix(
			ownerTransform_->modelId,
			weaponBaseFrameIndex_
		);
		return VGet(baseMatrix.m[3][0], baseMatrix.m[3][1], baseMatrix.m[3][2]);
	}

	// ボーン情報がない場合は所有者の位置を返す
	if (ownerTransform_ != nullptr)
	{
		return ownerTransform_->pos;
	}

	return VGet(0.0f, 0.0f, 0.0f);
}

void WeaponBase::UpdateHitboxes(void)
{
	// 現在の攻撃タイプに対応する判定情報を取得
	if (hitboxInfoMap_.find(currentAttackType_) == hitboxInfoMap_.end())
	{
		return;
	}

	// アクティブな判定をクリア
	activeHitboxes_.clear();

	// 各判定情報をチェック
	auto& hitboxList = hitboxInfoMap_[currentAttackType_];
	for (auto& hitbox : hitboxList)
	{
		// フレームが範囲内ならアクティブ化
		if (currentFrame_ >= hitbox.activeStartFrame &&
			currentFrame_ <= hitbox.activeEndFrame)
		{
			// ローカル座標をワールド座標に変換
			HitboxInfo worldHitbox = hitbox;
			
			if (ownerTransform_ != nullptr)
			{
				// 所有者の回転を適用
				MATRIX rotMat = MGetRotY(ownerTransform_->rot.y);
				worldHitbox.localPosStart = VTransform(hitbox.localPosStart, rotMat);
				worldHitbox.localPosEnd = VTransform(hitbox.localPosEnd, rotMat);

				// 所有者の位置を加算
				worldHitbox.localPosStart = VAdd(worldHitbox.localPosStart, ownerTransform_->pos);
				worldHitbox.localPosEnd = VAdd(worldHitbox.localPosEnd, ownerTransform_->pos);
			}

			worldHitbox.isActive = true;
			activeHitboxes_.push_back(worldHitbox);
		}
	}
}

void WeaponBase::UpdateTransformWeapon(void)
{
	// 武器が独立モデルの場合、所有者に追従させる
	if (transform_.modelId != -1 && ownerTransform_ != nullptr)
	{
		transform_.pos = ownerTransform_->pos;
		transform_.rot = ownerTransform_->rot;
	}
}

void WeaponBase::DrawDebug(void)
{
	// アクティブな攻撃判定を描画
	for (const auto& hitbox : activeHitboxes_)
	{
		if (hitbox.isActive)
		{
			// カプセル状の判定を球体で表現
			DrawSphere3D(hitbox.localPosStart, hitbox.radius, 8, 0xFF0000, 0xFF0000, false);
			DrawSphere3D(hitbox.localPosEnd, hitbox.radius, 8, 0xFF0000, 0xFF0000, false);
			DrawLine3D(hitbox.localPosStart, hitbox.localPosEnd, 0xFF0000);
		}
	}

	// 武器の先端と根元を描画
	VECTOR tipPos = GetWeaponTipPos();
	VECTOR basePos = GetWeaponBasePos();
	DrawSphere3D(tipPos, 5.0f, 8, 0x00FF00, 0x00FF00, true);
	DrawSphere3D(basePos, 5.0f, 8, 0x0000FF, 0x0000FF, true);
	DrawLine3D(basePos, tipPos, 0xFFFF00);
}
