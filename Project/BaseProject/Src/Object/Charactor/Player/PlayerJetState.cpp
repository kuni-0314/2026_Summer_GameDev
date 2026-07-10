#include <DxLib.h>
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "Player.h"
#include "PlayerJetState.h"

void PlayerJetState::Enter(Player* player)
{
	player->SetMovePow(VScale(player->GetMoveDir(), Player::POW_JET));
	player->SetJet(true);
	player->SetJetTime(0.0f);

	//追加
	player->PlayBlinkEffect();
}

void PlayerJetState::Update(Player* player)
{
	if (player->GetJetTime() < Player::JET_TIME)
	{
		player->SetJetTime(player->GetJetTime() + SceneManager::GetInstance().GetDeltaTime());
	}
	else
	{
		auto ins = InputManager::GetInstance();

		if (player->IsJump())
		{
			// 空中にいる場合は落下状態に遷移
			player->ChangeState(Player::STATE::FALL);
			player->SetAttacking(false);
		}
		else if (ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D))
		{
			//if (ins->IsNew(KEY_INPUT_LSHIFT))
			//{
			//	// 移動キーとダッシュキーが入力されている場合
			//	player->ChangeState(Player::STATE::FAST_RUN);
			//}
			//else
			{
				// 移動キーが入力されている場合
				player->ChangeState(Player::STATE::RUN);
				player->SetAttacking(false);
			}
		}
		else
		{
			// それ以外は待機状態に遷移
			player->ChangeState(Player::STATE::IDLE);
			player->SetAttacking(false);
		}
	}
}

void PlayerJetState::Exit(Player* player)
{
	player->SetJet(false);
}
