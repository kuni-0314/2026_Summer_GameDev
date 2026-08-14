#pragma once
#include "PlayerState.h"

class PlayerRunState : public PlayerState
{
	void Enter(Player* player) override;
	void Update(Player* player) override;
};

