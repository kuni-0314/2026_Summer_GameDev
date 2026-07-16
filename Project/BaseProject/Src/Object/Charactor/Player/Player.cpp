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
#include "../../../Effect/LoadEffekseer/EffekseerEffect.h" // パスは環境に合わせて調整してください
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


Player::Player(int padNum)
	:
	padNum_(padNum),
	CharactorBase()
{
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
	// 移動前座標を更新
	prevPos_ = transform_.pos;

	// 攻撃のクールタイムを減算
	if (attackCoolTime_ > 0)
	{
		attackCoolTime_--;
	}

	// コンボタイマーを減算
	if (comboTimer_ > 0)
	{
		comboTimer_--;
	}

	// 各キャラクターごとの更新処理
	UpdateProcess();
	if (!m_isPowerUp)
	{
		// 移動方向に応じて徐々に回転
		DelayRotate();
		// 重力による移動量
		CalcGravityPow();
		// 衝突判定前準備
		CollisionReserve();
		// 衝突判定
		Collision();
		// アニメーション再生
		animationController_->Update();
	}
	// モデル制御更新
	transform_.Update();
	// 各キャラクターごとの更新後処理
	UpdateProcessPost();


	// hpのindexは4、luckのindexは9
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

	//// 矢印キー右でステータスにポイント割り振り
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_RIGHT))
	//{
	//	if (pendingPoints_ > 0)
	//	{
	//		GrantStatus(currentGrantStatusIndex_);
	//		pendingPoints_--;
	//	}
	//}

	//// 矢印キー左でステータスからポイントを戻す
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_LEFT))
	//{
	//	RevokeStatus(currentGrantStatusIndex_);
	//}



}

void Player::Damege(int damege)
{
	hp_ -= damege;
	//int a = StartJoypadVibration(padNum_ + 1, 1000, 500, -1);
	//VibrateGamepad(int gamepadIndex, int power, int time)
	InputManager::GetInstance()->VibrateGamepad(padNum_ + 1, 1000, 500);
	if (hp_ <= 0)
	{
		hp_ = 0;
	}
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

	//基底クラスのリソースロード
	CharactorBase::InitLoad();
}

void Player::InitTransform()
{

	//大きさ、座標等の初期化
	transform_.scl = { SCL_PlAYER ,SCL_PlAYER ,SCL_PlAYER };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT_LOCAL_PLAYER);
	transform_.pos = POS_PLAYER;


	transform_.Update();

}

void Player::InitCollider()
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}

void Player::InitAnimation()
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	// 待機状態アニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE)
		, 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");

	// 走るアニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN)
		, 20.0f, Application::PATH_MODEL + "Player/Walk.mv1");

	// ダッシュアニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::FAST_RUN)
		, 40.0f, Application::PATH_MODEL + "Player/Run.mv1");
	//ジャンプアニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP)
		, 60.0f, Application::PATH_MODEL + "Player/Jump.mv1");

	// 攻撃アニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N1)
		, 80.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N2)
		, 60.0f, Application::PATH_MODEL + "Player/Attack2.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N3)
		, 90.0f, Application::PATH_MODEL + "Player/Attack3.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N4)
		, 50.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N5)
		, 80.0f, Application::PATH_MODEL + "Player/Attack5.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_H)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_D)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_A1)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_A2)
		, 70.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_A3)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_A4)
		, 50.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_A5)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_F)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");//tmp
	animationController_->Add(static_cast<int>(ANIM_TYPE::MAGIC)
		, 40.0f, Application::PATH_MODEL + "Player/Spell Cast.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ROLLING)
		, 65.0f, Application::PATH_MODEL + "Player/Rolling.mv1");

	//初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
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

	// 武器初期化
	sword_ = new KeyBlade3(KEY_BLADE_3_LOCAL_POS_START, KEY_BLADE_3_LOCAL_POS_END, KEY_BLADE_3_RADIUS, transform_);
	sword_->Init();


}

