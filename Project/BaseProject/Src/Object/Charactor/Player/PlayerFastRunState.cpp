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

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);

		// 方向入力がある場合
		if (!AsoUtility::EqualsVZero(dir))
		{
			// ダッシュキーが入力されていないか
			if (!ins->IsNew(KEY_INPUT_LSHIFT))
			{
				player->ChangeState(Player::STATE::RUN);
				return;
			}

			// 移動速度を設定
			player->SetMoveSpeed(Player::SPEED_DASH);

			//Y軸のみのカメラ角度を取得
			Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

			//移動方向をカメラに合わせる
			VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
			player->SetMoveDir(moveDir);

			//移動量を計算
			player->SetMovePow(VScale(moveDir, Player::SPEED_DASH));
		}
		else
		{
			player->ChangeState(Player::STATE::IDLE);
			return;
		}
	}
}
