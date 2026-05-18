#include "../../../Manager/SceneManager.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/Camera.h"
#include "player.h"
#include "../../Common/AnimationController.h"
#include "../../Collider/Line/ColliderLine.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Utility/AsoUtility.h"
#include "PlayerJumpState.h"

void PlayerJumpState::Enter(Player* player)
{
	// 初期アニメーション再生
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::JUMP), false);

	// ジャンプ量の計算
	VECTOR jumpPow = player->GetJumpPow();
	jumpPow = VAdd(jumpPow, VScale(AsoUtility::DIR_U, Player::POW_JUMP_INIT));
	player->SetJumpPow(jumpPow);
	player->SetStepJump(0.0f);
	player->SetJump(true);
}

void PlayerJumpState::Update(Player* player)
{
	auto ins = InputManager::GetInstance();

	// ジャンプキーが入力されているか
	if (player->IsJump() &&
		ins->IsNew(KEY_INPUT_SPACE)
		/*|| ins->IsGamepadNew(InputManager::PadInput::B, 0)*/)
	{
		if (player->GetStepJump() < Player::TIME_JUMP_INPUT)
		{
			player->SetJumpPow(VAdd(player->GetJumpPow(), VScale(AsoUtility::DIR_U, Player::POW_JUMP_KEEP)));
			player->SetStepJump(player->GetStepJump() + SceneManager::GetInstance().GetDeltaTime());
		}
	}

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// 着地している場合は、待機状態に遷移
	if (!player->IsJump())
	{
		player->ChangeState(Player::STATE::IDLE);
		return;
	}

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		ins->GetInputDirXZ(dir, KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D);
	}
	//else
	//{
	//	 ゲームパッド操作
	//}

	// 方向入力がある場合
	if (!AsoUtility::EqualsVZero(dir))
	{
		// 移動速度を設定
		float speed = 0.0f;
		if (ins->IsNew(KEY_INPUT_LSHIFT))
		{
			speed = Player::SPEED_DASH;
		}
		else
		{
			speed = Player::SPEED_MOVE;
		}

		//Y軸のみのカメラ角度を取得
		Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotY();

		//移動方向をカメラに合わせる
		VECTOR moveDir = Quaternion::PosAxis(cameraRot, dir);
		player->SetMoveDir(moveDir);

		//移動量を計算
		player->SetMovePow(VScale(moveDir, speed));

		// ジャンプ中でない場合は状態を変更
		if (!player->IsJump())
		{
			// ダッシュキーが入力されているか
			if (ins->IsNew(KEY_INPUT_LSHIFT))
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
	// ジャンプ中は方向入力がない場合でも状態遷移はしない
	//else
	//{
	//}
}

void PlayerJumpState::CollisionReserve(Player* player)
{
	// アニメーションごとの線分調整
	if (player->GetAnimationController()->GetPlayType() == static_cast<int>(Player::ANIM_TYPE::JUMP))
	{
		// ジャンプ中は線分を伸ばす
		if (player->GetOwnColliders().count(static_cast<int>(CharactorBase::COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				player->GetOwnColliders().at(static_cast<int>(CharactorBase::COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(Player::COL_LINE_JUMP_START_LOCAL_POS);
			colLine->SetLocalPosEnd(Player::COL_LINE_JUMP_END_LOCAL_POS);
		}

		if (player->GetOwnColliders().count(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				player->GetOwnColliders().at(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(Player::COL_CAPSULE_TOP_JUMP_LOCAL_POS);
			colCapsule->SetLocalPosDown(Player::COL_CAPSULE_DOWN_JUMP_LOCAL_POS);
			colCapsule->SetRadius(Player::COL_CAPSULE_RADIUS);
		}
	}
	else
	{
		// 通常時の線分に戻す
		if (player->GetOwnColliders().count(static_cast<int>(CharactorBase::COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				player->GetOwnColliders().at(static_cast<int>(CharactorBase::COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(Player::COL_LINE_START_LOCAL_POS);
			colLine->SetLocalPosEnd(Player::COL_LINE_END_LOCAL_POS);
		}
		//通常時のカプセル
		if (player->GetOwnColliders().count(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				player->GetOwnColliders().at(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(Player::COL_CAPSULE_TOP_LOCAL_POS);
			colCapsule->SetLocalPosDown(Player::COL_CAPSULE_DOWN_LOCAL_POS);
		}
	}
}
