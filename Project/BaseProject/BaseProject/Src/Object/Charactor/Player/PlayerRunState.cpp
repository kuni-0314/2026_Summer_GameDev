#include <DxLib.h>
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Camera.h"
#include "player.h"
#include "../../Common/AnimationController.h"
#include "PlayerRunState.h"

void PlayerRunState::Enter(Player* player)
{
	auto ins = InputManager::GetInstance();

	// スティックの傾きを取得
	float leftStickX = ins->GetLeftStickX(player->GetPadNum());
	float leftStickY = ins->GetLeftStickY(player->GetPadNum());

	// 取得した傾きの値は-32768～32767の範囲なので、-1.0f～1.0fの範囲に正規化
	float normLeftStickX = leftStickX / 32768.0f;
	float normLeftStickY = leftStickY / 32768.0f;

	// スティックの傾きの大きさを計算
	float pow = VSize({ normLeftStickX, 0.0f, normLeftStickY });

	// 初期アニメーション再生
	if (pow > 0.85f)
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::FAST_RUN), true);
	}
	else
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::RUN), true);
	}
}

void PlayerRunState::Update(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 遷移チェック
	if (CheckTransitions(player))
	{
		return;
	}

	// スティックの傾きを取得
	float leftStickX = ins->GetLeftStickX(player->GetPadNum());
	float leftStickY = ins->GetLeftStickY(player->GetPadNum());

	// 取得した傾きの値は-32768～32767の範囲なので、-1.0f～1.0fの範囲に正規化
	float normLeftStickX = leftStickX / 32768.0f;
	float normLeftStickY = leftStickY / 32768.0f;

	// スティックの傾きの大きさを計算
	float pow = VSize({ normLeftStickX, 0.0f, normLeftStickY });

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	bool enableKAM = ins->IsEnableKeyAndMouse();
	bool isGamepadConnected = (GetJoypadNum() > 0);

	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			ins->GetStickDirXZ(dir, player->GetPadNum(), true, ins->DEFAULT_STICK_DEADZONE);
			ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
		}
		else
		{
			ins->GetStickDirXZ(dir, player->GetPadNum(), true, ins->DEFAULT_STICK_DEADZONE);
		}
	}
	else
	{
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
	}

	// スティックが動いていなくて、キーボードのWASD入力がある場合
	if (pow <= 0.0f && !AsoUtility::EqualsVZero(dir))
	{
		pow = 1.0f;
	}

	// アニメーション再生
	if (pow > 0.5f)
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::FAST_RUN), true);
	}
	else
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::RUN), true);
	}

	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		player->SetMoveSpeed(Player::SPEED_MOVE);
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);
		player->SetMovePow(VScale(moveDir, Player::SPEED_MOVE));
	}
	else
	{
		player->ChangeState(Player::STATE::IDLE);
		player->SetAttacking(false);
		return;
	}
}
