#include "../../../Manager/InputManager.h"
#include "Player.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Utility/AsoUtility.h"
#include "PlayerIdleState.h"

void PlayerIdleState::Enter(Player* player)
{
	// 初期アニメーション再生
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::IDLE), true);

	// 慣性を採用のためコメントアウト
	// 移動量をゼロにする
	//VECTOR movePow = {0, 0, 0};
	//player->SetMovePow(movePow);
}

void PlayerIdleState::Update(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 慣性による移動量の減衰
	if (!AsoUtility::EqualsVZero(player->GetMovePow()))
	{
		VECTOR movePow = player->GetMovePow();

		// 地上
		movePow = VScale(movePow, Player::GROUND_MOVE_DEC_RATE);

		// 十分小さくなったら完全に停止
		if (VSize(movePow) < 0.01f)
		{
			movePow = { 0.0f, 0.0f, 0.0f };
		}

		player->SetMovePow(movePow);
	}

	// 遷移チェック
	if (CheckTransitions(player))
	{
		return;
	}

	// 入力デバイスの種類を判定
	bool isGamepadConnected = (GetJoypadNum() > 0);
	constexpr int GAMEPAD_INDEX = 0;

	// 移動入力のチェック
	bool hasMoveInput = false;
	bool isDashInput = false;

	if (isGamepadConnected)
	{
		// ゲームパッド：左スティックの入力チェック
		short stickX = 0, stickY = 0;
		ins->GetLeftStick(GAMEPAD_INDEX, stickX, stickY);

		constexpr float STICK_DEADZONE = 0.2f;
		constexpr float STICK_MAX = 32767.0f;
		float normalizedX = stickX / STICK_MAX;
		float normalizedY = stickY / STICK_MAX;

		hasMoveInput = (abs(normalizedX) > STICK_DEADZONE || abs(normalizedY) > STICK_DEADZONE);
		isDashInput = ins->IsGamepadNew(InputManager::PadInput::A, GAMEPAD_INDEX);
	}
	else
	{
		// キーボード：WASDキーの入力チェック
		hasMoveInput = (ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
		                ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D));
		isDashInput = ins->IsNew(KEY_INPUT_LSHIFT);
	}

	// 移動キーが入力されているか
	if (hasMoveInput)
	{
		// ダッシュキーが入力されているか
		if (isDashInput)
		{
			player->ChangeState(Player::STATE::FAST_RUN);
			return;
		}
		else
		{
			player->ChangeState(Player::STATE::RUN);
			return;
		}
	}
}
