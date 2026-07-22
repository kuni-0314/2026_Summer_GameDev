#include <DxLib.h>
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Collider/Sphere/ColliderSphere.h"
#include "../../../Collider/Line/ColliderLine.h"
#include "../../../Collider/Model/ColliderModel.h"
#include "../../../Common/AnimationController.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Object/Item/HP/HpItem.h"
#include "../../../../Object/Item/ItemManger.h"
#include "../../../../Sound/AudioManager.h"
#include "./../../../../Application.h"
#include "../../Player/Player.h"
#include "EnemyLarge.h"

EnemyLarge::EnemyLarge(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f),
	hoverTime_(0.0f),
	ringTransform_(nullptr)
{
}

EnemyLarge::~EnemyLarge()
{
}

void EnemyLarge::Draw(void)
{
	// 親クラスの描画処理
	EnemyBase::Draw();

	if (isDrop_)
	{
		MV1SetScale(ringModelHandle_, ringTransform_->scl);
		MV1SetPosition(ringModelHandle_, ringTransform_->pos);
		MV1DrawModel(ringModelHandle_);
	}

#ifdef _DEBUG
	DrawSphere3D(attackWorldPos_, COL_SPHERE_RADIUS, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);

	STATE next = state_;
	const char* name = "";
	if (next == STATE::THINK) name = "THINK";
	else if (next == STATE::IDLE) name = "IDlE";
	else if (next == STATE::ATTACK_PUNCH) name = "ATTACK_PUNCH";
	else if (next == STATE::ATTACK_RUN) name = "ATTACK_RUN";
	else if (next == STATE::CHARGE) name = "CHARGE";
	else if (next == STATE::ATTACK_DROP) name = "ATTACK_DROP";

	DrawFormatString(0, 200, GetColor(255, 255, 255), "STATE: %s", name);
	DrawFormatString(0, 250, GetColor(255, 255, 255), "距離: %.2f", distance_);

	DrawFormatString(500, 250, GetColor(255, 255, 255), "距離: %d", isAttack_);

	// Aの球（赤）
	DrawSphere3D(transform_.pos, pushOutRadius_, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	// Bの球（青）
	DrawSphere3D(playerPos_, playerRad_, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
	// 中心を結ぶ線（緑）
	DrawLine3D(transform_.pos, playerPos_, GetColor(0, 255, 0));

	//正面にPlayerがいるのか
	if (InFront())
	{
		if (player_->IsAttacking())
		{
			//ここで正面のエフェクト
			DrawString(500, 140, "正面の攻撃はねぇよ", 0xffffff);
		}
	}


#endif
}

void EnemyLarge::Release(void)
{
}

void EnemyLarge::InitLoad()
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_LARGE));

	//SE読み込み
	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);
	
}

void EnemyLarge::InitTransform()
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();

	ringTransform_ = std::make_unique<Transform>();
	ringTransform_->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_LARGE_RING));
	ringModelHandle_ = ringTransform_->modelId;
	ringTransform_->scl = { RING_SCALE ,RING_SCALE ,RING_SCALE };
	ringTransform_ ->quaRot = Quaternion::Identity();
	ringTransform_ ->quaRotLocal = Quaternion::Euler(ROT);
	ringTransform_->Update();
}

void EnemyLarge::InitCollider()
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ（前半分）
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY, &transform_,
		COLBODY_CAPSULE_TOP_LOCAL_POS, COLBODY_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);


	// DxLib側の衝突情報セットアップ
	MV1SetupCollInfo(transform_.modelId);
	// モデルのコライダ
	ColliderModel* colModel =
		new ColliderModel(ColliderBase::TAG::STAGE, &transform_);
	//対象とするフレーム
	for (const std::string& name : TARGET_FRAME_NAMES)
	{
		colModel->AddTargetFrameIds(name);
	}

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);
}

