#include "PlayerAttackState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"

void PlayerAttackState::Enter(Player* player)
{
	auto ins = InputManager::GetInstance();

	if (ins->IsMouseTrgUp(MOUSE_INPUT_LEFT))
	{
		if (ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30)
		{
			if (player->IsAir())
			{
				attackType_ = PlayerAttackState::ATTACK_TYPE::FALL;
			}
			else
			{
				attackType_ = PlayerAttackState::ATTACK_TYPE::HEAVY;
			}
		}
		else
		{
			if (player->IsAir())
			{
				switch (attackType_)
				{
				case PlayerAttackState::ATTACK_TYPE::AIR1:
					attackType_ = PlayerAttackState::ATTACK_TYPE::AIR2;
					break;
				case PlayerAttackState::ATTACK_TYPE::AIR2:
					attackType_ = PlayerAttackState::ATTACK_TYPE::AIR3;
					break;
				case PlayerAttackState::ATTACK_TYPE::AIR3:
					attackType_ = PlayerAttackState::ATTACK_TYPE::AIR4;
					break;
				case PlayerAttackState::ATTACK_TYPE::AIR4:
					attackType_ = PlayerAttackState::ATTACK_TYPE::AIR5;
					break;
				case PlayerAttackState::ATTACK_TYPE::AIR5:;
				default:
					attackType_ = PlayerAttackState::ATTACK_TYPE::AIR1;
					break;
				}
			}
			else
			{
				if (ins->IsNew(KEY_INPUT_LSHIFT) &&
					(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D)))
				{
					attackType_ = PlayerAttackState::ATTACK_TYPE::DASH;
				}
				else
				{
					switch (attackType_)
					{
					case PlayerAttackState::ATTACK_TYPE::NORMAL1:
						attackType_ = PlayerAttackState::ATTACK_TYPE::NORMAL2;
						break;
					case PlayerAttackState::ATTACK_TYPE::NORMAL2:
						attackType_ = PlayerAttackState::ATTACK_TYPE::NORMAL3;
						break;
					case PlayerAttackState::ATTACK_TYPE::NORMAL3:
						attackType_ = PlayerAttackState::ATTACK_TYPE::NORMAL4;
						break;
					case PlayerAttackState::ATTACK_TYPE::NORMAL4:
						attackType_ = PlayerAttackState::ATTACK_TYPE::NORMAL5;
						break;
					case PlayerAttackState::ATTACK_TYPE::NORMAL5:
					default:
						attackType_ = PlayerAttackState::ATTACK_TYPE::NORMAL1;
						break;
					}
				}
			}
		}
	}
	else if (!ins->IsMouseNew(MOUSE_INPUT_LEFT))
	{
		tmp_ = false;
	}

	if (tmp_) player->ChangeState(Player::STATE::ATTACK);


	if (player->IsAir())
	{
		player->SetMovePow({ 0.0f, 100.0f, 0.0f });
	}




	// 攻撃アニメーション再生
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::ATTACK), false);

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

		// 地上
		movePow = VScale(movePow, Player::GROUND_MOVE_DEC_RATE);

		// 十分小さくなったら完全に停止
		if (VSize(movePow) < 0.01f)
		{
			movePow = { 0.0f, 0.0f, 0.0f };
		}

		player->SetMovePow(movePow);
	}

	auto ins = InputManager::GetInstance();
	




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
	case PlayerAttackState::ATTACK_TYPE::NORMAL1:
		state[0][0] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL2:
		state[0][1] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL3:
		state[0][2] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL4:
		state[0][3] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::NORMAL5:
		state[0][4] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::HEAVY:
		state[1][0] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::DASH:
		state[2][0] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::AIR1:
		state[3][0] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::AIR2:
		state[3][1] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::AIR3:
		state[3][2] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::AIR4:
		state[3][3] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::AIR5:
		state[3][4] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::FALL:
		state[4][0] = true;
		break;
	case PlayerAttackState::ATTACK_TYPE::MAX:
	default:
		break;
	}

	const int size = 50;
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			DrawBox(size * x, size * y, size * (x + 1), size * (y + 1), 0xFFFF00, state[y][x]);
		}
	}
}
