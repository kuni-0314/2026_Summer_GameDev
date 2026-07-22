#include "PlayerAttackState.h"
#include "Player.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/InputManager.h"
#include "../../../Scene/GameScene.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Sound/AudioManager.h"

void PlayerAttackState::Enter(Player* player)
{
	// 攻撃タイプを決定
	attackType_ = GetNextAttackType(player);

	// スタック防止用の変数を初期化
	stopTimer_ = 0;
	noMovementFrameCount_ = 0;
	isAnimationSkipped_ = false;
	forceResumed_ = false;

	// ルートモーション有効化
	//player->SetApplyRootMotion(true);
	
	// アニメーション開始時のモデルのローカル座標を記録
	MATRIX modelMatrix = MV1GetFrameLocalWorldMatrix(player->GetTransform().modelId, 2);
	VECTOR localPos = { modelMatrix.m[3][0], modelMatrix.m[3][1], modelMatrix.m[3][2] };
	player->SetAnimStartModelPos(localPos);

	// クールタイムを設定（アニメーション中は攻撃不可）
	player->SetAttackCoolTime(9999);

	// コンボタイマーをリセット（アニメーション中はコンボ不可）
	player->SetComboTimer(0);

	// 攻撃位置を計算してコライダ生成
	VECTOR attackPos = CalculateAttackPosition(player);
	player->GetGameScene()->CreateAttackCollider(
		ColliderBase::TAG::PLAYER,
		attackPos,
		ATTACK_RADIUS,
		ATTACK_POW[static_cast<int>(attackType_)]); 

	if (player->IsAir())
	{
		// ジャンプ量の計算
		VECTOR jumpPow = player->GetJumpPow();
		jumpPow = VAdd(jumpPow, VScale(AsoUtility::DIR_U, Player::POW_JUMP_INIT));
		player->SetJumpPow(jumpPow);
		player->SetStepJump(0.0f);
		player->SetJump(true);
	}

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);

}

void PlayerAttackState::Update(Player* player)
{
	// 攻撃アニメーション中は他の状態への遷移をチェックしない
	// はずだったけど回避だけ可能にしますわ

	// 移動量の減衰
	if (!AsoUtility::EqualsVZero(player->GetMovePow()))
	{
		VECTOR movePow = player->GetMovePow();
		movePow = VScale(movePow, Player::GROUND_MOVE_DEC_RATE);
	
		if (VSize(movePow) < 0.01f)
		{
			movePow = { 0.0f, 0.0f, 0.0f };
		}
	
		player->SetMovePow(movePow);
	}

	auto ins = InputManager::GetInstance();
	bool enableKAM = ins->IsEnableKeyAndMouse();
	bool isGamepadConnected = (GetJoypadNum() > 0);

	if (isGamepadConnected)
	{
		if (enableKAM)
		{
			if (ins->IsGamepadTrgDown(InputManager::PadInput::X, player->GetPadNum()) ||
				ins->IsMouseTrgDown(MOUSE_INPUT_RIGHT))
			{	
				player->ChangeState(Player::STATE::JET);
			}
		}
		else
		{
			if (ins->IsGamepadTrgDown(InputManager::PadInput::X, player->GetPadNum()))
			{
				player->ChangeState(Player::STATE::JET);
			}
		}
	}
	else
	{
		if (ins->IsMouseTrgDown(MOUSE_INPUT_RIGHT))
		{
			player->ChangeState(Player::STATE::JET);
		}
	}

	// ジャンプキーが入力されているか
	bool isJumpKeyPressed = false;
	if (player->IsJump())
	{
		if (isGamepadConnected)
		{
			if (enableKAM)
			{
				if (ins->IsNew(KEY_INPUT_SPACE) || ins->IsGamepadNew(InputManager::PadInput::B, player->GetPadNum()))
				{
					if (player->GetStepJump() < Player::TIME_JUMP_INPUT)
					{
						player->SetJumpPow(VAdd(player->GetJumpPow(), VScale(AsoUtility::DIR_U, Player::POW_JUMP_KEEP)));
						player->SetStepJump(player->GetStepJump() + SceneManager::GetInstance().GetDeltaTime());
					}
					isJumpKeyPressed = true;
				}
			}
			else
			{
				if (ins->IsGamepadNew(InputManager::PadInput::B, player->GetPadNum()))
				{
					if (player->GetStepJump() < Player::TIME_JUMP_INPUT)
					{
						player->SetJumpPow(VAdd(player->GetJumpPow(), VScale(AsoUtility::DIR_U, Player::POW_JUMP_KEEP)));
						player->SetStepJump(player->GetStepJump() + SceneManager::GetInstance().GetDeltaTime());
					}
					isJumpKeyPressed = true;
				}
			}
		}
		else
		{
			if (ins->IsNew(KEY_INPUT_SPACE))
			{
				if (player->GetStepJump() < Player::TIME_JUMP_INPUT)
				{
					player->SetJumpPow(VAdd(player->GetJumpPow(), VScale(AsoUtility::DIR_U, Player::POW_JUMP_KEEP)));
					player->SetStepJump(player->GetStepJump() + SceneManager::GetInstance().GetDeltaTime());
				}
				isJumpKeyPressed = true;
			}
		}

		if (!isJumpKeyPressed)
		{
			if (player->GetStepJump() < Player::TIME_JUMP_INPUT)
			{
				player->SetJumpPow(VAdd(player->GetJumpPow(), VScale(AsoUtility::DIR_U, Player::POW_JUMP_NEUTRAL)));
				player->SetStepJump(player->GetStepJump() + SceneManager::GetInstance().GetDeltaTime());
			}
			isJumpKeyPressed = true;
		}
	}

	VECTOR framePos = MV1GetFramePosition(player->GetTransform().modelId, 2);
	framePos.y = player->GetPos().y;
	player->SetPos(framePos);

	UpdateAttack(player);

	// アニメーション終了時
	if (player->GetAnimationController()->IsEnd() || isAnimationSkipped_)
	{
		// クールタイムをクリア（攻撃可能に）
		player->SetAttackCoolTime(0);

		// コンボタイマーを設定
		if (attackType_ == ATTACK_TYPE::HEAVY)
		{
			player->SetComboTimer(0);
		}
		else
		{
			player->SetComboTimer(COMBO_WINDOW_FRAME);
		}
		
		// 特定のアニメーションはルートフレームが移動しているので設定を戻す
		if (attackType_ == ATTACK_TYPE::NORMAL4 ||
			attackType_ == ATTACK_TYPE::HEAVY)
		{
			player->GetAnimationController()->SetDynamicOffset(false);
			player->GetAnimationController()->SetIgnoreRootMove(false);
		}

		player->ChangeState(Player::STATE::IDLE);
		player->SetAttacking(false);
		return;
	}

	// 
	int a = 0;
}

