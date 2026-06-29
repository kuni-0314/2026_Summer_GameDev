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
	hoverTime_(0.0f)
{
}

EnemyLarge::~EnemyLarge()
{
}

void EnemyLarge::Draw(void)
{
	// 基底クラスの描画処理
	CharactorBase::Draw();



#ifdef _DEBUG

#endif //_DEBUG


	STATE next = state_;
	const char* name = "";
	if (next == STATE::THINK) name = "THINK";
	else if (next == STATE::IDLE) name = "IDlE";
	else if (next == STATE::ATTACK_PUNCH) name = "ATTACK_PUNCH";
	else if (next == STATE::ATTACK_RUN) name = "ATTACK_RUN";
	else if (next == STATE::CHARGE) name = "CHARGE";

	DrawFormatString(0, 500, GetColor(255, 255, 255), "STATE: %s", name);


}

void EnemyLarge::Release(void)
{
}

void EnemyLarge::InitLoad()
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_LARGE));
	

}

void EnemyLarge::InitTransform()
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.pos = { 0.0f, 100.0f, 1500.0f };

	transform_.Update();
}

void EnemyLarge::InitCollider()
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


	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
	
}

void EnemyLarge::InitPost()
{

	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyLarge::ChangeStateIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::MOVE),
		std::bind(&EnemyLarge::ChangeStateMove, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_RUN),
		std::bind(&EnemyLarge::ChangeStateAttackRun, this));

	stateChanges_.emplace(static_cast<int>(STATE::ATTACK_PUNCH),
		std::bind(&EnemyLarge::ChangeStateAttackPunch, this));

	stateChanges_.emplace(static_cast<int>(STATE::CHARGE),
		std::bind(&EnemyLarge::ChangeStateCharge, this));


	// 初期状態設定
	ChangeState(STATE::IDLE);
}

void EnemyLarge::UpdateProcess()
{
	playerPos_ = player_->GetPos();
	playerRad_ = player_->GetCollRadius();

	// プレイヤー方向
	toPlayer_ = VSub(playerPos_, transform_.pos);

	//プレイヤーとの距離測定
	distance_ = VSize(toPlayer_);
	
	STATE state = state_;

	if(look_)
	{
		LookPlayer();
	}
}

void EnemyLarge::UpdateProcessPost()
{
	stateUpdate_();

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

void EnemyLarge::ChangeStateCharge()
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateCharge, this);

	look_ = true;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyLarge::ChangeStateAttackPunch(void)
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateAttackPunch, this);

	look_ = false;

	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::ATTACK_PUNCH), false);
}

void EnemyLarge::ChangeStateAttackRun(void)
{
	stateUpdate_ = std::bind(&EnemyLarge::UpdateRunAttack, this);

	look_ = true;

	moveSpeed_ = 6.0f;

	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::MOVE), true);
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

	if (count < countUp)
	{
		countUp = 0;
		ChangeState(STATE::CHARGE);
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyLarge::UpdateCharge()
{
	countUp++;

	if (count/2 < countUp)
	{
		countUp = 0;
		ChangeState(STATE::ATTACK_RUN);
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyLarge::UpdateAttackPunch(void)
{
	if(animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyLarge::UpdateRunAttack(void)
{

	//進行方向を決める
	if(look_)
	{
		//突進開始位置
		startPos = transform_.pos;
		look_ = false;
	}

	//進行方向を決めた後移動
	//移動距離
	float dist =VSize(VSub(transform_.pos, startPos));

	if (dist > ATTACK_RUN_END_POINT)
	{
		ChangeState(STATE::ATTACK_PUNCH);
	}

	movePow_ = VScale(moveDir_, moveSpeed_);
	
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