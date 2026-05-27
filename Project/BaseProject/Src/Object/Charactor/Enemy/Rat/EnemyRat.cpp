
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


EnemyRat::EnemyRat(const EnemyBase::EnemyData& data, Player* player)
	:EnemyBase(data, player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

EnemyRat::~EnemyRat(void)
{
}

void EnemyRat::Draw(void)
{

	// 基底クラスの描画処理
	CharactorBase::Draw();


	//デバッグ用攻撃範囲描画
	VECTOR local = ATTACK_SPHERE_LOCAL_POS;
	// 回転を適用
	VECTOR rotated = transform_.quaRot.PosAxis(local);

	// ワールド座標へ
	worldPos = VAdd(transform_.pos, rotated);

	if (stateBase_ == static_cast<int>(STATE::ATTACK))
	{
		DrawSphere3D(worldPos,
			COL_SPHERE_RADIUS, 10, 0x0000ff, 0x0000ff, false);
	}

	DrawSphere3D(worldPos,
		COL_SWICH_RADIUS, 10, 0x0000ff, 0x0000ff, false);
}



void EnemyRat::InitLoad(void)
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RAT));


}

void EnemyRat::InitTransform(void)
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	//transform_.pos = { 0.0f, 100.0f, 1500.0f };

	transform_.Update();
}

void EnemyRat::InitCollider(void)
{
	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);


}



void EnemyRat::InitAnimation(void)
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	int type = -1;

	//待機
	type = static_cast<int>(ANIM_TYPE::IDLE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_IDLE);

	type = static_cast<int>(ANIM_TYPE::WALK);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_WALK);

	type = static_cast<int>(ANIM_TYPE::ATTACK);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_ATTACK);

	type = static_cast<int>(ANIM_TYPE::END);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_END);

	type = static_cast<int>(ANIM_TYPE::HIT);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_HIT);

	type = static_cast<int>(ANIM_TYPE::RUN);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_HIT);

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);


}

void EnemyRat::InitPost(void)
{
	// 状態遷移初期処理登録
	stateChanges_.emplace(static_cast<int>(STATE::NONE),
		std::bind(&EnemyRat::ChangeStateNone, this));
	stateChanges_.emplace(static_cast<int>(STATE::THINK),
		std::bind(&EnemyRat::ChangeStateThink, this));
	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyRat::ChangeStateIdle, this));
	stateChanges_.emplace(static_cast<int>(STATE::WANDER),
		std::bind(&EnemyRat::ChangeStateWander, this));
	stateChanges_.emplace(static_cast<int>(STATE::ATTACK),
		std::bind(&EnemyRat::ChangeStateAttack, this));
	stateChanges_.emplace(static_cast<int>(STATE::HIT),
		std::bind(&EnemyRat::ChangeStateHit, this));
	stateChanges_.emplace(static_cast<int>(STATE::DIE),
		std::bind(&EnemyRat::ChangeStateDie, this));
	stateChanges_.emplace(static_cast<int>(STATE::RUN),
		std::bind(&EnemyRat::ChangeStateRun, this));
	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&EnemyRat::ChangeStateEnd, this));
	// 初期状態設定
	ChangeState(STATE::THINK);
}

void EnemyRat::UpdateProcess(void)
{

	stateUpdate_();

	VECTOR playerPos = player_->GetPos();
	float playerRad = player_->GetcollRadius_();

	//一度プレイヤーを見つけるとずっと追従する
	if (look_)
	{
		LookPlayer();
	}
	else if(AsoUtility::IsHitSpheres(worldPos, COL_SWICH_RADIUS, playerPos, playerRad))
	{
		look_ = true;
	}

	auto const ins = InputManager::GetInstance();


	if (ins->IsTrgDown(KEY_INPUT_1))
	{

		Damege(5);

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

void EnemyRat::UpdateProcessPost(void)
{
	EnemyBase::UpdateProcessPost();

	if (!InMovableRange())
	{
		//移動可能範囲外に出たら移動座標に戻す
		transform_.pos = prevPos_;
		transform_.Update();

		//思考状態に戻す
		ChangeState(STATE::THINK);
	}

}

void EnemyRat::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}

void EnemyRat::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateNone, this);
}
void EnemyRat::ChangeStateThink(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateThink, this);

	// 思考
	// ランダムに次の行動を決定
	// 30%で待機、70%で徘徊
	int rand = GetRand(100);
	if (rand < 20)
	{
		ChangeState(STATE::IDLE);
	}
	else
	{
		ChangeState(STATE::WANDER);
	}
}
void EnemyRat::ChangeStateIdle(void)
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
void EnemyRat::ChangeStateWander(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateWander, this);

	// ランダムな角度
	float angle = static_cast<float>(GetRand(360)) * DX_PI_F / 180.0f;
	// 移動方向
	moveDir_ = VGet(cosf(angle), 0.0f, sinf(angle));
	// ランダムな移動時間
	step_ = 2.0f + static_cast<float>(GetRand(5));
	// 移動スピード
	moveSpeed_ = 3.0f;
	// 歩きアニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::WALK), true);
}
void EnemyRat::ChangeStateAttack(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateAttack, this);

	// ランダムな角度
	float angle = static_cast<float>(GetRand(360)) * DX_PI_F / 180.0f;
	// 移動方向
	moveDir_ = VGet(cosf(angle), 0.0f, sinf(angle));

	movePow_ = AsoUtility::VECTOR_ZERO;

	// 攻撃アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK), true);
}
void EnemyRat::ChangeStateHit(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateHit, this);
	movePow_ = AsoUtility::VECTOR_ZERO;

	//死亡アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::HIT), false);
}
void EnemyRat::ChangeStateDie(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateDie, this);
	movePow_ = AsoUtility::VECTOR_ZERO;

	//死亡アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::END), false);
}
void EnemyRat::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateEnd, this);
	
}

void EnemyRat::ChangeStateRun(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateRun, this);

	// ランダムな角度
	float angle = static_cast<float>(GetRand(360)) * DX_PI_F / 180.0f;
	// 移動方向
	moveDir_ = VGet(cosf(angle), 0.0f, sinf(angle));
	// ランダムな移動時間
	step_ = 2.0f + static_cast<float>(GetRand(5));
	// 移動スピード
	moveSpeed_ = 10.0f;
	// 歩きアニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::RUN), true);
}

void EnemyRat::UpdateNone(void)
{
}

void EnemyRat::UpdateThink(void)
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

void EnemyRat::UpdateWander(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}

	// 移動する ← 追加
	movePow_ = VScale(moveDir_, moveSpeed_);
}

void EnemyRat::UpdateAttack(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}


	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRat::UpdateHit(void)
{
	
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::THINK);
		return;
	}

}

void EnemyRat::UpdateDie(void)
{
	if (animationController_->IsEnd())
	{
		MV1DeleteModel(transform_.modelId);
		ChangeState(STATE::END);
	}
	movePow_ = AsoUtility::VECTOR_ZERO;

}

void EnemyRat::UpdateEnd(void)
{

	
}

void EnemyRat::UpdateRun(void)
{
}