void PlayerAttackState::Draw(Player* player)
{
	// デバッグ表示用（コメントアウト済み）
}

void PlayerAttackState::Exit(Player* player)
{
	// クールタイムをクリア（攻撃可能に）
	player->SetAttackCoolTime(0);

	// コンボタイマーを設定
	if (attackType_ == ATTACK_TYPE::HEAVY)
	{
		player->SetComboTimer(0);
	}
	else
	{
		player->SetComboTimer(COMBO_WINDOW_FRAME);
	}

	// 特定のアニメーションはルートフレームが移動しているので設定を戻す
	if (attackType_ == ATTACK_TYPE::NORMAL4 ||
		attackType_ == ATTACK_TYPE::HEAVY)
	{
		player->GetAnimationController()->SetDynamicOffset(false);
		player->GetAnimationController()->SetIgnoreRootMove(false);
	}
	player->SetAttacking(false);
	player->GetGameScene()->DeleteAttackCollider();
}

VECTOR PlayerAttackState::CalculateAttackPosition(Player* player)
{
	// プレイヤーの位置と回転を取得
	VECTOR playerPos = player->GetTransform().pos;
	VECTOR playerRot = player->GetTransform().rot;

	// ローカル座標をワールド座標に変換
	MATRIX rotMat = MGetRotY(playerRot.y);
	VECTOR worldOffset = VTransform(ATTACK_LOCAL_POS, rotMat);

	// プレイヤーの位置に加算
	return VAdd(playerPos, worldOffset);
}

