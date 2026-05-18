#include "../../../Manager/InputManager.h"
#include "Player.h"
#include "../../../Object/Common/AnimationController.h"
#include "PlayerIdleState.h"

void PlayerIdleState::Enter(Player* player)
{
	// 初期アニメーション再生
	player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::IDLE), true);

	// 移動量をゼロにする
	VECTOR movePow = {0, 0, 0};
	player->SetMovePow(movePow);
}

void PlayerIdleState::Update(Player* player)
{
	auto ins = InputManager::GetInstance();

	// ジャンプキーが入力されているか
	if (!player->IsJump() && ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		player->ChangeState(Player::STATE::JUMP);
		return;
	}

	// 移動キーが入力されているか
	if (ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D))
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
