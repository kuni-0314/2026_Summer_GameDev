#include <DxLib.h>
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Manager/PostEffectManager.h"
#include "Player.h"
#include "PlayerJetState.h"

void PlayerRollState::Enter(Player* player)
{
	player->SetJet(true);

	// ローリング中は無敵状態にする
	player->SetInvincible(true);

	// アニメーション再生
	player->GetAnimationController()->SetRootFrameParams(true, "mixamorig:Hips", { 0.0f, 97.0f, 0.0f });
	player->GetAnimationController()->Play(
		static_cast<int>(Player::ANIM_TYPE::ROLLING), false, true);
}

void PlayerRollState::Update(Player* player)
{
	// ローリング(旧ジェット）
	// ローリング中はその他の入力を受け付けない
	int animNum = player->GetAnimationController()->GetAnimFrameNum();
	if (animNum >= 10 && animNum <= 35)
	{
		player->SetMovePow(VScale(player->GetMoveDir(), Player::POW_ROLL));
		player->SetRolling(true);
	}
	else
	{
		// 慣性の法則で減速する
		player->SetMovePow(VScale(player->GetMovePow(), 0.95f));
		player->SetRolling(false);
	}

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

	player->SetInvincible(false);

	player->GetAnimationController()->SetIgnoreRootMove(false);
}
