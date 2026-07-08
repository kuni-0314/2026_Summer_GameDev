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
	player->SetAttacking(true);

	// 攻撃タイプを決定
	attackType_ = GetNextAttackType(player);

	// ルートモーション有効化
	player->SetApplyRootMotion(true);
	
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
		ATTACK_POW[static_cast<int>(attackType_)],
		60); 

	//if (player->IsAir())
	//{
	//	VECTOR movePow = player->GetMovePow();
	//	movePow.y = 200.0f;
	//	player->SetMovePow(movePow);
	//}

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);

}

void PlayerAttackState::Update(Player* player)
{
	// 攻撃アニメーション中は他の状態への遷移をチェックしない

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
	// ジャンプキーが入力されているか
	bool isJumpKeyPressed = false;
	if (player->IsJump())
	{
		if (isGamepadConnected)
		{
			if (enableKAM)
			{
				if (ins->IsNew(KEY_INPUT_SPACE) || ins->IsGamepadNew(InputManager::PadInput::A, player->GetPadNum()))
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
				if (ins->IsGamepadNew(InputManager::PadInput::A, player->GetPadNum()))
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

	// 移動量の減衰を無効化(ルートモーションで制御)
	// ダッシュ攻撃などの場合はルートモーションで移動を表現

	VECTOR framePos = MV1GetFramePosition(player->GetTransform().modelId, 2);
	framePos.y = player->GetPos().y;
	player->SetPos(framePos);

	// アニメーション終了時
	if (player->GetAnimationController()->IsEnd())
	{
		// クールタイムをクリア（攻撃可能に）
		player->SetAttackCoolTime(0);

		// コンボタイマーを設定
		if (attackType_ == ATTACK_TYPE::HEAVY || 
			attackType_ == ATTACK_TYPE::FALL)
		{
			player->SetComboTimer(0);
		}
		else
		{
			player->SetComboTimer(COMBO_WINDOW_FRAME);
		}
		
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}

void PlayerAttackState::Draw(Player* player)
{
	// デバッグ表示用（コメントアウト済み）
}

void PlayerAttackState::Exit(Player* player)
{
	// 攻撃終了時の処理
	player->SetAttacking(false);
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
		isHeavyAttack = ins->GetGamepadLastHoldTime(static_cast<int>(InputManager::PadInput::X), GAMEPAD_INDEX) > 30;
	}
	else
	{
		isHeavyAttack = ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30;
	}

	if (isHeavyAttack)
	{
	
		

		if (player->IsAir())
		{
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::FALL;
			//AudioManager::GetInstance()->SetSeVolume(100);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
		else
		{
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::HEAVY;
			//AudioManager::GetInstance()->SetSeVolume(100);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
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

	if (player->IsAir())
	{
		// 空中コンボ
		if (!inCombo)
		{
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A1), false, true);
			return ATTACK_TYPE::AIR1;
		}

		switch (attackType_)
		{
		case ATTACK_TYPE::AIR1:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A2), false, true);
			return ATTACK_TYPE::AIR2;
		case ATTACK_TYPE::AIR2:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A3), false, true);
			return ATTACK_TYPE::AIR3;
		case ATTACK_TYPE::AIR3:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A4), false, true);
			return ATTACK_TYPE::AIR4;
		case ATTACK_TYPE::AIR4:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A5), false, true);
			return ATTACK_TYPE::AIR5;
		default:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A1), false, true);
			return ATTACK_TYPE::AIR1;
		}
	}
	else
	{
		// 地上コンボ
		if (!inCombo)
		{
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
			//AudioManager::GetInstance()->SetSeVolume(150);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
		switch (attackType_)
		{
		case ATTACK_TYPE::NORMAL1: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N2), false, true);
			//AudioManager::GetInstance()->SetSeVolume(150);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
			return ATTACK_TYPE::NORMAL2;
		case ATTACK_TYPE::NORMAL2: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N3), false, true);
			return ATTACK_TYPE::NORMAL3;
		case ATTACK_TYPE::NORMAL3: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N4), false, true);
			return ATTACK_TYPE::NORMAL4;
		case ATTACK_TYPE::NORMAL4: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N5), false, true);
			return ATTACK_TYPE::NORMAL5;
		default: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
		}
	}
}
