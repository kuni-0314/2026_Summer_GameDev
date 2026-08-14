#pragma once
#include "PlayerAirborneState.h"

class PlayerFallState : public PlayerAirborneState
{
public:

	void Enter(Player* player) override;
	void Update(Player* player) override;

};