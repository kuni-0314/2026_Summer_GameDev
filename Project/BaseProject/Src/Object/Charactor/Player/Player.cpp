#include "Player.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Manager/Camera.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "../../../Object/Collider/Line/ColliderLine.h"
#include "../../../Object/Collider/Capsule/ColliderCapsule.h"
#include "../../../Object/Collider/Sphere/ColliderSphere.h"
#include "../../Weapon/Sword/KeyBlade1.h"
#include "../../Weapon/Sword/KeyBlade2.h"
#include "../../Weapon/Sword/KeyBlade3.h"
#include "../../../Effect/LoadEffekseer/EffekseerEffect.h"
#include "../../../Effect/EffectManager.h"
#include "../../../Sound/AudioManager.h"
#include "../../../Sound/SoundTable.h"
#include "../../../Scene/GameScene.h"
#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerFastRunState.h"
#include "PlayerJumpState.h"
#include "PlayerJetState.h"
#include "PlayerFallState.h"
#include "PlayerAttackState.h"
#include "PlayerMagicState.h"
#include "../../Charactor/Enemy/Dragon/EnemyDragon.h"


Player::Player(int padNum)
	:
	padNum_(padNum),
	CharactorBase()
{
	weight_ = WEIGHT::NORMAL;
}

Player::~Player()
{
	for (auto& state : states_)
	{
		delete state.second;
	}
	states_.clear();
}

void Player::Update()
{
	// ˆÚ“®‘OÀ•W‚ğXV
	prevPos_ = transform_.pos;

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€‚ğŒ¸Z
	if (attackCoolTime_ > 0)
	{
		attackCoolTime_--;
	}

	// ƒRƒ“ƒ{ƒ^ƒCƒ}[‚ğŒ¸Z
	if (comboTimer_ > 0)
	{
		comboTimer_--;
	}
<<<<<<< HEAD
	
=======

<<<<<<< HEAD
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
	// ƒq[ƒ‹‚ÌŠÔ‚ğŒ¸Z
	if (recoveryEffect_)
	{
		recoveryEffect_->SetPosition(transform_.pos);
<<<<<<< HEAD
	}

	if (tornadoDamageCoolTime_ > 0)
	{
		tornadoDamageCoolTime_--;
=======
=======
	if (tornadoDamageCoolTime_ > 0)
	{
		tornadoDamageCoolTime_--;
>>>>>>> origin/FIXï¼’
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
	}

	// ŠeƒLƒƒƒ‰ƒNƒ^[‚²‚Æ‚ÌXVˆ—
	UpdateProcess();
	if (!m_isPowerUp)
	{
		// ˆÚ“®•ûŒü‚É‰‚¶‚Ä™X‚É‰ñ“]
		DelayRotate();
		// d—Í‚É‚æ‚éˆÚ“®—Ê
		CalcGravityPow();
		// Õ“Ë”»’è‘O€”õ
		CollisionReserve();
		// Õ“Ë”»’è
		Collision();
		// ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
		animationController_->Update();
	}
	// ƒ‚ƒfƒ‹§ŒäXV
	transform_.Update();
	// ŠeƒLƒƒƒ‰ƒNƒ^[‚²‚Æ‚ÌXVŒãˆ—
	UpdateProcessPost();


	// hp‚Ìindex‚Í4Aluck‚Ìindex‚Í9
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_UP))
	//{
	//	if (currentGrantStatusIndex_ <= 4)
	//	{
	//		currentGrantStatusIndex_ = MAX_STATUS_INDEX;
	//	}
	//	else
	//	{
	//		currentGrantStatusIndex_--;
	//	}
	//}
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_DOWN))
	//{
	//	if (currentGrantStatusIndex_ >= MAX_STATUS_INDEX)
	//	{
	//		currentGrantStatusIndex_ = 4;
	//	}
	//	else
	//	{
	//		currentGrantStatusIndex_++;
	//	}
	//}

	//// –îˆóƒL[‰E‚ÅƒXƒe[ƒ^ƒX‚Éƒ|ƒCƒ“ƒgŠ„‚èU‚è
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_RIGHT))
	//{
	//	if (pendingPoints_ > 0)
	//	{
	//		GrantStatus(currentGrantStatusIndex_);
	//		pendingPoints_--;
	//	}
	//}

	//// –îˆóƒL[¶‚ÅƒXƒe[ƒ^ƒX‚©‚çƒ|ƒCƒ“ƒg‚ğ–ß‚·
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_LEFT))
	//{
	//	RevokeStatus(currentGrantStatusIndex_);
	//}



}

//void Player::Damage(int damage)
//{
//	hp_ -= damage;
//	//int a = StartJoypadVibration(padNum_ + 1, 1000, 500, -1);
//	//VibrateGamepad(int gamepadIndex, int power, int time)
//	InputManager::GetInstance()->VibrateGamepad(padNum_ + 1, 1000, 500);
//	if (hp_ <= 0)
//	{
//		hp_ = 0;
//	}
//}

