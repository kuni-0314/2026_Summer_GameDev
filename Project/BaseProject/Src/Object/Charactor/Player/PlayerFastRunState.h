#pragma once
#include "PlayerState.h"

class PlayerFastRunState : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player) override;
};

