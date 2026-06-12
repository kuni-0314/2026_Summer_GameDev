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
	// 初期アニメーション再生
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::RUN), true);
}

void PlayerRunState::Update(Player* player)
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
		bool isDashInput = false;
		if (isGamepadConnected)
		{
			isDashInput = ins->IsGamepadNew(InputManager::PadInput::A, GAMEPAD_INDEX);
		}
		else
		{
			isDashInput = ins->IsNew(KEY_INPUT_LSHIFT);
		}

		if (isDashInput)
		{
			player->ChangeState(Player::STATE::FAST_RUN);
			return;
		}
		
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
