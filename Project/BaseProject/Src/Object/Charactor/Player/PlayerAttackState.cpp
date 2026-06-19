#include "PlayerAttackState.h"
#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Scene/GameScene.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Sound/AudioManager.h"

void PlayerAttackState::Enter(Player* player)
{
	// çUåÇÉ^ÉCÉvÇåàíË
	attackType_ = GetNextAttackType(player);

	// ÉãÅ[ÉgÉÇÅ[ÉVÉáÉìóLå¯âª
	player->SetApplyRootMotion(true);
	
	// ÉAÉjÉÅÅ[ÉVÉáÉìäJénéûÇÃÉÇÉfÉãÇÃÉçÅ[ÉJÉãç¿ïWÇãLò^
	MATRIX modelMatrix = MV1GetFrameLocalWorldMatrix(player->GetTransform().modelId, 2);
	VECTOR localPos = { modelMatrix.m[3][0], modelMatrix.m[3][1], modelMatrix.m[3][2] };
	player->SetAnimStartModelPos(localPos);

	// ÉNÅ[ÉãÉ^ÉCÉÄÇê›íËÅiÉAÉjÉÅÅ[ÉVÉáÉìíÜÇÕçUåÇïsâ¬Åj
	player->SetAttackCoolTime(9999);

	// ÉRÉìÉ{É^ÉCÉ}Å[ÇÉäÉZÉbÉgÅiÉAÉjÉÅÅ[ÉVÉáÉìíÜÇÕÉRÉìÉ{ïsâ¬Åj
	player->SetComboTimer(0);

	// çUåÇà íuÇåvéZÇµÇƒÉRÉâÉCÉ_ê∂ê¨
	VECTOR attackPos = CalculateAttackPosition(player);
	player->GetGameScene()->CreateAttackCollider(
		ColliderBase::TAG::PLAYER,
		attackPos,
		ATTACK_RADIUS,
		ATTACK_POW[static_cast<int>(attackType_)],
		60); 
<<<<<<< HEAD
=======
	
	//if (player->IsAir())
	//{
	//	VECTOR movePow = player->GetMovePow();
	//	movePow.y = 200.0f;
	//	player->SetMovePow(movePow);
	//}

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);

>>>>>>> origin/Œ±‰øÆÊ≠£Ôºí
}

void PlayerAttackState::Update(Player* player)
{
	// çUåÇÉAÉjÉÅÅ[ÉVÉáÉìíÜÇÕëºÇÃèÛë‘Ç÷ÇÃëJà⁄ÇÉ`ÉFÉbÉNÇµÇ»Ç¢

	// à⁄ìÆó ÇÃå∏êä
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

	// à⁄ìÆó ÇÃå∏êäÇñ≥å¯âª(ÉãÅ[ÉgÉÇÅ[ÉVÉáÉìÇ≈êßå‰)
	// É_ÉbÉVÉÖçUåÇÇ»Ç«ÇÃèÍçáÇÕÉãÅ[ÉgÉÇÅ[ÉVÉáÉìÇ≈à⁄ìÆÇï\åª

	VECTOR framePos = MV1GetFramePosition(player->GetTransform().modelId, 2);
	framePos.y = player->GetPos().y;
	player->SetPos(framePos);

	// ÉAÉjÉÅÅ[ÉVÉáÉìèIóπéû
	if (player->GetAnimationController()->IsEnd())
	{
		// ÉNÅ[ÉãÉ^ÉCÉÄÇÉNÉäÉAÅiçUåÇâ¬î\Ç…Åj
		player->SetAttackCoolTime(0);

		// ÉRÉìÉ{É^ÉCÉ}Å[Çê›íË
		if (attackType_ == ATTACK_TYPE::HEAVY || 
			attackType_ == ATTACK_TYPE::FALL)
		{
			player->SetComboTimer(0);
		}
		else
		{
			player->SetComboTimer(COMBO_WINDOW_FRAME);
		}
		
		player->ChangeState(Player::STATE::IDLE);
		return;
	}
}

void PlayerAttackState::Draw(Player* player)
{
	// ÉfÉoÉbÉOï\é¶ópÅiÉRÉÅÉìÉgÉAÉEÉgçœÇ›Åj
}

VECTOR PlayerAttackState::CalculateAttackPosition(Player* player)
{
	// ÉvÉåÉCÉÑÅ[ÇÃà íuÇ∆âÒì]ÇéÊìæ
	VECTOR playerPos = player->GetTransform().pos;
	VECTOR playerRot = player->GetTransform().rot;

	// ÉçÅ[ÉJÉãç¿ïWÇÉèÅ[ÉãÉhç¿ïWÇ…ïœä∑
	MATRIX rotMat = MGetRotY(playerRot.y);
	VECTOR worldOffset = VTransform(ATTACK_LOCAL_POS, rotMat);

	// ÉvÉåÉCÉÑÅ[ÇÃà íuÇ…â¡éZ
	return VAdd(playerPos, worldOffset);
}

