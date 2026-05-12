#include "EnemyRat.h"
#include "../../Player/Player.h"
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Collider/Line/ColliderLine.h"
#include "../../../Common/AnimationController.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Manager/InputManager.h"



EnemyRat::EnemyRat(const EnemyBase::EnemyData& data,Player*player)
	:EnemyBase(data,player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

EnemyRat::~EnemyRat(void)
{
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
	
	animationController_->AddInFbx(static_cast<int>(ANIM_TYPE::WALK), 30.0f, 8);


	int type = -1;

	//待機
	type = static_cast<int>(ANIM_TYPE::IDLE);
	animationController_->AddInFbx(type, 20.0f, type);
	
	type = static_cast<int>(ANIM_TYPE::WALK);
	animationController_->AddInFbx(type, 20.0f, type);

	type = static_cast<int>(ANIM_TYPE::ATTACK);
	animationController_->AddInFbx(type, 20.0f, type);

	type = static_cast<int>(ANIM_TYPE::END);
	animationController_->AddInFbx(type, 20.0f, type);

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
	stateChanges_.emplace(static_cast<int>(STATE::CHASE),
		std::bind(&EnemyRat::ChangeStateChaseRat, this));
	stateChanges_.emplace(static_cast<int>(STATE::END),
		std::bind(&EnemyRat::ChangeStateEnd, this));
	// 初期状態設定
	ChangeState(STATE::THINK);



}

void EnemyRat::UpdateProcess(void)
{
	stateUpdate_();

	// シーン遷移
	auto const ins = InputManager::GetInstance();


	// プレイヤーが攻撃範囲内か判定
	if (IsPlayerInAttackRange())
	{
		ChangeState(STATE::ATTACK);
	}
	// プレイヤーが追跡範囲内か判定
	else if (IsPlayerInChaseRange())
	{
		ChangeState(STATE::CHASE);
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
	if (rand < 30)
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

	// 攻撃時間の初期化
	step_ = 1.0f;

	// 攻撃アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK), true);

	// プレイヤーの方向を向く
	FacePlayer();

	// 移動を停止
	movePow_ = AsoUtility::VECTOR_ZERO;

	attackCooldown_ = ATTACK_COOLDOWN;


}
void EnemyRat::ChangeStateChaseRat(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateChaseRat, this);

	// 歩きアニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::WALK), true);

	moveSpeed_ = CHASE_SPEED;
}
void EnemyRat::ChangeStateEnd(void)
{
	stateUpdate_ = std::bind(&EnemyRat::UpdateEnd, this);
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

	movePow_ = AsoUtility::VECTOR_ZERO;  // 移動しない
}

void EnemyRat::UpdateWander(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}

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
}

void EnemyRat::UpdateChaseRat(void)
{
	// プレイヤーが追跡範囲外に出たら思考状態に戻す
	if (!IsPlayerInChaseRange())
	{
		ChangeState(STATE::THINK);
		return;
	}

	// プレイヤーの方向を向く
	FacePlayer();

	// プレイヤーに向かって移動
	VECTOR playerPos = player_->GetPos();  // ← こう変更
	moveDir_ = VNorm(VSub(playerPos, transform_.pos));
	movePow_ = VScale(moveDir_, moveSpeed_);
}

void EnemyRat::UpdateEnd(void)
{
}

bool EnemyRat::IsPlayerInAttackRange(void) const
{
	if (!player_) return false;
	VECTOR playerPos = player_->GetPos();
	float dis = AsoUtility::SqrMagnitude(transform_.pos, playerPos);
	return dis < ATTACK_DISTANCE * ATTACK_DISTANCE;
}

bool EnemyRat::IsPlayerInChaseRange(void) const
{

	if (!player_) return false;

	VECTOR playerPos = player_->GetPos();
	float dis = AsoUtility::SqrMagnitude(transform_.pos, playerPos);
	return dis < CHASE_DISTANCE * CHASE_DISTANCE;
}

void EnemyRat::FacePlayer(void)
{
	VECTOR playerPos = player_->GetPos(); 
	VECTOR diff = VSub(playerPos, transform_.pos);
	diff.y = 0.0f;

	moveDir_ = VNorm(diff);

	transform_.rot.y = atan2(moveDir_.x, moveDir_.z);
	transform_.rot.y += AsoUtility::Deg2RadF(180.0f);

	transform_.rot.x = transform_.rot.z = 0.0f;

	MV1SetRotationXYZ(transform_.modelId, transform_.rot);
}