void EnemyLarge::InitAnimation()
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE)
		, 20.0f, Application::PATH_MODEL + "Enemy/Large/Idle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::MOVE)
		, 20.0f, Application::PATH_MODEL + "Enemy/Large/Walk.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_PUNCH)
		, 20.0f, Application::PATH_MODEL + "Enemy/Large/Punch.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_DROP)
		, 20.0f, Application::PATH_MODEL + "Enemy/Large/Drop.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::HIT)
		, 20.0f, Application::PATH_MODEL + "Enemy/Large/Hit.mv1");


	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	
}

void EnemyLarge::InitPost()
{
	weight_ = WEIGHT::HEAVY;

	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyLarge::ChangeStateIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::THINK),
		std::bind(&EnemyLarge::ChangeStateThink, this));

	stateChanges_.emplace(static_cast<int>(STATE::MOVE),
		std::bind(&EnemyLarge::ChangeStateMove, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_RUN),
		std::bind(&EnemyLarge::ChangeStateAttackRun, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_PUNCH),
		std::bind(&EnemyLarge::ChangeStateAttackPunch, this));

	stateChanges_.emplace(static_cast<int>(STATE::CHARGE),
		std::bind(&EnemyLarge::ChangeStateCharge, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_DROP),
		std::bind(&EnemyLarge::ChangeStateAttackDrop, this));

	stateChanges_.emplace(static_cast<int>(STATE::HIT),
		std::bind(&EnemyLarge::ChangeStateHit, this));

	stateChanges_.emplace(static_cast<int>(STATE::DIE),
		std::bind(&EnemyLarge::ChangeStateDie, this));

	// 初期状態設定
	ChangeState(STATE::IDLE);


	power_ = 2;
	pushOutRadius_  = 150.0f;

}

void EnemyLarge::UpdateProcess()
{
	//プレイヤー情報関連
	playerPos_ = player_->GetPos();
	playerRad_ = player_->GetCollRadius();

	// プレイヤー方向
	toPlayer_ = VSub(playerPos_, transform_.pos);
	//プレイヤーとの距離測定
	distance_ = VSize(toPlayer_);
	
	STATE state = state_;

	//プレイヤー視認状態
	if(look_)
	{
		LookPlayer();
	}

	//ダメージヒット処理
	preHp_ = hp_;//被ダメージ前HP保存

	CheckPlayerSwordCollision();
	CheckPlayerMagicCollision();

	//正面にPlayerがいるのか
	/*bool isATField_;
	isATField_ = false;*/

	if (!isATField_ && InFront() && player_->IsAttacking())
	{
		isATField_ = true;
		ATfield(transform_.pos);
	}

	if (!player_->IsAttacking())
	{
		isATField_ = false;
	}

	if (hp_ < preHp_)
	{
		//ChangeState(STATE::HIT);
	}

	if (hp_ <= 0)
	{
		ChangeState(STATE::DIE);
	}

	//押し出し
	PushOutSphere(transform_.pos,pushOutRadius_,
		player_->GetPos(),player_->GetCollRadius(),true); 
	


	//衝撃波
	if (isDrop_)
	{
		ringTransform_->scl = VAdd(ringTransform_->scl, RING_ADD_SCL);

		if (ringTransform_->scl.x >= RING_MAX_SCALE)
		{
			isDrop_ = false;
			ringTransform_->scl = { RING_SCALE, RING_SCALE, RING_SCALE };
		
		}

		ringTransform_->Update();
	}
	

	//パンチ攻撃判定座標更新
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(transform_.modelId, 22);
	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);
	// 位置を適用
	attackWorldPos_ = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot = Quaternion::GetRotation(mat);
}

void EnemyLarge::UpdateProcessPost()
{
	stateUpdate_();

}

