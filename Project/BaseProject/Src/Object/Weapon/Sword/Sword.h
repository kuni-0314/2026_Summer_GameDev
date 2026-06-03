#pragma once
#include "../WeaponBase.h"

class Sword : public WeaponBase
{
public:
	// コンストラクタ
	Sword(Transform* ownerTransform);
	// デストラクタ
	~Sword(void) override;

	// 更新
	void Update(void) override;

protected:
	// 武器固有の初期化
	void InitHitboxes(void) override;

private:
	// 攻撃タイプ（PlayerAttackState::ATTACK_TYPE と対応）
	enum class ATTACK_TYPE
	{
		NORMAL1 = 0,    // 通常攻撃1
		NORMAL2 = 1,    // 通常攻撃2
		NORMAL3 = 2,    // 通常攻撃3
		NORMAL4 = 3,    // 通常攻撃4
		NORMAL5 = 4,    // 通常攻撃5
		HEAVY = 5,      // 強攻撃
		DASH = 6,       // ダッシュ攻撃
		AIR1 = 7,       // 空中攻撃1
		AIR2 = 8,       // 空中攻撃2
		AIR3 = 9,       // 空中攻撃3
		AIR4 = 10,      // 空中攻撃4
		AIR5 = 11,      // 空中攻撃5
		FALL = 12,      // 落下攻撃
		MAX
	};

	// 各攻撃タイプの判定を設定
	void SetupNormalAttack1(void);
	void SetupNormalAttack2(void);
	void SetupNormalAttack3(void);
	void SetupNormalAttack4(void);
	void SetupNormalAttack5(void);
	void SetupHeavyAttack(void);
	void SetupDashAttack(void);
	void SetupAirAttack1(void);
	void SetupAirAttack2(void);
	void SetupAirAttack3(void);
	void SetupAirAttack4(void);
	void SetupAirAttack5(void);
	void SetupFallAttack(void);
};