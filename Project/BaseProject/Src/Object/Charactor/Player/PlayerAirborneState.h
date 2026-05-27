#pragma once
#include "PlayerState.h"

// ‹ó’†ó‘Ô‚ÌŠî’êƒNƒ‰ƒX
class PlayerAirborneState : public PlayerState
{
public:
	void Update(Player* player) override;
	void CollisionReserve(Player* player) override;
};