void Player::Damage(int damage, const VECTOR& hitDir)
{
	if (isInvincible_) return;

	auto dir = VNorm(hitDir);
	knockbackPow_ = VScale(dir, 4.0f);

	hp_ -= damage;
	if (hp_ <= 0)
	{
		hp_ = 0;

		return;
	}

	gameScene_->ShakeHpUI();

	// Œ´‘¥‚Æ‚µ‚Ä–³“GƒtƒŒ[ƒ€‚ğİ’è‚·‚é
	invincibleFrameCount_ = INVINCIBLE_FRAME_COUNT;
}

void Player::HealHp(int heal)
{
	hp_ += heal;
	if (hp_ > MAX_HP)
	{
		hp_ = MAX_HP;
	}
}

void Player::InitLoad()
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));

	//Šî’êƒNƒ‰ƒX‚ÌƒŠƒ\[ƒXƒ[ƒh
	CharactorBase::InitLoad();
}

void Player::InitTransform()
{

	//‘å‚«‚³AÀ•W“™‚Ì‰Šú‰»
	transform_.scl = { SCL_PlAYER ,SCL_PlAYER ,SCL_PlAYER };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT_LOCAL_PLAYER);
	transform_.pos = POS_PLAYER;


	transform_.Update();

}

void Player::InitCollider()
{
	// å‚É’n–Ê‚Æ‚ÌÕ“Ë‚Åg—p‚·‚éü•ªƒRƒ‰ƒCƒ_
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// å‚É•Ç‚â–Ø‚È‚Ç‚ÌÕ“Ë‚Åg—p‚·‚éƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}

void Player::InitAnimation()
{
	//ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[
	animationController_ = new AnimationController(transform_.modelId);

	auto& anim = animationController_;

	// ‘Ò‹@ó‘ÔƒAƒjƒ[ƒVƒ‡ƒ“
	anim->Add(static_cast<int>(ANIM_TYPE::IDLE)
		, 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");

	// ‘–‚éƒAƒjƒ[ƒVƒ‡ƒ“
	anim->Add(static_cast<int>(ANIM_TYPE::RUN)
		, 20.0f, Application::PATH_MODEL + "Player/Walk.mv1");

	// ƒ_ƒbƒVƒ…ƒAƒjƒ[ƒVƒ‡ƒ“
	anim->Add(static_cast<int>(ANIM_TYPE::FAST_RUN)
		, 40.0f, Application::PATH_MODEL + "Player/Run.mv1");
	//ƒWƒƒƒ“ƒvƒAƒjƒ[ƒVƒ‡ƒ“
	anim->Add(static_cast<int>(ANIM_TYPE::JUMP)
		, 60.0f, Application::PATH_MODEL + "Player/Jump.mv1");

	// UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_N1)
		, 100.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_N2)
		, 65.0f, Application::PATH_MODEL + "Player/Attack2.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_N3)
<<<<<<< HEAD
=======
=======
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N1)
		, 90.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N2)
		, 90.0f, Application::PATH_MODEL + "Player/Attack2.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N3)
>>>>>>> origin/FIXï¼’
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
		, 90.0f, Application::PATH_MODEL + "Player/Attack3.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_N4)
		, 75.0f, Application::PATH_MODEL + "Player/Attack4.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_N5)
		, 90.0f, Application::PATH_MODEL + "Player/Attack5.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_H)
		, 100.0f, Application::PATH_MODEL + "Player/Heavy.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_D)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_A1)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_A2)
		, 70.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_A3)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_A4)
		, 50.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_A5)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::ATK_F)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	anim->Add(static_cast<int>(ANIM_TYPE::MAGIC)
		, 40.0f, Application::PATH_MODEL + "Player/Spell Cast.mv1");
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
	anim->Add(static_cast<int>(ANIM_TYPE::ROLLING)
		, 65.0f, Application::PATH_MODEL + "Player/Rolling.mv1");
	anim->Add(static_cast<int>(ANIM_TYPE::DAMAGE)
		, 100.0f, Application::PATH_MODEL + "Player/Damage.mv1");
<<<<<<< HEAD
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL_END)
		, 85.0f, Application::PATH_MODEL + "Player/FallingLanding.mv1");
=======
=======
	animationController_->Add(static_cast<int>(ANIM_TYPE::ROLLING)
		, 65.0f, Application::PATH_MODEL + "Player/R.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL_END)
		, 85.0f, Application::PATH_MODEL + "Player/FallingLanding.mv1");