PlayerAttackState::ATTACK_TYPE PlayerAttackState::GetNextAttackType(Player* player)
{
	auto ins = InputManager::GetInstance();

	// ì¸óÕÉfÉoÉCÉXÇÃéÌóﬁÇîªíË
	bool isGamepadConnected = (GetJoypadNum() > 0);
	constexpr int GAMEPAD_INDEX = 0;

	// í∑âüÇµîªíË
	bool isHeavyAttack = false;
	if (isGamepadConnected)
	{
		isHeavyAttack = ins->GetGamepadLastHoldTime(static_cast<int>(InputManager::PadInput::X), GAMEPAD_INDEX) > 30;
	}
	else
	{
		isHeavyAttack = ins->GetMouseLastHoldTime(MOUSE_INPUT_LEFT) > 30;
	}

	if (isHeavyAttack)
	{
	
		

		if (player->IsAir())
		{
			// ÉAÉjÉÅÅ[ÉVÉáÉìçƒê∂
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::FALL;
			AudioManager::GetInstance()->SetSeVolume(100);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
		else
		{
			// ÉAÉjÉÅÅ[ÉVÉáÉìçƒê∂
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_H), false, true);
			return ATTACK_TYPE::HEAVY;
			AudioManager::GetInstance()->SetSeVolume(100);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
	}

	// É_ÉbÉVÉÖçUåÇîªíË
<<<<<<< HEAD
	//bool isDashAttack = false;
	//if (!player->IsAir())
	//{
	//	if (isGamepadConnected)
	//	{
	//		// ÉQÅ[ÉÄÉpÉbÉhÅFç∂ÉXÉeÉBÉbÉNì¸óÕíÜ
	//		short stickX = 0, stickY = 0;
	//		ins->GetLeftStick(GAMEPAD_INDEX, stickX, stickY);

	//		constexpr float STICK_DEADZONE = 0.2f;
	//		constexpr float STICK_MAX = 32767.0f;
	//		float normalizedX = stickX / STICK_MAX;
	//		float normalizedY = stickY / STICK_MAX;

	//		isDashAttack = (abs(normalizedX) > STICK_DEADZONE || abs(normalizedY) > STICK_DEADZONE);
	//	}
	//	else
	//	{
	//		// ÉLÅ[É{Å[ÉhÅFÉVÉtÉg + WASD
	//		isDashAttack = ins->IsNew(KEY_INPUT_LSHIFT) &&
	//			(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
	//			 ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D));
	//	}
	//}

	//if (isDashAttack)
	//{
	//	// ÉAÉjÉÅÅ[ÉVÉáÉìçƒê∂
	//	player->GetAnimationController()->Play(
	//		static_cast<int>(Player::ANIM_TYPE::ATK_D), false, true);
	//	return ATTACK_TYPE::DASH;
	//}
=======
	if (!player->IsAir() && 
		ins->IsNew(KEY_INPUT_LSHIFT) &&
		(ins->IsNew(KEY_INPUT_W) || ins->IsNew(KEY_INPUT_A) || 
		 ins->IsNew(KEY_INPUT_S) || ins->IsNew(KEY_INPUT_D)))
	{


		// ÉAÉjÉÅÅ[ÉVÉáÉìçƒê∂
		player->GetAnimationController()->Play(
			static_cast<int>(Player::ANIM_TYPE::ATK_D), false, true);

		AudioManager::GetInstance()->SetSeVolume(100);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);

		return ATTACK_TYPE::DASH;

	
	
	}
>>>>>>> origin/Œ±‰øÆÊ≠£Ôºí

	// ÉRÉìÉ{åpë±îªíË
	bool inCombo = player->GetComboTimer() > 0;

	if (player->IsAir())
	{
		// ãÛíÜÉRÉìÉ{
		if (!inCombo)
		{
			// ÉAÉjÉÅÅ[ÉVÉáÉìçƒê∂
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A1), false, true);
			return ATTACK_TYPE::AIR1;
		}

		switch (attackType_)
		{
		case ATTACK_TYPE::AIR1:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A2), false, true);
			return ATTACK_TYPE::AIR2;
		case ATTACK_TYPE::AIR2:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A3), false, true);
			return ATTACK_TYPE::AIR3;
		case ATTACK_TYPE::AIR3:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A4), false, true);
			return ATTACK_TYPE::AIR4;
		case ATTACK_TYPE::AIR4:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A5), false, true);
			return ATTACK_TYPE::AIR5;
		default:
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_A1), false, true);
			return ATTACK_TYPE::AIR1;
		}
	}
	else
	{
		// ínè„ÉRÉìÉ{
		if (!inCombo)
		{
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
			AudioManager::GetInstance()->SetSeVolume(150);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
		}
		switch (attackType_)
		{
		case ATTACK_TYPE::NORMAL1: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N2), false, true);
			AudioManager::GetInstance()->SetSeVolume(150);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_ATTACK_1);
			return ATTACK_TYPE::NORMAL2;
		case ATTACK_TYPE::NORMAL2: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N3), false, true);
			return ATTACK_TYPE::NORMAL3;
		case ATTACK_TYPE::NORMAL3: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N4), false, true);
			return ATTACK_TYPE::NORMAL4;
		case ATTACK_TYPE::NORMAL4: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N5), false, true);
			return ATTACK_TYPE::NORMAL5;
		default: 
			player->GetAnimationController()->Play(
				static_cast<int>(Player::ANIM_TYPE::ATK_N1), false, true);
			return ATTACK_TYPE::NORMAL1;
		}
	}
}
