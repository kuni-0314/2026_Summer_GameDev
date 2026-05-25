#include "PlayerState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"

bool PlayerState::CheckTransitions(Player* player)
{
	auto ins = InputManager::GetInstance();

	// ジェットキーが入力されているか
	if (ins->IsTrgDown(KEY_INPUT_E))
	{
		player->ChangeState(Player::STATE::JET);
		return true;
	}

	// ジャンプキーが入力されているか
	if (!player->IsAir() && !player->IsJump() && ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		player->ChangeState(Player::STATE::JUMP);
		return true;
	}

	bool isTrgUp = ins->IsMouseTrgUp(MOUSE_INPUT_LEFT);

	if (!player->IsJet() && isTrgUp)
	{
		player->ChangeState(Player::STATE::ATTACK);
		return true;
	}

	return false;
}