>>>>>>> origin/FIXï¼’
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb

	//‰ŠúƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	anim->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Player::InitPost()
{
	status_.level = DEFAULT_LEVEL;
	status_.hp = DEFAULT_HP;
	status_.mp = DEFAULT_MP;
	status_.physAtk = DEFAULT_PHYS_ATK;
	status_.physDef = DEFAULT_PHYS_DEF;
	status_.magicAtk = DEFAULT_MAGIC_ATK;
	status_.magicDef = DEFAULT_MAGIC_DEF;
	status_.wisdom = DEFAULT_WISDOM;
	status_.luck = DEFAULT_LUCK;
	currentGrantStatusIndex_ = 4;
	pendingPoints_ = 30;

	hp_ = MAX_HP;

	InitState();

	// •Ší‰Šú‰»
	sword_ = new KeyBlade3(KEY_BLADE_3_LOCAL_POS_START, KEY_BLADE_3_LOCAL_POS_END, KEY_BLADE_3_RADIUS, transform_);
	sword_->Init();

}

void Player::UpdateProcess()
{
	// ƒpƒ[ƒAƒbƒv’†
	if (m_isPowerUp)
	{
		m_powerUpTimer--;

		if (m_powerUpTimer <= 0)
		{
			m_isPowerUp = false;
		}
	}
	else
	{
		if (currentState_ != nullptr)
		{
			currentState_->Update(this);
		}
	}

	auto ins = InputManager::GetInstance();
	if (ins->IsTrgDown(KEY_INPUT_R))
	{
		transform_.pos = POS_PLAYER;
	}

	if (ins->IsTrgDown(KEY_INPUT_P))
	{
		ActivatePowerUp();
	}

	if (ins->IsTrgDown(KEY_INPUT_B))
	{
		PlayBlinkEffect();
	}

	// ƒVƒ‡[ƒgƒJƒbƒgƒL[”»’è
	if (ins->IsGamepadNew(InputManager::PadInput::LB, padNum_))
	{
		isShortCut_ = true;
	}
	else
	{
		isShortCut_ = false;
	}

	// –‚–@ŠJn
	int activeMagicCount = 0;
	if (isShortCut_ && ins->IsGamepadTrgDown(InputManager::PadInput::Y, padNum_))
	{
		activeMagicCount = 1;
	}
	if (isShortCut_ && ins->IsGamepadTrgDown(InputManager::PadInput::X, padNum_))
	{
		activeMagicCount = 2;
	}
	if (isShortCut_ && ins->IsGamepadTrgDown(InputManager::PadInput::A, padNum_))
	{
		activeMagicCount = 3;
	}

	if ((ins->IsTrgDown(KEY_INPUT_E) || ins->IsGamepadTrgDown(InputManager::PadInput::Y, padNum_)))
	{
		GameScene* gameScene = dynamic_cast<GameScene*>(scnMng_.GetScene());
		switch (gameScene->GetSelectedCommand())
		{
		case GameScene::COMMAND::FIRE:
			activeMagicCount = 1;
			break;
		case GameScene::COMMAND::THUNDER:
			activeMagicCount = 2;
			break;
		case GameScene::COMMAND::HEAL:
			activeMagicCount = 3;
			break;
		default:
			break;
		}
	}

	switch (activeMagicCount)
	{
	case 1:
		if (fireCoolTime_ <= 0)
		{
			CreateFireMagic();
		}
		else
		{
			AudioManager::GetInstance()->PlaySE(SoundID::SE_NOT_MAGIC);
		}
		break;
	case 2:
		if (thunderCoolTime_ <= 0)
		{
			CreateThunderMagic();
		}
		else
		{
			AudioManager::GetInstance()->PlaySE(SoundID::SE_NOT_MAGIC);
		}
		break;
	case 3:
		if (healCoolTime_ <= 0)
		{
			CreateHealMagic();
		}
		else
		{
			AudioManager::GetInstance()->PlaySE(SoundID::SE_NOT_MAGIC);
		}
		break;
	default:
		break;
	}

	// –‚–@ˆ—
	UpdateMagic();

	CheckPlayerRingCollision();
	//ƒhƒ‰ƒSƒ“ƒuƒŒƒX“–‚½‚è”»’è
	DragonBreathCheckCollision();
	//ƒhƒ‰ƒSƒ“ƒgƒ‹ƒl[ƒh“–‚½‚è”»’è
	DragonTornadoCheckCollision();

}
void Player::UpdateProcessPost()
{
	if (sword_ != nullptr)
	{
		sword_->Update();
	}

}

