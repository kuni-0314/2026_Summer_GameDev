#include "PlayerMagicState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Collider/Sphere/ColliderSphere.h"

void PlayerMagicState::Enter(Player* player)
{
	// 魔法アニメーション再生
	player->GetAnimationController()->Play(
		static_cast<int>(Player::ANIM_TYPE::MAGIC), false, true);

	// ルートモーションを有効化
	player->SetApplyRootMotion(false);

	// アニメーション開始時のモデルの座標を記録
	MATRIX modelMatrix = MV1GetFrameLocalWorldMatrix(player->GetTransform().modelId, 2);
	VECTOR localPos = { modelMatrix.m[3][0], modelMatrix.m[3][1], modelMatrix.m[3][2] };
	player->SetAnimStartModelPos(localPos);

	// 魔法の種類を初期化（後で実装）
	magicType_ = MAGIC_TYPE::THUNDER;
}

void PlayerMagicState::Update(Player* player)
{
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

	// アニメーションが終了したらIdleステートに戻る
	if (player->GetAnimationController()->IsEnd())
	{
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}

void PlayerMagicState::Draw(Player* player)
{
#ifdef _DEBUG
	// デバッグ表示
	DrawFormatString(20, 100, 0xFFFFFF, "State: MAGIC");
	DrawFormatString(20, 125, 0xFFFFFF, "MagicType: %d", static_cast<int>(magicType_));
#endif
}

void PlayerMagicState::Exit(Player* player)
{
	// ルートモーションを無効化
	player->SetApplyRootMotion(false);
}