void Player::UpdateProcess()
{
	// パワーアップ中
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

	// ショートカットキー判定
	if (ins->IsGamepadNew(InputManager::PadInput::LB, padNum_))
	{
		isShortCut_ = true;
	}
	else
	{
		isShortCut_ = false;
	}

	// 魔法開始
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

	// 魔法処理
	UpdateMagic();

	CheckPlayerRingCollision();

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
	//基底クラスの描画処理
	ActorBase::Draw();
	// 丸影の描画
	DrawShadow();

	// プレイヤーの武器描画
	if (sword_ != nullptr)
	{
		sword_->Draw();
	}
#ifdef _DEBUG
	// ステータス描画
	int x = 20;
	int y = 20;
	int lineHeight = 25;

	// 背景描画(半透明の黒)
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	//DrawBox(x - 10, y - 10, x + 250, y + lineHeight * 11 + 10, 0x000000, true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// タイトル
	//DrawFormatString(x, y, 0xFFFFFF, "=== Player Status ===");
	y += lineHeight;

	currentState_->Draw(this);

	// ステータス情報を描画（選択中の項目を黄色でハイライト）
	unsigned int color = 0xFFFFFF;
	unsigned int highlightColor = 0xFFFF00;


	//y += lineHeight;
	DrawFormatString(x, y, 0xffffff, "jumpPow   : %f.", jumpPow_.y);

	VECTOR lineStart = transform_.pos;
	VECTOR lineEnd = { transform_.pos.x, transform_.pos.y + jumpPow_.y * 10.0f, transform_.pos.z };
	DrawLine3D(lineStart, lineEnd, 0xFFFFFF);
	DrawSphere3D(lineEnd, 5.0f, 16, 0xFF00FF, 0xFF00FF, true);

	// プレイヤーの周りを回転するオブジェクトの描画
	static float angle = 0.0f;
	angle += 0.01f;

	// プレイヤーの座標を中心に半径100.0fで回転
	float radius = 100.0f;
	VECTOR rot = transform_.pos;
	rot.x += sinf(angle) * radius;
	rot.z += cosf(angle) * radius;

	// プレイヤーの座標系を描画
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

	//for (int i = 0; i < THUNDER_COUNT; i++)
	//{
	//	if (!thunderInfos_[i].isActive) continue;
	//	VECTOR spherePos = thunderInfos_[i].transform.pos;
	//	DrawSphere3D(spherePos, 100.0f, 16, 0xff00ff, 0xff00ff, false);
	//}
	//if (isAliveFire_)
	//{
	//	DrawSphere3D(fireInfo_.transform.pos, FIRE_RADIUS, 16, 0xff0000, 0xff0000, false);
	//}


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

	// エフェクト再生時間
	m_powerUpTimer = 60; // 約1秒(60FPSの場合)

	VECTOR effectPos = transform_.pos;
	// エフェクトの再生位置をプレイヤーの座標に設定
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/PowerUp/PowerUp2.efkefc",
		effectPos
	);
	// エフェクトを再生
	effect->Play(
		effectPos,
		transform_.quaRot
	);

	EffectManager::GetInstance().RegisterEffect(effect);
}

void Player::PlayBlinkEffect()
{
	// エフェクト再生時間
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Bring/Blink.efkefc",
		transform_.pos
	);
	// エフェクトの再生位置をプレイヤーの背後に設定
	VECTOR back = transform_.quaRot.GetBack();
	// 50.0fの距離を背後に設定
	VECTOR backPos = VScale(
		back,
		50.0f
	);
	// プレイヤーの座標に背後の位置を加算
	backPos = VAdd(
		transform_.pos,
		backPos
	);
	// エフェクトを再生
	effect->Play(
		backPos,
		transform_.quaRot
	);
	// エフェクトをエフェクトマネージャーに登録
	EffectManager::GetInstance().RegisterEffect(effect);
}

void Player::CheckPlayerRingCollision()
{
	//// プレイヤーのカプセルコライダを取得
	//ColliderCapsule* playerCollider =
	//	dynamic_cast<ColliderCapsule*>(ownColliders_[static_cast<int>(COLLIDER_TYPE::CAPSULE)]);

	//if (playerCollider == nullptr) return;

	////衝突情報更新
	//MV1RefreshCollInfo(playerCollider->GetFollow()->modelId);

	//// 登録されている衝突物を全てチェック
	//bool isHit = false;
	//for (const auto& hitCol : hitColliders_)
	//{
	//	if (hitCol->GetTag() != ColliderBase::TAG::ENEMY_RING) continue;

	//	// 派生クラスへキャスト
	//	const ColliderModel* ringCollider =
	//		dynamic_cast<const ColliderModel*>(hitCol);

	//	if (ringCollider == nullptr) continue;
	//	//モデルとカプセルの諸突判定
	//	auto hits = MV1CollCheck_Capsule(
	//		ringCollider->GetFollow()->modelId, -1,
	//		playerCollider->GetPosTop(), playerCollider->GetPosDown(), playerCollider->GetRadius());

	//	// 衝突した複数のポリゴンと衝突回避するまで、位置を移動させる
	//	for (int i = 0; i < hits.HitNum; i++)
	//	{
	//		auto hitPoly = hits.Dim[i];
	//		isHit = true;
	//		break;
	//	}
	//	// 検出した地面ポリゴン情報の後始末
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
	// 各ステータスが初期値以下にならないようにチェック
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
	states_[STATE::MAGIC] = new PlayerMagicState();  // 追加
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
		fireInfo_ = FireInfo();
		fireInfo_.timer = 0;
		fireInfo_.transform.pos = transform_.pos;
		fireInfo_.dir = transform_.quaRot.GetForward();
		CreateFireCollider(fireInfo_);
		isAliveFire_ = true;
		fireCoolTime_ = FIRE_COOL_TIME;
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
			// 角度範囲を狭く: -60度から+60度(前方120度の範囲)
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

		// 全部終了
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
		GameScene::CAM_MODE mode = gameScene_->GetCamMode();
		if (fireInfo_.timer < FIRE_LIFETIME)
		{
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