void Player::Draw()
{
	//Šî’êƒNƒ‰ƒX‚Ì•`‰æˆ—
	ActorBase::Draw();
	// ŠÛ‰e‚Ì•`‰æ
	DrawShadow();

	// ƒvƒŒƒCƒ„[‚Ì•Ší•`‰æ
	if (sword_ != nullptr)
	{
		sword_->Draw();
	}
#ifdef _DEBUG
	// ƒXƒe[ƒ^ƒX•`‰æ
	int x = 20;
	int y = 20;
	int lineHeight = 25;

	// ƒ^ƒCƒgƒ‹
	//DrawFormatString(x, y, 0xFFFFFF, "=== Player Status ===");
	y += lineHeight;

	currentState_->Draw(this);

	// ƒXƒe[ƒ^ƒXî•ñ‚ğ•`‰æi‘I‘ğ’†‚Ì€–Ú‚ğ‰©F‚ÅƒnƒCƒ‰ƒCƒgj
	unsigned int color = 0xFFFFFF;
	unsigned int highlightColor = 0xFFFF00;


	//y += lineHeight;
	DrawFormatString(x, y, 0xffffff, "jumpPow   : %f.", jumpPow_.y);

	VECTOR lineStart = transform_.pos;
	VECTOR lineEnd = { transform_.pos.x, transform_.pos.y + jumpPow_.y * 10.0f, transform_.pos.z };
	DrawLine3D(lineStart, lineEnd, 0xFFFFFF);
	DrawSphere3D(lineEnd, 5.0f, 16, 0xFF00FF, 0xFF00FF, true);

	// ƒvƒŒƒCƒ„[‚Ìü‚è‚ğ‰ñ“]‚·‚éƒIƒuƒWƒFƒNƒg‚Ì•`‰æ
	static float angle = 0.0f;
	angle += 0.01f;

	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚ğ’†S‚É”¼Œa100.0f‚Å‰ñ“]
	float radius = 100.0f;
	VECTOR rot = transform_.pos;
	rot.x += sinf(angle) * radius;
	rot.z += cosf(angle) * radius;

	// ƒvƒŒƒCƒ„[‚ÌÀ•WŒn‚ğ•`‰æ
	VECTOR lineX = { transform_.pos.x + 50.0f, transform_.pos.y, transform_.pos.z };
	VECTOR lineY = { transform_.pos.x, transform_.pos.y + 50.0f, transform_.pos.z };
	VECTOR lineZ = { transform_.pos.x, transform_.pos.y, transform_.pos.z + 50.0f };
	DrawLine3D(transform_.pos, lineX, 0xFF0000);
	DrawLine3D(transform_.pos, lineY, 0x00FF00);
	DrawLine3D(transform_.pos, lineZ, 0x0000FF);
	DrawSphere3D(lineX, 5.0f, 16, 0xFF0000, 0xFF0000, true);
	DrawSphere3D(lineY, 5.0f, 16, 0x00FF00, 0x00FF00, true);
	DrawSphere3D(lineZ, 5.0f, 16, 0x0000FF, 0x0000FF, true);
	DrawSphere3D(rot, 5.0f, 16, 0xFFFF00, 0xFFFF00, true);

	//debugPos_‚Ì‹…‘Ì•`‰æ(‘å‚«‚³‚Í—‰ºUŒ‚‚Ì”»’è—p‚Ì”¼Œa‚Æ“¯‚¶)
	DrawSphere3D(debugPos_, ATTACK_RANGE, 16, 0x00FFFF, 0x00FFFF, false);

	VECTOR test = transform_.quaRot.PosAxis(VGet(0, 0, -100));
	//DrawFormatString(0, 500, 0xffffff, "<Player> HP : %d", hp_);
#endif // _DEBUG
}

void Player::ChangeState(STATE newState)
{
	currentState_->Exit(this);
	currentState_ = states_[newState];
	currentState_->Enter(this);
}

bool Player::IsShortCut()
{
	return isShortCut_;
}

void Player::ActivatePowerUp()
{
	if (m_isPowerUp) return;

	m_isPowerUp = true;

	// ƒGƒtƒFƒNƒgÄ¶ŠÔ
	m_powerUpTimer = 60; // –ñ1•b(60FPS‚Ìê‡)

	VECTOR effectPos = transform_.pos;
	// ƒGƒtƒFƒNƒg‚ÌÄ¶ˆÊ’u‚ğƒvƒŒƒCƒ„[‚ÌÀ•W‚Éİ’è
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/PowerUp/PowerUp2.efkefc",
		effectPos
	);
	// ƒGƒtƒFƒNƒg‚ğÄ¶
	effect->Play(
		effectPos,
		transform_.quaRot
	);

	EffectManager::GetInstance().RegisterEffect(effect);
}

