#include "PlayerState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"

bool PlayerState::CheckTransitions(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 入力デバイスの種類を判定
	bool isGamepadConnected = (GetJoypadNum() > 0);
	constexpr int GAMEPAD_INDEX = 0;

	// 攻撃入力のチェック（クールタイム無しの時）
	bool isAttackInput = false;
	if (isGamepadConnected)
	{
		// ゲームパッド：Xボタン
		isAttackInput = ins->IsGamepadTrgUp(InputManager::PadInput::X, GAMEPAD_INDEX);
	}
	else
	{
		// キーボード：マウス左ボタン
		isAttackInput = ins->IsMouseTrgUp(MOUSE_INPUT_LEFT);
	}

	if (isAttackInput && player->GetAttackCoolTime() <= 0)
	{
		player->ChangeState(Player::STATE::ATTACK);
		return true;
	}

	// ジェット入力のチェック
	bool isJetInput = false;
	if (isGamepadConnected)
	{
		// ゲームパッド：Bボタン
		isJetInput = ins->IsGamepadTrgDown(InputManager::PadInput::B, GAMEPAD_INDEX);
	}
	else
	{
		// キーボード：Eキー
		isJetInput = ins->IsTrgDown(KEY_INPUT_E);
	}

	if (isJetInput)
	{
		player->ChangeState(Player::STATE::JET);
		return true;
	}

	// ジャンプ入力のチェック
	bool isJumpInput = false;
	if (isGamepadConnected)
	{
		// ゲームパッド：Aボタン（ダッシュと兼用）
		// 地上にいる時のみジャンプ可能
		if (!player->IsAir() && !player->IsJump())
		{
			isJumpInput = ins->IsGamepadTrgDown(InputManager::PadInput::A, GAMEPAD_INDEX);
		}
	}
	else
	{
		// キーボード：スペースキー
		isJumpInput = ins->IsTrgDown(KEY_INPUT_SPACE);
	}

	if (!player->IsAir() && !player->IsJump() && isJumpInput)
	{
		player->ChangeState(Player::STATE::JUMP);
		return true;
	}

	return false;
}
