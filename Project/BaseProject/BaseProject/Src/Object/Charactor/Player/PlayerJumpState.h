#pragma once
#include "PlayerAirborneState.h"

class PlayerJumpState : public PlayerAirborneState
{
public:
	void Enter(Player* player) override;
};