void Player::PlayBlinkEffect()
{
	// ƒGƒtƒFƒNƒgÄ¶ŠÔ
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Bring/Blink.efkefc",
		transform_.pos
	);
	// ƒGƒtƒFƒNƒg‚ÌÄ¶ˆÊ’u‚ğƒvƒŒƒCƒ„[‚Ì”wŒã‚Éİ’è
	VECTOR back = transform_.quaRot.GetBack();
	// 50.0f‚Ì‹——£‚ğ”wŒã‚Éİ’è
	VECTOR backPos = VScale(
		back,
		50.0f
	);
	// ƒvƒŒƒCƒ„[‚ÌÀ•W‚É”wŒã‚ÌˆÊ’u‚ğ‰ÁZ
	backPos = VAdd(
		transform_.pos,
		backPos
	);
	// ƒGƒtƒFƒNƒg‚ğÄ¶
	effect->Play(
		backPos,
		transform_.quaRot
	);
	// ƒGƒtƒFƒNƒg‚ğƒGƒtƒFƒNƒgƒ}ƒl[ƒWƒƒ[‚É“o˜^
	EffectManager::GetInstance().RegisterEffect(effect);
}

void Player::DeleteFireEffect()
{
	if (!isAliveFire_) return;

	VECTOR pos = fireInfo_.transform.pos;

	//------------------------------------
	// FireƒGƒtƒFƒNƒg’â~
	//------------------------------------
	if (fireInfo_.effect)
	{
		fireInfo_.effect->Stop();
		fireInfo_.effect.reset();
	}

	//------------------------------------
	// Burst¶¬
	//------------------------------------
	auto burst = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Fire/Burst.efkefc",
		pos
	);

	burst->Play(
		pos,
		Quaternion()
	);

	burst->SetLifeTime(60);

	EffectManager::GetInstance().RegisterEffect(burst);

	//------------------------------------
	// ’eíœ
	//------------------------------------
	isAliveFire_ = false;
}

void Player::CheckPlayerRingCollision()
{
	//// ƒvƒŒƒCƒ„[‚ÌƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_‚ğæ“¾
	//ColliderCapsule* playerCollider =
	//	dynamic_cast<ColliderCapsule*>(ownColliders_[static_cast<int>(COLLIDER_TYPE::CAPSULE)]);

	//if (playerCollider == nullptr) return;

	////Õ“Ëî•ñXV
	//MV1RefreshCollInfo(playerCollider->GetFollow()->modelId);

	//// “o˜^‚³‚ê‚Ä‚¢‚éÕ“Ë•¨‚ğ‘S‚Äƒ`ƒFƒbƒN
	//bool isHit = false;
	//for (const auto& hitCol : hitColliders_)
	//{
	//	if (hitCol->GetTag() != ColliderBase::TAG::ENEMY_RING) continue;

	//	// ”h¶ƒNƒ‰ƒX‚ÖƒLƒƒƒXƒg
	//	const ColliderModel* ringCollider =
	//		dynamic_cast<const ColliderModel*>(hitCol);

	//	if (ringCollider == nullptr) continue;
	//	//ƒ‚ƒfƒ‹‚ÆƒJƒvƒZƒ‹‚Ì”“Ë”»’è
	//	auto hits = MV1CollCheck_Capsule(
	//		ringCollider->GetFollow()->modelId, -1,
	//		playerCollider->GetPosTop(), playerCollider->GetPosDown(), playerCollider->GetRadius());

	//	// Õ“Ë‚µ‚½•¡”‚Ìƒ|ƒŠƒSƒ“‚ÆÕ“Ë‰ñ”ğ‚·‚é‚Ü‚ÅAˆÊ’u‚ğˆÚ“®‚³‚¹‚é
	//	for (int i = 0; i < hits.HitNum; i++)
	//	{
	//		auto hitPoly = hits.Dim[i];
	//		isHit = true;
	//		break;
	//	}
	//	// ŒŸo‚µ‚½’n–Êƒ|ƒŠƒSƒ“î•ñ‚ÌŒãn––
	//	MV1CollResultPolyDimTerminate(hits);
	//}

	//if (isHit) Damege(1);
}

void Player::GrantStatus(int index)
{
	switch (index)
	{
	case 4:
		status_.physAtk += 1;
		break;
	case 5:
		status_.physDef += 1;
		break;
	case 6:
		status_.magicAtk += 1;
		break;
	case 7:
		status_.magicDef += 1;
		break;
	case 8:
		status_.wisdom += 1;
		break;
	case 9:
		status_.luck += 1;
		break;
	default:
		break;
	}
}

