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

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		if (ins->IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins->IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins->IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins->IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }
	}
	// ゲームパッド操作
	//else
	//{
	//}

	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		// ダッシュキー
		if (ins->IsNew(KEY_INPUT_LSHIFT))
		{
			player->ChangeState(Player::STATE::FAST_RUN);
		}
		float speed = 0;
		//if (isDash_)
		//{
		//	moveSpeed_ = SPEED_DASH;
		//}
		//else
		{
			//移動スピード
			speed = Player::SPEED_MOVE;
		}
		player->SetMoveSpeed(speed);

		// ジャンプ中はアニメーションを変えない
		//if (!isJump_)
		//{
		//	// アニメーション
		//	if (isDash_)
		//	{

		//		animationController_->Play(
		//			static_cast<int>(ANIM_TYPE::FAST_RUN), true);
		//	}
		//	else
		//	{
		//		animationController_->Play(
		//			static_cast<int>(ANIM_TYPE::RUN), true);
		//	}
		//}


		//Y軸のみのカメラ角度を取得
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

		//移動方向をカメラに合わせる
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);

		//移動量を計算
		/*if (!isJet_)*/ player->SetMovePow(VScale(moveDir, speed));
	}
	else
	{
		//// ジャンプ中はアニメーションを変えない
		//if (!isJump_)
		{
			player->ChangeState(Player::STATE::IDLE);
		}
	}
}
