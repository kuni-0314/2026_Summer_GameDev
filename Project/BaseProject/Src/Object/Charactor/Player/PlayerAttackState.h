#pragma once
#include "PlayerState.h"

class PlayerAttackState : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
};
