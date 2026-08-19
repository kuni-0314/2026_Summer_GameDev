#include <DxLib.h>
#include "EnemyRat.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Collider/Sphere/ColliderSphere.h"
#include "../../../Collider/Line/ColliderLine.h"
#include "../../../Common/AnimationController.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Object/Item/HP/HpItem.h"
#include "../../../../Object/Item/ItemManger.h"
#include "../../../../Sound/AudioManager.h"
#include "../../Player/Player.h"


EnemyRat::EnemyRat(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

EnemyRat::~EnemyRat()
{
}

void EnemyRat::Draw()
{

	// 親クラスの描画処理
	EnemyBase::Draw();


	STATE next = state_;

	const char* name = "";

	if (next == STATE::THINK) name = "THINK";
	else if (next == STATE::ATTACK) name = "ATTACK";
	else if (next == STATE::IDLE) name = "IDLE";
	else if (next == STATE::RUN) name = "RUN";
	else if (next == STATE::HIT) name = "HIT";
	else if (next == STATE::CHARGE) name = "CHARGE";

	//DrawFormatString(0, 350, GetColor(255, 255, 255), "RAT_STATE: %s", name);
}



void EnemyRat::InitLoad()
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RAT));


	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);


}

void EnemyRat::InitTransform()
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	//transform_.pos = { 0.0f, 100.0f, 1500.0f };

	transform_.Update();
}

void EnemyRat::InitCollider()
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



void EnemyRat::InitAnimation()
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	int type = -1;

	//待機
	type = static_cast<int>(ANIM_TYPE::IDLE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_IDLE);
	//歩く
	type = static_cast<int>(ANIM_TYPE::WALK);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_WALK);
	//攻撃
	type = static_cast<int>(ANIM_TYPE::ATTACK);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_ATTACK);

	type = static_cast<int>(ANIM_TYPE::END);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_END);
	//攻撃受け
	type = static_cast<int>(ANIM_TYPE::HIT);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_HIT);
	//走る
	type = static_cast<int>(ANIM_TYPE::RUN);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_RUN);

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);


}

void EnemyRat::InitPost()
{
	weight_ = WEIGHT::NORMAL;

	/// 必要な状態遷移のみにスッキリ整理
	stateChanges_.emplace(static_cast<int>(STATE::THINK), std::bind(&EnemyRat::ChangeStateThink, this));
	stateChanges_.emplace(static_cast<int>(STATE::IDLE), std::bind(&EnemyRat::ChangeStateIdle, this));
	stateChanges_.emplace(static_cast<int>(STATE::RUN), std::bind(&EnemyRat::ChangeStateRun, this)); // WANDERをMOVE扱いに変更、または新設
	stateChanges_.emplace(static_cast<int>(STATE::CHARGE), std::bind(&EnemyRat::ChangeStateCharge, this)); // ★追加
	stateChanges_.emplace(static_cast<int>(STATE::ATTACK), std::bind(&EnemyRat::ChangeStateAttack, this));
	stateChanges_.emplace(static_cast<int>(STATE::HIT), std::bind(&EnemyRat::ChangeStateHit, this));
	stateChanges_.emplace(static_cast<int>(STATE::DIE), std::bind(&EnemyRat::ChangeStateDie, this));
	stateChanges_.emplace(static_cast<int>(STATE::END), std::bind(&EnemyRat::ChangeStateEnd, this));

	// チャージ用カウント初期化（定数はヘッダ等で適宜定義してください）
	attackCharge_ = 60;

	// 初期状態設定
	ChangeState(STATE::THINK);

	look_ = false;
	pushOutRadius_ = 100.0f;
}

void EnemyRat::UpdateProcess()
{
	stateUpdate_();

	playerPos_ = player_->GetPos();
	playerRad_ = player_->GetCollRadius();

	// プレイヤー方向と距離の更新
	toPlayer_ = VSub(playerPos_, transform_.pos);
	distance_ = VSize(toPlayer_);

	// 索敵範囲の中にいる時だけプレイヤーを見る
	if (distance_ < SEARCH_DISTANCE){look_ = true;}
	if (look_){LookPlayer();}
	
	int rand = GetRand(100);
	if (5 < rand)
	{
		LookPlayer();
	}
	
	//デバッグ用攻撃範囲描画
	VECTOR local = ATTACK_SPHERE_LOCAL_POS;
	// 回転を適用
	VECTOR rotated = transform_.quaRot.PosAxis(local);

	// ワールド座標へ
	attackWorldPos_ = VAdd(transform_.pos, rotated);

	PushOutSphere(transform_.pos, pushOutRadius_,
		player_->GetPos(), player_->GetCollRadius(), true);

	//攻撃判定
	preHp_ = hp_;

	CheckPlayerSwordCollision();
	CheckPlayerMagicCollision();

	if (hp_ < preHp_)
	{
		if (hp_ <= 0)
		{
			ChangeState(STATE::DIE);
		}
		else
		{
			ChangeState(STATE::HIT);
		}
	}
}

