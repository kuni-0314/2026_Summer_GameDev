#pragma once
#include "PlayerState.h"

class PlayerMagicState : public PlayerState
{
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Draw(Player* player) override;
	void Exit(Player* player) override;

	enum class MAGIC_TYPE
	{
		FIRE,
		THUNDER,
		HEAL,
		MAX
	};

	static constexpr float MAGIC_POW[static_cast<int>(MAGIC_TYPE::MAX)] =
	{
		10.0f,	// FIRE
		5.0f,	// THUNDER
		10.0f,	// HEAL
	};

private:
	// –‚–@‚ÌŽí—Þ
	MAGIC_TYPE magicType_;
};

