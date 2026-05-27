#include "PlayerAttackState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"

void PlayerAttackState::Enter(Player* player)
{
	// 攻撃アニメーション再生
	 player->GetAnimationController()->Play(static_cast<int>(Player::ANIM_TYPE::ATTACK), false);

	 if (player->IsAir())
	 {
		 player->SetMovePow({ 0.0f, 100.0f, 0.0f });
	 }


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

	// アニメーションが終了したらIDLE状態に戻る
	 if (player->GetAnimationController()->IsEnd())
	 {
	 	player->ChangeState(Player::STATE::IDLE);
	 	return;
	 }
}
