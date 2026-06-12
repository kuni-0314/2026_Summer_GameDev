#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "Player.h"
#include "../../Common/AnimationController.h"
#include "PlayerFastRunState.h"

void PlayerFastRunState::Enter(Player* player)
{
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::FAST_RUN), true);
}

void PlayerFastRunState::Update(Player* player)
{
	auto ins = InputManager::GetInstance();
	
	// 遷移チェック
	if (CheckTransitions(player))
	{
		return;
	}

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// 入力デバイスの種類を判定
	bool isGamepadConnected = (GetJoypadNum() > 0);
	constexpr int GAMEPAD_INDEX = 0;

	bool isDashInput = false;

	if (isGamepadConnected)
	{
		// ゲームパッド操作
		short stickX = 0, stickY = 0;
		ins->GetLeftStick(GAMEPAD_INDEX, stickX, stickY);

		// スティック入力を方向ベクトルに変換（デッドゾーン処理）
		constexpr float STICK_DEADZONE = 0.2f;
		constexpr float STICK_MAX = 32767.0f;
		float normalizedX = stickX / STICK_MAX;
		float normalizedY = stickY / STICK_MAX;

		if (abs(normalizedX) > STICK_DEADZONE || abs(normalizedY) > STICK_DEADZONE)
		{
			dir.x = normalizedX;
			dir.z = -normalizedY; // Y軸は反転
		}

		isDashInput = ins->IsGamepadNew(InputManager::PadInput::A, GAMEPAD_INDEX);
	}
	else
	{
		// キーボード操作
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
		isDashInput = ins->IsNew(KEY_INPUT_LSHIFT);
	}

	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		// ダッシュキーが入力されていないか
		if (!isDashInput)
		{
			player->ChangeState(Player::STATE::RUN);
			return;
		}

		// 移動速度を設定
		player->SetMoveSpeed(Player::SPEED_DASH);

		// Y軸のみのカメラ角度を取得
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

		// 移動方向をカメラに合わせる
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);

		// 移動量を計算
		player->SetMovePow(VScale(moveDir, Player::SPEED_DASH));
	}
	else
	{
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}