void EnemyLarge::ATfield(const VECTOR& pos)
{
	VECTOR front = VNorm(moveDir_);

	VECTOR effectPos = VAdd(pos, VScale(front, 120.0f));
	effectPos.y += 100.0f;

	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Hit/Hit.efkefc",
		effectPos);

	effect->SetScale(80.0f);
	effect->Play(effectPos, Quaternion());

	EffectManager::GetInstance().RegisterEffect(effect);
}

void EnemyLarge::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}


void EnemyLarge::ChangeStateIdle()
{
	stateUpdate_ =  std::bind(&EnemyLarge::UpdateIdle, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyLarge::ChangeStateDie()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateDie, this);
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::HIT), false);
}

void EnemyLarge::ChangeStateThink()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateThink, this);

	//IDLEと同じ

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyLarge::ChangeStateCharge()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateCharge, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyLarge::ChangeStateAttackPunch()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateAttackPunch, this);

	look_ = true;

	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK_PUNCH), false);
}

void EnemyLarge::ChangeStateAttackRun()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateAttackRun, this);

	look_ = true;

	
	if (hp_ > hp_ / 2)
	{
		moveSpeed_ = 8.0f;
	}
	else
	{
		moveSpeed_ = 10.0f;
	}

	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::MOVE), true);
}

void EnemyLarge::ChangeStateAttackDrop()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateAttackDrop, this);

	jumpApplied_ = false;
	look_ = false;

	movePow_ = AsoUtility::VECTOR_ZERO;


	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK_DROP), false);
}

void EnemyLarge::ChangeStateHit()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateHit, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// まだプレイヤーの攻撃が実装されいないのでその場で
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::HIT), false);
}

void EnemyLarge::ChangeStateMove()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateMove, this);

	look_ = true;

	// 移動スピード
	moveSpeed_ = 3.0f;

	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::MOVE), true);
}



