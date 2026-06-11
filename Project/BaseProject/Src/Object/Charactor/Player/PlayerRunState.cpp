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

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
	}
	else
	{

	}

	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		// ダッシュキーが入力されているか
		if (ins->IsNew(KEY_INPUT_LSHIFT))
		{
			player->ChangeState(Player::STATE::FAST_RUN);
			return;
		}
		
		// 移動速度を設定
		player->SetMoveSpeed(Player::SPEED_MOVE);

		//Y軸のみのカメラ角度を取得
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

		//移動方向をカメラに合わせる
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);

		//移動量を計算
		player->SetMovePow(VScale(moveDir, Player::SPEED_MOVE));
	}
	else
	{
		// 方向入力がない場合は待機状態に戻る
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}
