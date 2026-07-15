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
#include "../../Player/Player.h"
#include "../Shot/ShotBase.h"
#include "../Shot/ShotStraight.h"
#include "EnemyRase.h"
#include <EffekseerForDXLib.h>


EnemyRase::EnemyRase(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f),
	hoverTime_(0.0f),
	baseHeight_(0.0f)
{
}

EnemyRase::~EnemyRase()
{
}

void EnemyRase::Draw()
{
	// 基底クラスの描画処理
	CharactorBase::Draw();
	DrawShot();

#ifdef _DEBUG

	STATE next = state_;

	const char* name = "";

	if (next == STATE::THINK) name = "THINK";
	else if (next == STATE::ATTACK) name = "ATTACK";
	else if (next == STATE::IDLE) name = "IDLE";
	else if (next == STATE::MOVE) name = "MOVE";
	else if (next == STATE::CHARGE) name = "CHARGE";

	DrawFormatString(0, 400, GetColor(255, 255, 255), "STATE: %s", name);

	DrawFormatString(500, 400, GetColor(255, 255, 255), "RASE_HP: %d", hp_);

#endif // _DEBUG
}

void EnemyRase::Release(void)
{
	
}

void EnemyRase::InitLoad()
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RASE));
	//弾のロード
	shotmodel_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RASE_BALL);

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);

}

void EnemyRase::InitTransform()
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	//transform_.pos = { 0.0f, 100.0f, 1500.0f };

	transform_.Update();

}

void EnemyRase::InitCollider()
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

}

void EnemyRase::InitAnimation()
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	int type = -1;

	//待機
	type = static_cast<int>(ANIM_TYPE::IDLE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_FRY);
	//攻撃待機
	type = static_cast<int>(ANIM_TYPE::CHARGE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_CHARGE);

	type = static_cast<int>(ANIM_TYPE::HIT);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_HIT);

	type = static_cast<int>(ANIM_TYPE::DIE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_DEAD);

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyRase::InitPost()
{

	//基準の高さ保存
	baseHeight_ = transform_.pos.y;

	
	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyRase::ChangeStateIdle, this));
	stateChanges_.emplace(static_cast<int>(STATE::THINK),
		std::bind(&EnemyRase::ChangeStateThink, this));
	stateChanges_.emplace(static_cast<int>(STATE::MOVE),
		std::bind(&EnemyRase::ChangeStateMove, this));
	stateChanges_.emplace(static_cast<int>(STATE::ATTACK),
		std::bind(&EnemyRase::ChangeStateAttack, this));
	stateChanges_.emplace(static_cast<int>(STATE::WAIT),
		std::bind(&EnemyRase::ChangeStateWait, this));
	stateChanges_.emplace(static_cast<int>(STATE::CHARGE),
		std::bind(&EnemyRase::ChangeStateCharge, this));
	stateChanges_.emplace(static_cast<int>(STATE::HIT),
		std::bind(&EnemyRase::ChangeStateHit, this));
	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&EnemyRase::ChangeStateEnd, this));
	stateChanges_.emplace(static_cast<int>(STATE::DIE),
		std::bind(&EnemyRase::ChangeStateDie, this));

	shotCharge_ =  SHOT_CHARGE_COUNT;
	
	pushOutRadius_ = 40.0f;

	// 初期状態設定
	ChangeState(STATE::THINK);
	
}

void EnemyRase::UpdateProcess()
{
	playerPos_ = player_->GetPos();
	playerRad_ = player_->GetCollRadius();

	// プレイヤー方向
	toPlayer_ = VSub(playerPos_, transform_.pos);

	//プレイヤーとの距離測定
	distance_ = VSize(toPlayer_);

	LookPlayer();

	//上下の揺れ
	hoverTime_ += scnMng_.GetDeltaTime();
	transform_.pos.y =
		baseHeight_ + sinf(hoverTime_ * HOVER_SPEED) * HOVER_HEIGHT;
	//ちょっとした横揺れ
	transform_.pos.x += sinf(hoverTime_ * 0.7f) * 0.2f;

	for (auto& shot : shots_)
	{
		shot.shotTransform_.Update();
	}


	PushOutSphere(transform_.pos, pushOutRadius_,
		player_->GetPos(), player_->GetCollRadius(), true);

	auto const ins = InputManager::GetInstance();

	//ダメージヒット処理
	preHp_ = hp_;//被ダメージ前HP保存

	CheckPlayerSwordCollision();

	if (hp_ < preHp_)
	{
		if (hp_ <= 0)
		{
			ChangeState(STATE::DIE);
			isAlive_ = false;
		}
		else
		{
			ChangeState(STATE::HIT);
		}
	}
}

void EnemyRase::UpdateProcessPost()
{
	stateUpdate_();
	//弾の更新
	UpdateShot();

}

void EnemyRase::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}

void EnemyRase::ChangeStateIdle()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateIdle, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}

void EnemyRase::ChangeStateAttack()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateAttack, this);

	shotFired_ = false;

	AudioManager::GetInstance()->SetSeVolume(80);
	AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_RASE_ATTACK);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyRase::ChangeStateMove(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateMove, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動スピード
	moveSpeed_ = 3.0f;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyRase::ChangeStateWait(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateWait, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

}

void EnemyRase::ChangeStateHit()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateHit, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// まだプレイヤーの攻撃が実装されいないのでその場で
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::HIT), false);
}