void EnemyLarge::UpdateIdle()
{
	countUp++;

	if (countMax < countUp)
	{
		countUp = 0;
		ChangeState(STATE::THINK);
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyLarge::UpdateDie()
{

	if (animationController_->IsEnd())
	{
		MV1DeleteModel(transform_.modelId);
	}
}

void EnemyLarge::UpdateThink()
{

	int rand = GetRand(100);

	//距離が近かったらパンチ
	if (distance_ < SWICH_DISTANCE)
	{
		if (rand <= 20)
		{
			ChangeState(STATE::IDLE);
		}
		else
		ChangeState(STATE::ATTACK_PUNCH);
	}
	else if (rand >= 30)
	{
		ChangeState(STATE::CHARGE);
	}
	else
	{
		ChangeState(STATE::ATTACK_DROP);
	}

	
}

void EnemyLarge::UpdateCharge()
{
	int rand = GetRand(100);

	countUp++;

	if (countMax /2 < countUp)
	{
		countUp = 0;
		if (rand < 70)
		{
			ChangeState(STATE::ATTACK_RUN);
		}
		else
		{
			ChangeState(STATE::IDLE);
		}
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

bool EnemyLarge::InFront()
{
	VECTOR toPlayer = VSub(player_->GetPos(), transform_.pos);
	float distance = VSize(toPlayer);

	if (distance > 300.0f)
		return false;

	toPlayer.y = 0.0f;
	toPlayer = VNorm(toPlayer);

	return VDot(moveDir_, toPlayer) > 0.5f;
}

void EnemyLarge::UpdateAttackPunch()
{

	//アニメーションコントローラーの取得
	if (animationController_ == nullptr) return;
	const auto& anim = animationController_->GetPlayAnim();
	if (anim.totalTime <= 0.0f) return;

	// Transform の更新
	transform_.Update();

	//進行方向を決める
	if (look_)
	{
		look_ = false;
	}

	if (!isAttack_)
	{
		float attackTriggerTime = anim.totalTime * ATTACK_PUNCH_TINE;
		if (anim.step >= attackTriggerTime)
		{
			isAttack_ = true;
		}
		// 攻撃判定
		if (AsoUtility::IsHitSpheres(attackWorldPos_, COL_SPHERE_RADIUS, playerPos_, playerRad_))
		{
			player_->Damage(power_, transform_.GetForward());
			isAttack_ = true;
		}
	}

	if(animationController_->IsEnd())
	{

		ChangeState(STATE::THINK);
		isAttack_ = false;
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyLarge::UpdateAttackRun()
{

	//進行方向を決める
	if(look_)
	{
		//突進開始位置
		startPos = transform_.pos;
		look_ = false;
	}

	//進行方向を決めた後移動
	
	float dist =VSize(VSub(transform_.pos, startPos)); //移動距離

	if (distance_ <= SWICH_DISTANCE)
	{
		ChangeState(STATE::THINK);
	}

	if (dist > ATTACK_RUN_END_POINT)
	{
		ChangeState(STATE::CHARGE);
	}

	movePow_ = VScale(moveDir_, moveSpeed_);
	
}

void EnemyLarge::UpdateAttackDrop()
{
	//アニメーションコントローラーの取得
	if (animationController_ == nullptr) return;
	const auto& anim = animationController_->GetPlayAnim();
	if (anim.totalTime <= 0.0f) return;

	//ジャンプ処理
	float jumpTriggerTime = anim.totalTime * ATTACK_DROP_JUMP_TIME_RATIO;
	if (!jumpApplied_ && anim.step >= jumpTriggerTime)
	{
		jumpApplied_ = true;
		jumpPow_ = VGet(0.0f, 25.0f, 0.0f);
		isJump_ = true;
		isAir_ = true;
	}

	//衝撃波発生
	float attackTriggerTime = anim.totalTime * ATTACK_DROP_JUMP_TIME_WAVE;


	if (!attackTriggerRing_ && anim.step >= attackTriggerTime)
	{
		// 発生の瞬間に一度だけ座標をセット
		ringTransform_->pos = transform_.pos;
		ringTransform_->pos.y += 20;
		ringTransform_->scl = { RING_SCALE,RING_SCALE ,RING_SCALE };
		isDrop_ = true;
		attackTriggerRing_ = true;

		isDrop_ = true;
		attackTriggerRing_ = true;

		auto effect = std::make_shared<EffekseerEffect>(
			L"Data/Effect/Circle/ShockWave.efkefc",
			ringTransform_->pos
		);

		//Y軸を３０上げる
		ringTransform_->pos.y += 30.0f;

		effect->SetScale(30.0f);
		effect->Play(ringTransform_->pos, Quaternion());

		EffectManager::GetInstance().RegisterEffect(effect);
	}

	if (animationController_->IsEnd())
	{
		isAir_ = false;
		isJump_ = false;
		attackTriggerRing_ = false;
		jumpApplied_ = false;
		ChangeState(STATE::IDLE);
	}

	if (!isDrop_)
	{
		wasHitRing_ = false;
		return;
	}
	VECTOR playerPos = player_->GetPos();
	float nowR1 = ringTransform_.get()->scl.x * 97.5f;
	float nowR2 = ringTransform_.get()->scl.x * 92.5f;
	VECTOR vec = VSub(playerPos, ringTransform_.get()->pos);
	float dist = VSize(vec);
	if (wasHitRing_ == false &&
		dist <= nowR1 && dist >= nowR2)
	{
		if (player_->IsJump()) return;
		player_->Damage(power_, transform_.GetForward());
		wasHitRing_ = true;
		wasHitMagic_ = true;
	}
}

void EnemyLarge::UpdateHit()
{
	if (animationController_->IsEnd())
	{

		ChangeState(STATE::IDLE);
	}
}

void EnemyLarge::UpdateMove()
{
	//攻撃範囲に入るまで移動
	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::ATTACK_RUN);
	}

	// 移動する ← 追加
	movePow_ = VScale(moveDir_, moveSpeed_);
}