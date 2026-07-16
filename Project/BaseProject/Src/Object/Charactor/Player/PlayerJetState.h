#pragma once
#include "PlayerState.h"

class PlayerRollState : public PlayerState
{
	public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Exit(Player* player) override;
};

