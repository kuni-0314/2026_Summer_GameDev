#include <DxLib.h>
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Object/Common/AnimationController.h"
#include "Player.h"
#include "PlayerJetState.h"

void PlayerRollState::Enter(Player* player)
{
	player->SetMovePow(VScale(player->GetMoveDir(), Player::POW_ROLL));
	player->SetJet(true);

	// アニメーション再生
	player->GetAnimationController()->Play(
		static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
}

void PlayerRollState::Update(Player* player)
{
	// ローリング(旧ジェット）
	// ローリング中はその他の入力を受け付けない

	if (player->GetAnimationController()->IsEnd())
	{
		auto ins = InputManager::GetInstance();

		if (player->IsJump())
		{
			// 空中にいる場合は落下状態に遷移
			player->ChangeState(Player::STATE::FALL);
		}
		else if (ins->IsNew({ KEY_INPUT_W,KEY_INPUT_A,KEY_INPUT_S,KEY_INPUT_D }) ||
			ins->GetGamepadTriggerValue(true, player->GetPadNum()) > 0)
		{
			// 移動キーが入力されている場合
			player->ChangeState(Player::STATE::RUN);
		}
		else
		{
			// それ以外は待機状態に遷移
			player->ChangeState(Player::STATE::IDLE);
		}
	}
}

void PlayerRollState::Exit(Player* player)
{
	player->SetJet(false);
}