void EnemyRase::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateEnd, this);

}

void EnemyRase::ChangeStateDie()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateDie, this);
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DIE), false);

}

void EnemyRase::ChangeStateCharge(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateCharge, this);

	shotFired_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::CHARGE), true);
}

void EnemyRase::ChangeStateThink()
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateThink, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

}

void EnemyRase::UpdateIdle()
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::UpdateAttack()
{
	if (!shotFired_)
	{
		AttackShot();
		shotFired_ = true;
	}
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::UpdateMove(void)
{
	//攻撃範囲に入るまで移動
	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::CHARGE);
	}

	// 移動する ← 追加
	movePow_ = VScale(moveDir_, moveSpeed_);
}

void EnemyRase::UpdateWait(void)
{
}

void EnemyRase::UpdateThink(void)
{
	
	//攻撃するか否や
	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::CHARGE);
	}
	else
	{
		ChangeState(STATE::MOVE);
	}
}

void EnemyRase::UpdateHit()
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::THINK);
		return;
	}
}

void EnemyRase::UpdateDie()
{
	if (animationController_->IsEnd())
	{
		MV1DeleteModel(transform_.modelId);
		ChangeState(STATE::END);
	}
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::UpdateEnd()
{
}

void EnemyRase::UpdateCharge(void)
{
	shotCharge_--;

	if (shotCharge_ <= 0)
	{
		ChangeState(STATE::ATTACK);
		shotCharge_ = SHOT_CHARGE_COUNT;
	}
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::AttackShot()
{
	SHOT shot;

	shot.isShotAlive_ = true;

	shot.shotTransform_.scl = { SHOT_SCALE, SHOT_SCALE, SHOT_SCALE };
	shot.shotTransform_.quaRot = Quaternion::Identity();
	shot.shotTransform_.quaRotLocal = Quaternion::Euler(ROT);

	shot.shotTransform_.modelId = shotmodel_;
	shot.shotTransform_.SetModel(MV1DuplicateModel(shotmodel_));

	shot.shotTransform_.pos = transform_.pos;
	shot.dir_ = VNorm(toPlayer_);

	shot.shotTransform_.Update();

	shot.effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Fire/Fire.efkefc",
		shot.shotTransform_.pos
	);

	shot.effect->Play(
		shot.shotTransform_.pos,
		Quaternion::LookRotation(shot.dir_)
	);

	EffectManager::GetInstance().RegisterEffect(shot.effect);

	shots_.emplace_back(std::move(shot));
}

void EnemyRase::UpdateShot(void)
{
	for (auto& shot : shots_)
	{
		if (!shot.isShotAlive_) continue;

		//プレイヤーとの衝突判定
		if (AsoUtility::IsHitSpheres(shot.shotTransform_.pos, COL_SPHERE_RADIUS, playerPos_, playerRad_))
		{
			player_->Damege(1);
			shot.life = 0;
		}

		shot.speed += 0.05;

		// ホーミング処理
		if (shot.life > 60)
		{
			// プレイヤーの方向を計算
			VECTOR targetDir =
				VNorm(VSub(player_->GetPos(),
					shot.shotTransform_.pos));

			// ホーミングの方向を計算
			shot.dir_ =
				VNorm(VAdd(VScale(shot.dir_, 1.0f - shot.homingPower),
					VScale(targetDir, shot.homingPower)));

		}

		shot.shotTransform_.pos =
			VAdd(shot.shotTransform_.pos,
				VScale(shot.dir_, shot.speed));

		shot.shotTransform_.Update();

		//エフェクトをプレイヤーに追従	
		if (shot.effect)
		{
			shot.effect->SetPosition({ 0.0f,1000.0f,0.0f });
			shot.effect->SetRotation(Quaternion::LookRotation(shot.dir_));

		}


		if (shot.shotTransform_.pos.y <= 0)
		{
			shot.shotTransform_.pos.y = 0;
		}

		shot.life--;

		if (shot.life <= 0)
		{
			if (shot.effect)
			{
				shot.effect->Stop();
			}

			// 弾が消滅したときのエフェクトを生成
			auto burst = std::make_shared<EffekseerEffect>(
				L"Data/Effect/Fire/Burst.efkefc",
				shot.shotTransform_.pos
			);

			burst->Play(
				shot.shotTransform_.pos,
				Quaternion()
			);

			burst->SetLifeTime(120);

			EffectManager::GetInstance().RegisterEffect(burst);

			shot.isShotAlive_ = false;
			shot.speed = 3.0f;
			ChangeState(STATE::THINK);
		}
	}
}

void EnemyRase::DrawShot(void)
{
	for (auto& shot : shots_)
	{
		if (!shot.isShotAlive_)continue;

		shot.shotTransform_.Update();

		//描画
		MV1SetPosition(shotmodel_, shot.shotTransform_.pos);
		MV1DrawModel(shot.shotTransform_.modelId);
	}
}