void Player::RevokeStatus(int index)
{
	// ŠeƒXƒe[ƒ^ƒX‚ª‰Šú’lˆÈ‰º‚É‚È‚ç‚È‚¢‚æ‚¤‚Éƒ`ƒFƒbƒN
	switch (index)
	{
	case 2:
		if (status_.hp > DEFAULT_HP)
		{
			status_.hp -= 1;
			pendingPoints_++;
		}
		break;
	case 3:
		if (status_.mp > DEFAULT_MP)
		{
			status_.mp -= 1;
			pendingPoints_++;
		}
		break;
	case 4:
		if (status_.physAtk > DEFAULT_PHYS_ATK)
		{
			status_.physAtk -= 1;
			pendingPoints_++;
		}
		break;
	case 5:
		if (status_.physDef > DEFAULT_PHYS_DEF)
		{
			status_.physDef -= 1;
			pendingPoints_++;
		}
		break;
	case 6:
		if (status_.magicAtk > DEFAULT_MAGIC_ATK)
		{
			status_.magicAtk -= 1;
			pendingPoints_++;
		}
		break;
	case 7:
		if (status_.magicDef > DEFAULT_MAGIC_DEF)
		{
			status_.magicDef -= 1;
			pendingPoints_++;
		}
		break;
	case 8:
		if (status_.wisdom > DEFAULT_WISDOM)
		{
			status_.wisdom -= 1;
			pendingPoints_++;
		}
		break;
	case 9:
		if (status_.luck > DEFAULT_LUCK)
		{
			status_.luck -= 1;
			pendingPoints_++;
		}
		break;
	default:
		break;
	}
}

void Player::InitState()
{
	states_[STATE::IDLE] = new PlayerIdleState();
	states_[STATE::RUN] = new PlayerRunState();
	//states_[STATE::FAST_RUN] = new PlayerFastRunState();
	states_[STATE::JUMP] = new PlayerJumpState();
	states_[STATE::JET] = new PlayerRollState();
	states_[STATE::FALL] = new PlayerFallState();
	states_[STATE::ATTACK] = new PlayerAttackState();
	states_[STATE::MAGIC] = new PlayerMagicState();  // ’Ç‰Á
	currentState_ = states_[STATE::IDLE];
}

void Player::CreateThunderCollider(ThunderInfo& thunderInfo)
{
	ColliderSphere* colSphere = new ColliderSphere(
		ColliderBase::TAG::PLAYER_MAGIC, &thunderInfo.transform,
		{ 0.0f,0.0f,0.0f }, THUNDER_RADIUS);
	thunderInfo.collider = colSphere;
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->AddEnemyHitCollider(colSphere);
}

void Player::DestroyThunderCollider(const ThunderInfo& thunderInfo)
{
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->RemoveEnemyHitCollider(thunderInfo.collider);
}

void Player::CreateFireCollider(FireInfo& fireInfo)
{
	ColliderSphere* colSphere = new ColliderSphere(
		ColliderBase::TAG::PLAYER_MAGIC, &fireInfo.transform,
		{ 0.0f,0.0f,0.0f }, FIRE_RADIUS);
	fireInfo.collider = colSphere;
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->AddEnemyHitCollider(colSphere);
}

void Player::DestroyFireCollider(const FireInfo& fireInfo)
{
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->RemoveEnemyHitCollider(fireInfo.collider);
}

void Player::CreateFireMagic()
{
	if (!isAliveFire_)
	{
		// ‰Šúó‘Ô‚ÉƒŠƒZƒbƒg
		fireInfo_ = FireInfo();
		fireInfo_.timer = 0;
		fireInfo_.transform.pos = transform_.pos;
		fireInfo_.dir = transform_.quaRot.GetForward();
		CreateFireCollider(fireInfo_);
		isAliveFire_ = true;
		fireCoolTime_ = FIRE_COOL_TIME;
		// ƒtƒ@ƒCƒAƒGƒtƒFƒNƒg
		auto effect = std::make_shared<EffekseerEffect>(
			L"Data/Effect/Fire/Fire.efkefc",
			fireInfo_.transform.pos
		);
		EffectManager::GetInstance().RegisterEffect(effect);
		//Ä¶
		effect->Play(
			fireInfo_.transform.pos,
			fireInfo_.transform.quaRot
		);
	}
}

void Player::CreateThunderMagic()
{
	if (!isAliveThunder_)
	{
		thunderTimer_ = 0;
		isAliveThunder_ = true;

		for (int i = 0; i < THUNDER_COUNT; i++)
		{
			// Šp“x”ÍˆÍ‚ğ‹·‚­: -60“x‚©‚ç+60“x(‘O•û120“x‚Ì”ÍˆÍ)
			float randomAngle = (GetRand(120) - 60) * DX_PI_F / 180.0f;

			const float DIST_MAX = 700.0f;

			float randomRadius =
				sqrtf(GetRand(100) / 100.0f) * DIST_MAX;

			VECTOR randomPos =
			{
				sinf(randomAngle) * randomRadius,
				0.0f,
				cosf(randomAngle) * randomRadius
			};

			randomPos = transform_.quaRot.PosAxis(randomPos);

			thunderInfos_[i].transform.pos =
				VAdd(transform_.pos, randomPos);
			thunderInfos_[i].timer = 0;
			thunderInfos_[i].isActive = false;
			thunderInfos_[i].isDestroyed = false;
		}

		thunderCoolTime_ = THUNDER_COOL_TIME;
	}
}

