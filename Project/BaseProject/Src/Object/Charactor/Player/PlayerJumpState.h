#pragma once
#include "PlayerState.h"

class PlayerJumpState : public PlayerState
{
	public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void CollisionReserve(Player* player) override;
};

