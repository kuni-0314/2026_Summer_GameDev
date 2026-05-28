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

	// クールタイムを設定
	player->SetAttackCoolTime(ATTACK_COOL_TIME[static_cast<int>(attackType_)]);

	// コンボタイマーをリセット
	if (attackType_ == ATTACK_TYPE::HEAVY || 
		attackType_ == ATTACK_TYPE::DASH || 
		attackType_ == ATTACK_TYPE::FALL)
	{
		// 特殊攻撃はコンボをリセット
		player->SetComboTimer(0);
	}
	else
	{
		// 通常攻撃はコンボ継続
		player->SetComboTimer(COMBO_WINDOW_FRAME);
	}

	// アニメーション再生
	player->GetAnimationController()->Play(
		static_cast<int>(Player::ANIM_TYPE::ATTACK), false, true);

	player->GetGameScene()->CreateAttackCollider(
		ColliderBase::TAG::PLAYER,
		player->GetTransform().pos,
		50.0f, // 仮の攻撃範囲
		ATTACK_POW[static_cast<int>(attackType_)],
		ATTACK_COOL_TIME[static_cast<int>(attackType_)]);	// 一旦攻撃のクールタイムを攻撃コライダーの寿命にする

	// 空中攻撃の場合は移動量を設定
	//if (player->IsAir())
	//{
	//	player->SetMovePow({ 0.0f, 100.0f, 0.0f });
	//}
}

void PlayerAttackState::Update(Player* player)
{
	// 遷移チェック
	if (CheckTransitions(player))
	{
		return;
	}

	// 慣性による移動量の減衰
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

	// アニメーションが終了したらIDLE状態に戻る
	if (player->GetAnimationController()->IsEnd())
	{
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
		color = 0xFF0000; // 赤：クールタイム中
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

PlayerAttackState::ATTACK_TYPE PlayerAttackState::GetNextAttackType(Player* player)
{
	auto ins = InputManager::GetInstance();

	// 長押し判定
	if (ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30)
	{
		return player->IsAir() ? ATTACK_TYPE::FALL : ATTACK_TYPE::HEAVY;
	}

	// ダッシュ攻撃判定
	if (!player->IsAir() && 
		ins->IsNew(KEY_INPUT_LSHIFT) &&
		(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
		 ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D)))
	{
		return ATTACK_TYPE::DASH;
	}

	// コンボ継続判定
	bool inCombo = player->GetComboTimer() > 0;

	if (player->IsAir())
	{
		// 空中コンボ
		if (!inCombo) return ATTACK_TYPE::AIR1;

		switch (attackType_)
		{
		case ATTACK_TYPE::AIR1: return ATTACK_TYPE::AIR2;
		case ATTACK_TYPE::AIR2: return ATTACK_TYPE::AIR3;
		case ATTACK_TYPE::AIR3: return ATTACK_TYPE::AIR4;
		case ATTACK_TYPE::AIR4: return ATTACK_TYPE::AIR5;
		default: return ATTACK_TYPE::AIR1;
		}
	}
	else
	{
		// 地上コンボ
		if (!inCombo) return ATTACK_TYPE::NORMAL1;

		switch (attackType_)
		{
		case ATTACK_TYPE::NORMAL1: return ATTACK_TYPE::NORMAL2;
		case ATTACK_TYPE::NORMAL2: return ATTACK_TYPE::NORMAL3;
		case ATTACK_TYPE::NORMAL3: return ATTACK_TYPE::NORMAL4;
		case ATTACK_TYPE::NORMAL4: return ATTACK_TYPE::NORMAL5;
		default: return ATTACK_TYPE::NORMAL1;
		}
	}
}