void Player::CreateHealMagic()
{
	AudioManager::GetInstance()->PlaySE(SoundID::SE_MAGIC_HEAL);
	HealHp(HEAL_AMOUNT);
	healCoolTime_ = HEAL_COOL_TIME;

	//ƒq[ƒ‹ƒGƒtƒFƒNƒg
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Heal/Heal.efkefc",
		transform_.pos
	);

	effect->SetLifeTime(60);

	effect->Play(
		transform_.pos,
		transform_.quaRot
	);

	EffectManager::GetInstance().RegisterEffect(effect);
}

void Player::MagicCoolTime()
{
	if (thunderCoolTime_ > 0)
	{
		thunderCoolTime_--;
	}

	if (fireCoolTime_ > 0)
	{
		fireCoolTime_--;
	}

	if (healCoolTime_ > 0)
	{
		healCoolTime_--;
	}
}

int Player::GetThunderCoolTime()
{
	return thunderCoolTime_;
}

int Player::GetFireCoolTime()
{
	return fireCoolTime_;
}

int Player::GetHealCoolTime()
{
	return healCoolTime_;
}

void Player::ExecuteRangeAttack()
{
	// ­‚µ‘O•û‚ÉˆÚ“®‚µ‚Ä‚©‚çUŒ‚”»’è‚ğs‚¤
	auto attackPos = VAdd(transform_.pos, VScale(transform_.quaRot.GetForward(), 50.0f));
	const int DAMAGE = 3;
	debugPos_ = attackPos;
	gameScene_->CheckHitEnemy(attackPos, ATTACK_RANGE, DAMAGE);
}

void Player::UpdateMagic()
{
	MagicCoolTime();

	if (isAliveThunder_)
	{
		thunderTimer_++;

		bool alive = false;

		for (int i = 0; i < THUNDER_COUNT; i++)
		{
			if (thunderInfos_[i].isActive)
			{
				if (thunderInfos_[i].isDestroyed)
				{
					continue;
				}

				if (thunderInfos_[i].timer >= THUNDER_LIFETIME)
				{
					DestroyThunderCollider(thunderInfos_[i]);
					thunderInfos_[i].isDestroyed = true;
				}
				else
				{
					thunderInfos_[i].timer++;
					alive = true;
				}
			}
			else
			{
				if (thunderTimer_ >= i * THUNDER_INTERVAL)
				{
					thunderInfos_[i].isActive = true;

					CreateThunderCollider(thunderInfos_[i]);

					AudioManager::GetInstance()
						->PlaySE(SoundID::SE_THUNDER);

					alive = true;

					VECTOR pos = thunderInfos_[i].transform.pos;

					auto effect = std::make_shared<EffekseerEffect>(
						L"Data/Effect/Thunder/Thunder.efkefc",
						pos
					);

					effect->SetLifeTime(30);

					effect->Play(
						pos,
						Quaternion()
					);

					EffectManager::GetInstance().RegisterEffect(effect);

					thunderInfos_[i].effect = effect;
				}
			}
		}

		// ‘S•”I—¹
		if (!alive)
		{
			isAliveThunder_ = false;
		}

		for (int i = 0; i < THUNDER_COUNT; i++)
		{
			if (thunderInfos_[i].isActive)
			{
				thunderInfos_[i].transform.Update();
			}
		}
	}

	if (isAliveFire_)
	{
		//ƒJƒƒ‰ƒ‚[ƒh‚ğæ“¾
		GameScene::CAM_MODE mode = gameScene_->GetCamMode();
		if (fireInfo_.timer < FIRE_LIFETIME)
		{
			//ƒƒbƒNƒIƒ“‚ªON‚¾‚Á‚½ê‡
			if (mode == GameScene::CAM_MODE::TARGETING)
			{
				VECTOR targetPos = gameScene_->GetTargetPos();
				VECTOR vec = VSub(targetPos, fireInfo_.transform.pos);
				VECTOR dir = VNorm(vec);
				VECTOR move = VScale(dir, FIRE_SPEED);
				fireInfo_.transform.pos = VAdd(fireInfo_.transform.pos, move);
			}
			else
			{
				//ƒƒbƒNƒIƒ“‚ªOFF‚Ìê‡
				VECTOR move = VScale(fireInfo_.dir, FIRE_SPEED);
				fireInfo_.transform.pos = VAdd(fireInfo_.transform.pos, move);
			}
			fireInfo_.timer++;
		}
		else
		{
			DestroyFireCollider(fireInfo_);
			isAliveFire_ = false;
		}

		fireInfo_.transform.Update();
	}

	if (isAliveThunder_ || isAliveFire_) isAliveMagic_ = true;
	else isAliveMagic_ = false;
}

