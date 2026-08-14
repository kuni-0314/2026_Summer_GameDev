#include "player.h"
#include "../../Common/AnimationController.h"
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
