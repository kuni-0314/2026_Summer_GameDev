#pragma once
#include "PlayerState.h"

class PlayerAttackState : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Draw(Player* player) override;

	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL1,
		NORMAL2,
		NORMAL3,
		NORMAL4,
		NORMAL5,
		HEAVY,
		DASH,
		AIR1,
		AIR2,
		AIR3,
		AIR4,
		AIR5,
		FALL,
		MAX
	};

	static constexpr float ATTACK_POW[static_cast<int>(ATTACK_TYPE::MAX)] =
	{	// tmp
		10.0f,	// NORMAL1
		12.0f,	// NORMAL2
		15.0f,	// NORMAL3
		20.0f,	// NORMAL4
		25.0f,	// NORMAL5
		30.0f,	// HEAVY
		18.0f,  // DASH
		8.0f,   // AIR1
		10.0f,  // AIR2
		12.0f,  // AIR3
		15.0f,  // AIR4
		20.0f,  // AIR5
		25.0f   // FALL
	};

	static constexpr float ATTACK_COOL_TIME[static_cast<int>(ATTACK_TYPE::MAX)]
	{// tmp
		15.0f,	// NORMAL1
		15.0f,	// NORMAL2
		15.0f,	// NORMAL3
		15.0f,	// NORMAL4
		15.0f,	// NORMAL5
		60.0f,	// HEAVY
		30.0f,  // DASH
		15.0f,  // AIR1
		15.0f,  // AIR2
		15.0f,  // AIR3
		15.0f,  // AIR4
		15.0f,  // AIR5
		60.0f   // FALL
	};

private:
	// 攻撃の種類
	ATTACK_TYPE attackType_;

	// クールタイム
	int coolTime_;

	// 命名未定
	bool tmp_;
};