void Player::DragonBreathCheckCollision()
{
	// €–Só‘Ô‚È‚çˆ—‚µ‚È‚¢
	if (!isAlive_) return;
	//‚·‚Å‚Éƒ_ƒ[ƒW‚ğó‚¯‚Ä‚¢‚½‚çˆ—‚µ‚È‚¢
	if (wasHitDamage_) return;

	// ©g‚ÌƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_‚ğæ“¾
	ColliderCapsule* ownColCapsule = nullptr;
	for (const auto& ownCol : ownColliders_)
	{
		if (ownCol.second->GetTag() == ColliderBase::TAG::PLAYER)
		{
			ownColCapsule =
				dynamic_cast<ColliderCapsule*>(ownCol.second);
			//if (ownColCapsule == nullptr) return;
		}
	}

	// ƒvƒŒƒCƒ„[‚ÌŒ•ƒRƒ‰ƒCƒ_‚ÍhitColliders_‚É“o˜^‚³‚ê‚Ä‚¢‚é‚Í‚¸‚È‚Ì‚ÅA‘S‚Äƒ`ƒFƒbƒN
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() == ColliderBase::TAG::ENEMY_DRAGON_BREATH)
		{
			// ƒuƒŒƒX‚ÍƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// “G‚àƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_“¯m‚ÅÕ“Ë”»’è
			const ColliderCapsule* breathColCapsule =
				dynamic_cast<const ColliderCapsule*>(hitCol);

			if (breathColCapsule == nullptr) return;

			// Õ“Ë”»’è
			if (ownColCapsule->IsHit(breathColCapsule))
			{
				// ƒ_ƒ[ƒWˆ—
				Damage(3, transform_.GetForward());

				//ƒ_ƒ[ƒWƒTƒEƒ“ƒh
				AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_HIT);

				// ƒGƒtƒFƒNƒgÄ¶
				/*VECTOR hitPos = VAdd(
					ownColCapsule->GetCenter(),
					swordColCapsule->GetCenter());
				hitPos = VScale(hitPos, 0.5f);*/
				//HitEffect(hitPos, VNorm(VSub(hitPos, transform_.pos)), 1.0f);

				// ˆê“x‚ ‚Á‚½‚çƒtƒ‰ƒO
				wasHitDamage_ = true;

				InputManager::GetInstance()->VibrateGamepad(1, 500, 100);
			}
		}
	}

}

void Player::DragonTornadoCheckCollision()
{
	// €–Só‘Ô‚È‚çˆ—‚µ‚È‚¢
	if (!isAlive_) return;
	//‚·‚Å‚Éƒ_ƒ[ƒW‚ğó‚¯‚Ä‚¢‚½‚çˆ—‚µ‚È‚¢
	if (tornadoDamageCoolTime_ > 0) return;

	// ©g‚ÌƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_‚ğæ“¾
	ColliderCapsule* ownColCapsule = nullptr;
	for (const auto& ownCol : ownColliders_)
	{
		if (ownCol.second->GetTag() == ColliderBase::TAG::PLAYER)
		{
			ownColCapsule =
				dynamic_cast<ColliderCapsule*>(ownCol.second);
			//if (ownColCapsule == nullptr) return;
		}
	}

	// ƒvƒŒƒCƒ„[‚ÌŒ•ƒRƒ‰ƒCƒ_‚ÍhitColliders_‚É“o˜^‚³‚ê‚Ä‚¢‚é‚Í‚¸‚È‚Ì‚ÅA‘S‚Äƒ`ƒFƒbƒN
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() == ColliderBase::TAG::ENEMY_DRAGON_TORNADO)
		{
			// ƒuƒŒƒX‚ÍƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// “G‚àƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_“¯m‚ÅÕ“Ë”»’è
			const ColliderCapsule* tornadoColCapsule =
				dynamic_cast<const ColliderCapsule*>(hitCol);

			if (tornadoColCapsule == nullptr)
			{
				continue;
			}

			if (tornadoDamageCoolTime_ > 0)
			{
				return;
			}

			if (ownColCapsule->IsHit(tornadoColCapsule))
			{
				// ƒ_ƒ[ƒWˆ—
				Damage(2, transform_.GetForward());

				// 0.3•bŠÔƒ_ƒ[ƒW–³Œø
				tornadoDamageCoolTime_ = 30;

				//ƒ_ƒ[ƒWƒTƒEƒ“ƒh
				AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_HIT);

				// ƒGƒtƒFƒNƒgÄ¶
				/*VECTOR hitPos = VAdd(
					ownColCapsule->GetCenter(),
					swordColCapsule->GetCenter());
				hitPos = VScale(hitPos, 0.5f);*/
				//HitEffect(hitPos, VNorm(VSub(hitPos, transform_.pos)), 1.0f);


				InputManager::GetInstance()->VibrateGamepad(1, 500, 100);
			}
		}
	}
}
