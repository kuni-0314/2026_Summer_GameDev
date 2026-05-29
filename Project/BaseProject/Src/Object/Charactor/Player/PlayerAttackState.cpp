#include "PlayerAttackState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Scene/GameScene.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"

void PlayerAttackState::Enter(Player* player)
{
	// 攻撃タイプを決定
	attackType_ = GetNextAttackType(player);

	// クールタイムを設定（アニメーション中は攻撃不可）
	// 大きな値を設定して、アニメーション終了時に再設定
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
	
	if (player->IsAir())
	{
		VECTOR movePow = player->GetMovePow();
		movePow.y = 200.0f;
		player->SetMovePow(movePow);
	}
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

	// アニメーション終了時
	if (player->GetAnimationController()->IsEnd())
	{
		// クールタイムをクリア（攻撃可能に）
		player->SetAttackCoolTime(0);

		// コンボタイマーを設定
		if (attackType_ == ATTACK_TYPE::HEAVY || 
			attackType_ == ATTACK_TYPE::DASH || 
			attackType_ == ATTACK_TYPE::FALL)
		{
			// 強攻撃、ダッシュ攻撃、落下攻撃はコンボ不可
			player->SetComboTimer(0);
		}
		else
		{
			// 通常攻撃と空中攻撃は60フレームのコンボ受付時間
			player->SetComboTimer(COMBO_WINDOW_FRAME);
		}
		
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}

void PlayerAttackState::Draw(Player* player)
{
	bool state[5][5] = {};
	switch (attackType_)
	{
	case ATTACK_TYPE::NORMAL1: state[0][0] = true; break;
	case ATTACK_TYPE::NORMAL2: state[0][1] = true; break;
	case ATTACK_TYPE::NORMAL3: state[0][2] = true; break;
	case ATTACK_TYPE::NORMAL4: state[0][3] = true; break;
	case ATTACK_TYPE::NORMAL5: state[0][4] = true; break;
	case ATTACK_TYPE::HEAVY:   state[1][0] = true; break;
	case ATTACK_TYPE::DASH:    state[2][0] = true; break;
	case ATTACK_TYPE::AIR1:    state[3][0] = true; break;
	case ATTACK_TYPE::AIR2:    state[3][1] = true; break;
	case ATTACK_TYPE::AIR3:    state[3][2] = true; break;
	case ATTACK_TYPE::AIR4:    state[3][3] = true; break;
	case ATTACK_TYPE::AIR5:    state[3][4] = true; break;
	case ATTACK_TYPE::FALL:    state[4][0] = true; break;
	default: break;
	}

	const int size = 50;
	int color = 0xFFFF00;

	if (player->GetAttackCoolTime() > 0)
	{
		color = 0xFF0000; // 赤：クールタイム中（アニメーション中）
	}
	else if (player->GetComboTimer() > 0)
	{
		color = 0x00FF00; // 緑：コンボ可能
	}

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			DrawBox(size * x, size * y, size * (x + 1), size * (y + 1), 
				color, state[y][x]);
		}
	}

	DrawFormatString(0, 260, 0xFFFFFF, "CoolTime: %d", player->GetAttackCoolTime());
	DrawFormatString(0, 280, 0xFFFFFF, "ComboTimer: %d / %d", 
		player->GetComboTimer(), COMBO_WINDOW_FRAME);
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

	// 長押し判定
	if (ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30)
	{
		if (player->IsAir())
		{
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::FALL;
		}
		else
		{
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::HEAVY;
		}
	}

	// ダッシュ攻撃判定
	if (!player->IsAir() && 
		ins->IsNew(KEY_INPUT_LSHIFT) &&
		(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
		 ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D)))
	{
		// アニメーション再生
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_D), false, true);
		return ATTACK_TYPE::DASH;
	}

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
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A2), false, true);
			return ATTACK_TYPE::AIR2;
		case ATTACK_TYPE::AIR2:
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A3), false, true);
			return ATTACK_TYPE::AIR3;
		case ATTACK_TYPE::AIR3:
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A4), false, true);
			return ATTACK_TYPE::AIR4;
		case ATTACK_TYPE::AIR4:
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A5), false, true);
			return ATTACK_TYPE::AIR5;
		default:
			// アニメーション再生
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
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
		}
		switch (attackType_)
		{
		case ATTACK_TYPE::NORMAL1: 
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N2), false, true);
			return ATTACK_TYPE::NORMAL2;
		case ATTACK_TYPE::NORMAL2: 
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N3), false, true);
			return ATTACK_TYPE::NORMAL3;
		case ATTACK_TYPE::NORMAL3: 
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N4), false, true);
			return ATTACK_TYPE::NORMAL4;
		case ATTACK_TYPE::NORMAL4: 
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N5), false, true);
			return ATTACK_TYPE::NORMAL5;
		default: 
			// アニメーション再生
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
		}
	}
}
