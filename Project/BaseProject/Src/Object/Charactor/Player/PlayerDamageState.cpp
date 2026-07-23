#include "Player.h"
#include "../../Common/AnimationController.h"
#include "../../../Utility/AsoUtility.h"
#include "PlayerDamageState.h"

void PlayerDamageState::Enter(Player* player)
{
	auto anim = player->GetAnimationController();
	anim->Play(static_cast<int>(Player::ANIM_TYPE::DAMAGE), false);
}

void PlayerDamageState::Update(Player* player)
{
	// ˆÚ“®—Ê‚ÌŒ¸Š
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

	auto anim = player->GetAnimationController();

	if (anim->IsEnd())
	{
		player->ChangeState(Player::STATE::IDLE);
	}
}