void EnemyRat::UpdateProcessPost()
{
	

	if (!InMovableRange())
	{
		//移動可能範囲外に出たら移動座標に戻す
		transform_.pos = prevPos_;
		transform_.Update();

		//思考状態に戻す
		ChangeState(STATE::THINK);
		look_ = false;
	}

}

void EnemyRat::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}

void EnemyRat::ChangeStateNone()
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateNone, this);
}
void EnemyRat::ChangeStateThink()
{

	stateUpdate_ = std::bind(&EnemyRat::UpdateThink, this);
	step_ = 0.5f + static_cast<float>(GetRand(2));
	movePow_ = AsoUtility::VECTOR_ZERO;
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	
}
void EnemyRat::ChangeStateIdle()
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateIdle, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

}
void EnemyRat::ChangeStateAttack(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateAttack, this);
	isAttack_ = false;

	if (distance_ > 0.01f)	moveDir_ = VNorm(toPlayer_);

	AudioManager::GetInstance()->SetSeVolume(80);
	AudioManager::GetInstance()->PlaySE(SoundID::SE_ENEMY_RAT_ATTAK);

	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
}

void EnemyRat::ChangeStateCharge(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateCharge, this);
	movePow_ = AsoUtility::VECTOR_ZERO; // その場に立ち止まる

	// 待機アニメーション、または力を溜めるアニメ
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyRat::ChangeStateRun(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateRun, this);
	// ランダムな移動時間
	step_ = 2.0f + static_cast<float>(GetRand(5));
	// 移動スピード
	moveSpeed_ = 8.0f;
	// 歩きアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::RUN), true);
}

void EnemyRat::UpdateNone(void)
{
}

void EnemyRat::UpdateIdle(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRat::UpdateThink(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ > 0.0f)
	{
		// 考えている間は移動量をゼロにする
		movePow_ = AsoUtility::VECTOR_ZERO;
		return;
	}
	//まだプレイヤーを見つけていない状態
	if (!look_)
	{
		ChangeState(STATE::IDLE);
		return;
	}

	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::CHARGE); // 攻撃準備へ
	}
	else
	{
		ChangeState(STATE::RUN);   // 追いかける移動へ
	}
}

void EnemyRat::ChangeStateHit(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateHit, this);
	movePow_ = AsoUtility::VECTOR_ZERO;

	//死亡アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::HIT), false);
}
void EnemyRat::ChangeStateDie()
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateDie, this);
	movePow_ = AsoUtility::VECTOR_ZERO;

	//死亡アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::END), false);
}
void EnemyRat::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateEnd, this);

}

void EnemyRat::UpdateCharge(void)
{
	attackCharge_--;

	if (attackCharge_ <= 0)
	{
		ChangeState(STATE::ATTACK);
		// カウントをリセット
		attackCharge_ = ATTACK_CHARGE; 
	}

	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRat::UpdateAttack(void)
{
	// 飛び込み攻撃のように前進させる場合
	movePow_ = VScale(moveDir_, ATTACK_MOVE_SPEED);


	if (!isAttack_)
	{
		// 攻撃判定
		if (AsoUtility::IsHitSpheres(attackWorldPos_, COL_ATTACK_RADIUS, playerPos_, playerRad_))
		{
			player_->Damage(1, transform_.GetForward());
			isAttack_ = true;
		}
	}

	// アニメーションが終わったら思考状態へ戻る
	if (animationController_->IsEnd())
	{
		isAttack_ = false;
		ChangeState(STATE::THINK);
	}
}

void EnemyRat::UpdateHit()
{
	
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::THINK);
		return;
	}

}

void EnemyRat::UpdateDie()
{
	if (animationController_->IsEnd())
	{
		MV1DeleteModel(transform_.modelId);
		ChangeState(STATE::END);
	}
	movePow_ = AsoUtility::VECTOR_ZERO;

}

void EnemyRat::UpdateEnd()
{

	
}

void EnemyRat::UpdateRun()
{
	// 移動中に攻撃範囲に入ったら即チャージ
	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::CHARGE);
	}


	// 移動量を適用
	movePow_ = VScale(moveDir_, moveSpeed_);
}




