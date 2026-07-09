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
	int a = StartJoypadVibration(padNum_ + 1, 1000, 500, -1);
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
	ColliderBase:: TAG::PLAYER, &transform_,
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
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N2)
		, 70.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N3)
		, 60.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N4)
		, 50.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATK_N5)
		, 40.0f, Application::PATH_MODEL + "Player/Attack1.mv1");
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

		// 終了
		if (m_powerUpTimer <= 0)
		{
			m_isPowerUp = false;
		}
	}
	else
	{
		// 通常時だけ操作可能
		if (currentState_ != nullptr)
		{
			currentState_->Update(this);
		}
	}

	// 状態別更新処理
	//if (currentState_ != nullptr)
	//{
	//	currentState_->Update(this);
	//}

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_R))
	{
		// リスポーン
		transform_.pos = POS_PLAYER;
	}

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_P))
	{
		ActivatePowerUp();
	}

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_B))
	{
		PlayBlinkEffect();
	}

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_T))
	{
		// 雷の位置をランダムに設定
		for (int i = 0; i < THUNDER_COUNT; i++)
		{
			float randomAngle = (GetRand(180) - 90) * DX_PI_F / 180.0f;

			const float DIST_MAX = 500.0f;

			float randomRadius =
				sqrtf(GetRand(100) / 100.0f) * DIST_MAX;

			float randomX = sinf(randomAngle) * randomRadius;
			float randomZ = cosf(randomAngle) * randomRadius;

			VECTOR randomPos =
			{
				randomX,
				0.0f,
				randomZ
			};

			randomPos = transform_.quaRot.PosAxis(randomPos);

			thunderInfos_[i].transform.pos = VAdd(transform_.pos, randomPos);
			thunderInfos_[i].timer = 0;
			thunderInfos_[i].isActive = false;
			thunderInfos_[i].isDestroyed = false;
		}
		magicTimer_ = 0;
	}

	// 魔法開始からカウント
	magicTimer_++;

	// 魔法がまだ生存しているかどうか
	isAliveMagic_ = false;

	for (int i = 0; i < THUNDER_COUNT; i++)
	{
		// 雷が発生したか
		if (thunderInfos_[i].isActive)
		{
			// 雷は削除済みか
			if (thunderInfos_[i].isDestroyed)
			{
				continue;
			}
			else
			{
				// 雷は生存時間外か
				if (thunderInfos_[i].timer >= THUNDER_LIFETIME)
				{
					// 雷のコライダを消す
					DestroyThunderCollider(thunderInfos_[i]);

					// 破壊フラグを立てる
					thunderInfos_[i].isDestroyed = true;
				}
				else
				{
					thunderInfos_[i].timer++;
					isAliveMagic_ = true;
				}
			}
		}
		else
		{
			// 一定時間後雷を発生させる
			if (magicTimer_ >= i * THUNDER_INTERVAL)
			{
				// 雷を有効にする
				thunderInfos_[i].isActive = true;

				// 雷のコライダを作る
				CreateThunderCollider(thunderInfos_[i]);

				// 音を鳴らす
				AudioManager::GetInstance()->PlaySE(SoundID::SE_THUNDER);
			}
		}
	}

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

	//DrawFormatString(x, y, color, "Level  : %d", status_.level);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 2) ? highlightColor : color, "MovePow : %f", movePow_.x);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 3) ? highlightColor : color, "MP     : %d", status_.mp);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 4) ? highlightColor : color, "PhysAtk: %d", status_.physAtk);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 5) ? highlightColor : color, "PhysDef: %d", status_.physDef);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 6) ? highlightColor : color, "MagicAtk: %d", status_.magicAtk);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 7) ? highlightColor : color, "MagicDef: %d", status_.magicDef);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 8) ? highlightColor : color, "Wisdom : %d", status_.wisdom);
	//y += lineHeight;

	//DrawFormatString(x, y, (currentGrantStatusIndex_ == 9) ? highlightColor : color, "Luck   : %d", status_.luck);
	//y += lineHeight;

	//DrawFormatString(x, y, color, "PendingPoints   : %d", pendingPoints_);

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
	
	for (int i = 0; i < THUNDER_COUNT; i++)
	{
		if (!thunderInfos_[i].isActive) continue;
		VECTOR spherePos = thunderInfos_[i].transform.pos;
		DrawSphere3D(spherePos, 100.0f, 16, 0xff00ff, 0xff00ff, false);
	}

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
	states_[STATE::JET] = new PlayerJetState();
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