void PlayerAttackState::UpdateAttack(Player* player)
{
	auto animController = player->GetAnimationController();
	int currentFrame = animController->GetAnimFrameNum();
	const int ATK_START_FRAME = ATTACK_FRAME[static_cast<int>(attackType_)][ATK_S_ANIM_INDEX];
	const int ATK_END_FRAME = ATTACK_FRAME[static_cast<int>(attackType_)][ATK_E_ANIM_INDEX];
	
	if (currentFrame >= ATK_END_FRAME)
	{
		if (player->IsAttacking()) 
			player->SetAttacking(false);
	}
	else if (currentFrame >= ATK_START_FRAME && !player->IsAttacking())
	{
		player->SetAttacking(true);
	}

	isAnimationSkipped_ = false;
	switch (attackType_)
	{
	case PlayerAttackState::ATTACK_TYPE::NONE:
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL1:
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL2:
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL3:
		if (currentFrame > 120)
		{
		 isAnimationSkipped_ = true;
		}
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL4:
		if (currentFrame > 140)
		{
		 isAnimationSkipped_ = true;
		}
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL5:
		break;
	case PlayerAttackState::ATTACK_TYPE::HEAVY:
	{
		// 範囲攻撃の実行
		if (currentFrame >= 88 && player->IsAttacking())
		{
			player->ExecuteRangeAttack();
		}
		
		// フレーム82以降で空中落下攻撃の制御（強制再開済みの場合はスキップ）
		if (currentFrame >= 82 && !forceResumed_)
		{
			if (player->IsAir())
			{
				// 空中にいる場合はアニメーションを停止
				if (animController->IsStopped())
				{
					stopTimer_++;
				}
				else
				{
					animController->SetStopped(true);
					stopTimer_ = 0;
				}
			}
			else
			{
				// 着地した場合はアニメーションを再開
				if (animController->IsStopped())
				{
					animController->SetStopped(false);
					stopTimer_ = 0;
					noMovementFrameCount_ = 0;
				}
			}
		}

		// スタック防止：空中でアニメーションが停止している場合のみ判定
		if (player->IsAir() && animController->IsStopped() && !forceResumed_)
		{
			float movePowMagnitude = VSize(player->GetMovePow());
			if (movePowMagnitude < NO_MOVEMENT_THRESHOLD)
			{
				noMovementFrameCount_++;
			}
			else
			{
				noMovementFrameCount_ = 0;
			}

			// 一定時間停止かつ移動していない場合はアニメーションを強制再開
			if (stopTimer_ > STOP_TIMER_MAX && noMovementFrameCount_ > NO_MOVEMENT_FRAME_THRESHOLD)
			{
				animController->SetStopped(false);
				stopTimer_ = 0;
				noMovementFrameCount_ = 0;
				forceResumed_ = true;  // 強制再開フラグを立てる
			}
		}
	}
		break;
	case PlayerAttackState::ATTACK_TYPE::MAX:
		break;
	default:
		break;
	}
}

PlayerAttackState::ATTACK_TYPE PlayerAttackState::GetNextAttackType(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 入力デバイスの種類を判定
	bool isGamepadConnected = (GetJoypadNum() > 0);
	constexpr int GAMEPAD_INDEX = 0;

	// 長押し判定
	bool isHeavyAttack = false;
	if (isGamepadConnected)
	{
		isHeavyAttack = ins->GetGamepadLastHoldTime(static_cast<int>(InputManager::PadInput::A), GAMEPAD_INDEX) > 30;
	}
	else
	{
		isHeavyAttack = ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30;
	}

	if (isHeavyAttack || (player->IsAir() && isHeavyAttack))
	{
		// アニメーション再生
		player->GetAnimationController()->SetDynamicOffset(true);
		player->GetAnimationController()->SetRootFrameParams(true, "mixamorig:Hips", { 0.0f, 97.0f, 0.0f });
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::HEAVY;
	}

	// ダッシュ攻撃判定
	//bool isDashAttack = false;
	//if (!player->IsAir())
	//{
	//	if (isGamepadConnected)
	//	{
	//		// ゲームパッド：左スティック入力中
	//		short stickX = 0, stickY = 0;
	//		ins->GetLeftStick(GAMEPAD_INDEX, stickX, stickY);

	//		constexpr float STICK_DEADZONE = 0.2f;
	//		constexpr float STICK_MAX = 32767.0f;
	//		float normalizedX = stickX / STICK_MAX;
	//		float normalizedY = stickY / STICK_MAX;

	//		isDashAttack = (abs(normalizedX) > STICK_DEADZONE || abs(normalizedY) > STICK_DEADZONE);
	//	}
	//	else
	//	{
	//		// キーボード：シフト + WASD
	//		isDashAttack = ins->IsNew(KEY_INPUT_LSHIFT) &&
	//			(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
	//			 ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D));
	//	}
	//}

	//if (isDashAttack)
	//{
	//	// アニメーション再生
	//	player->GetAnimationController()->Play(
	//		static_cast<int>(Player::ANIM_TYPE::ATK_D), false, true);
	//	return ATTACK_TYPE::DASH;
	//}

	// コンボ継続判定
	bool inCombo = player->GetComboTimer() > 0;

	// 地上コンボ
	if (!inCombo)
	{
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
		return ATTACK_TYPE::NORMAL1;
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
	}
	switch (attackType_)
	{
	case ATTACK_TYPE::NORMAL1:
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N2), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::NORMAL2;
	case ATTACK_TYPE::NORMAL2:
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N3), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::NORMAL3;
	case ATTACK_TYPE::NORMAL3:
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N4), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::NORMAL4;
	case ATTACK_TYPE::NORMAL4:
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N5), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::NORMAL5;
	default:
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		return ATTACK_TYPE::NORMAL1;
	}
}
