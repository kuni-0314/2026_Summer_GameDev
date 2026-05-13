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

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		bool isInputMoveKey = false;
		if (ins->IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; isInputMoveKey = true; }
		if (ins->IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; isInputMoveKey = true; }
		if (ins->IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; isInputMoveKey = true; }
		if (ins->IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; isInputMoveKey = true; }

		// 移動キーが離されたら待機状態に戻る
		if (!isInputMoveKey)
		{
			player->ChangeState(Player::STATE::IDLE);
			return;
		}

		// ダッシュキーが離されたら通常走りに戻る
		{
			if (!ins->IsNew(KEY_INPUT_LSHIFT)) player->ChangeState(Player::STATE::RUN); 
			return;
		}

		if (!AsoUtility::EqualsVZero(dir))
		{
			float speed = Player::SPEED_DASH;

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
	}
}
