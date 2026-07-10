#include "PlayerState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"

bool PlayerState::CheckTransitions(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 入力デバイスの種類を判定
	bool enableKAM = ins->IsEnableKeyAndMouse();
	bool isGamepadConnected = (GetJoypadNum() > 0);

	// 攻撃入力のチェック（クールタイム無しの時）
	bool isAttackInput = false;
	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			isAttackInput = ins->IsGamepadTrgUp(InputManager::PadInput::A, player->GetPadNum());
			isAttackInput = isAttackInput || ins->IsMouseTrgUp(MOUSE_INPUT_LEFT);
		}
		else
		{
			isAttackInput = ins->IsGamepadTrgUp(InputManager::PadInput::A, player->GetPadNum());
		}
	}
	else
	{
		isAttackInput = ins->IsMouseTrgUp(MOUSE_INPUT_LEFT);
	}

	if (isAttackInput && player->GetAttackCoolTime() <= 0)
	{
		player->ChangeState(Player::STATE::ATTACK);
		//攻撃中判定
		player->SetAttacking(true);
		return true;
	}

	// ジェット入力のチェック
	bool isJetInput = false;
	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			isJetInput = ins->IsGamepadTrgDown(InputManager::PadInput::X, player->GetPadNum());
			isJetInput = isJetInput || ins->IsMouseTrgDown(MOUSE_INPUT_RIGHT);
		}
		else
		{
			isJetInput = ins->IsGamepadTrgDown(InputManager::PadInput::X, player->GetPadNum());
		}
	}
	else
	{
		isJetInput = ins->IsMouseTrgDown(MOUSE_INPUT_RIGHT);
	}

	if (isJetInput)
	{
		player->ChangeState(Player::STATE::JET);
		player->SetAttacking(false);
		return true;
	}

	// ジャンプ入力のチェック
	bool isJumpInput = false;
	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			isJumpInput = ins->IsGamepadTrgDown(InputManager::PadInput::B, player->GetPadNum());
			isJumpInput = isJumpInput || ins->IsTrgDown(KEY_INPUT_SPACE);
		}
		else
		{
			isJumpInput = ins->IsGamepadTrgDown(InputManager::PadInput::B, player->GetPadNum());
		}
	}
	else
	{
		isJumpInput = ins->IsTrgDown(KEY_INPUT_SPACE);
	}

	if (!player->IsAir() && !player->IsJump() && isJumpInput)
	{
		player->ChangeState(Player::STATE::JUMP);
		player->SetAttacking(false);
		return true;
	}

	return false;
}
