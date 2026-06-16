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

	// アニメーション再生
	if (pow > 0.85f)
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::FAST_RUN), true);
	}
	else
	{
		player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::RUN), true);
	}

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	bool enableKAM = ins->IsEnableKeyAndMouse();
	bool isGamepadConnected = (GetJoypadNum() > 0);

	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			// ゲームパッド操作
			ins->GetStickDirXZ(dir, player->GetPadNum(), true, ins->DEFAULT_STICK_DEADZONE);

			// キーボード操作
			ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
		}
		else
		{
			// ゲームパッド操作
			ins->GetStickDirXZ(dir, player->GetPadNum(), true, ins->DEFAULT_STICK_DEADZONE);
		}
	}
	else
	{
		// キーボード操作
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
	}
	
	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		// ダッシュ入力のチェック
		//bool isDashInput = false;
		//if (isGamepadConnected)
		//{
		//	isDashInput = ins->IsGamepadNew(InputManager::PadInput::A, GAMEPAD_INDEX);
		//}
		//else
		//{
		//	isDashInput = ins->IsNew(KEY_INPUT_LSHIFT);
		//}

		//if (isDashInput)
		//{
		//	player->ChangeState(Player::STATE::FAST_RUN);
		//	return;
		//}
		
		// 移動速度を設定
		player->SetMoveSpeed(Player::SPEED_MOVE);

		// Y軸のみのカメラ角度を取得
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

		// 移動方向をカメラに合わせる
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);

		// 移動量を計算
		player->SetMovePow(VScale(moveDir, Player::SPEED_MOVE));
	}
	else
	{
		// 方向入力がない場合は待機状態に戻る
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}
