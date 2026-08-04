#pragma once
#include "PlayerState.h"

class PlayerDamageState : public PlayerState
{
	virtual void Enter(Player* player);
	virtual void Update(Player* player);
};
