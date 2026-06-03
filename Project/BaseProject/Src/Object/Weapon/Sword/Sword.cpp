#include "Sword.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBase.h"
#include "../../../Application.h"

Sword::Sword(Transform* ownerTransform)
	: WeaponBase(TYPE::SWORD, ownerTransform)
{
}

Sword::~Sword(void)
{
}

void Sword::Update(void)
{
	// 親クラスの更新処理
	WeaponBase::Update();
}

void Sword::InitHitboxes(void)
{
	// 基本パラメータ
	baseDamage_ = 10;
	attackRange_ = 120.0f;

	// プレイヤーモデルの剣のボーンフレーム番号を設定
	// 注: 実際のモデルに合わせて調整が必要
	weaponBaseFrameIndex_ = 14;  // 剣の柄（手元）のフレーム
	weaponTipFrameIndex_ = 15;   // 剣の先端のフレーム

	// 各攻撃タイプの判定を設定
	SetupNormalAttack1();
	SetupNormalAttack2();
	SetupNormalAttack3();
	SetupNormalAttack4();
	SetupNormalAttack5();
	SetupHeavyAttack();
	SetupDashAttack();
	SetupAirAttack1();
	SetupAirAttack2();
	SetupAirAttack3();
	SetupAirAttack4();
	SetupAirAttack5();
	SetupFallAttack();
}

void Sword::SetupNormalAttack1(void)
{
	// 通常攻撃1: 縦斬り
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 80.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 80.0f, 100.0f);
	hitbox.radius = 35.0f;
	hitbox.activeStartFrame = 10;   // アニメーション開始から10フレーム後
	hitbox.activeEndFrame = 20;     // 20フレームまで判定有効
	hitbox.damageMultiplier = 1.0f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::NORMAL1)].push_back(hitbox);
}

void Sword::SetupNormalAttack2(void)
{
	// 通常攻撃2: 横斬り（範囲広め）
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(-30.0f, 80.0f, 50.0f);
	hitbox.localPosEnd = VGet(30.0f, 80.0f, 50.0f);
	hitbox.radius = 40.0f;
	hitbox.activeStartFrame = 8;
	hitbox.activeEndFrame = 18;
	hitbox.damageMultiplier = 1.1f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::NORMAL2)].push_back(hitbox);
}

void Sword::SetupNormalAttack3(void)
{
	// 通常攻撃3: 斜め斬り上げ
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 60.0f, 30.0f);
	hitbox.localPosEnd = VGet(0.0f, 120.0f, 80.0f);
	hitbox.radius = 38.0f;
	hitbox.activeStartFrame = 9;
	hitbox.activeEndFrame = 19;
	hitbox.damageMultiplier = 1.15f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::NORMAL3)].push_back(hitbox);
}

void Sword::SetupNormalAttack4(void)
{
	// 通常攻撃4: 回転斬り（キック攻撃のため判定は足元）
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 40.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 40.0f, 60.0f);
	hitbox.radius = 42.0f;
	hitbox.activeStartFrame = 10;
	hitbox.activeEndFrame = 22;
	hitbox.damageMultiplier = 1.2f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::NORMAL4)].push_back(hitbox);
}

void Sword::SetupNormalAttack5(void)
{
	// 通常攻撃5: フィニッシュ斬り（強力）
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 90.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 90.0f, 130.0f);
	hitbox.radius = 45.0f;
	hitbox.activeStartFrame = 12;
	hitbox.activeEndFrame = 25;
	hitbox.damageMultiplier = 1.5f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::NORMAL5)].push_back(hitbox);
}

void Sword::SetupHeavyAttack(void)
{
	// 強攻撃: 突進斬り（範囲広く長時間）
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 80.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 80.0f, 150.0f);
	hitbox.radius = 50.0f;
	hitbox.activeStartFrame = 15;
	hitbox.activeEndFrame = 35;
	hitbox.damageMultiplier = 2.0f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::HEAVY)].push_back(hitbox);
}

void Sword::SetupDashAttack(void)
{
	// ダッシュ攻撃: スライディング斬り（低めの位置）
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 50.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 50.0f, 100.0f);
	hitbox.radius = 40.0f;
	hitbox.activeStartFrame = 5;
	hitbox.activeEndFrame = 20;
	hitbox.damageMultiplier = 1.3f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::DASH)].push_back(hitbox);
}

void Sword::SetupAirAttack1(void)
{
	// 空中攻撃1
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 80.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 80.0f, 90.0f);
	hitbox.radius = 35.0f;
	hitbox.activeStartFrame = 8;
	hitbox.activeEndFrame = 18;
	hitbox.damageMultiplier = 1.0f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::AIR1)].push_back(hitbox);
}

void Sword::SetupAirAttack2(void)
{
	// 空中攻撃2
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(-20.0f, 80.0f, 40.0f);
	hitbox.localPosEnd = VGet(20.0f, 80.0f, 40.0f);
	hitbox.radius = 38.0f;
	hitbox.activeStartFrame = 7;
	hitbox.activeEndFrame = 17;
	hitbox.damageMultiplier = 1.1f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::AIR2)].push_back(hitbox);
}

void Sword::SetupAirAttack3(void)
{
	// 空中攻撃3
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 70.0f, 30.0f);
	hitbox.localPosEnd = VGet(0.0f, 110.0f, 70.0f);
	hitbox.radius = 40.0f;
	hitbox.activeStartFrame = 9;
	hitbox.activeEndFrame = 19;
	hitbox.damageMultiplier = 1.15f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::AIR3)].push_back(hitbox);
}

void Sword::SetupAirAttack4(void)
{
	// 空中攻撃4
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 80.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 80.0f, 100.0f);
	hitbox.radius = 42.0f;
	hitbox.activeStartFrame = 8;
	hitbox.activeEndFrame = 20;
	hitbox.damageMultiplier = 1.2f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::AIR4)].push_back(hitbox);
}

void Sword::SetupAirAttack5(void)
{
	// 空中攻撃5: フィニッシュ
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 90.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, 90.0f, 120.0f);
	hitbox.radius = 45.0f;
	hitbox.activeStartFrame = 10;
	hitbox.activeEndFrame = 23;
	hitbox.damageMultiplier = 1.4f;
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::AIR5)].push_back(hitbox);
}

void Sword::SetupFallAttack(void)
{
	// 落下攻撃: 下方向への強力な一撃
	HitboxInfo hitbox;
	hitbox.localPosStart = VGet(0.0f, 100.0f, 0.0f);
	hitbox.localPosEnd = VGet(0.0f, -50.0f, 0.0f);  // 下方向
	hitbox.radius = 55.0f;
	hitbox.activeStartFrame = 12;
	hitbox.activeEndFrame = 30;
	hitbox.damageMultiplier = 2.5f;  // 最強の攻撃
	hitbox.isActive = false;
	hitboxInfoMap_[static_cast<int>(ATTACK_TYPE::FALL)].push_back(hitbox);
}