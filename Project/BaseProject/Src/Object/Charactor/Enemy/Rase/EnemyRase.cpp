#include <DxLib.h>
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
#include "../../Player/Player.h"
#include "../Shot/ShotBase.h"
#include "../Shot/ShotStraight.h"
#include "EnemyRase.h"


EnemyRase::EnemyRase(const EnemyBase::EnemyData& data, Player* player)
	:EnemyBase(data, player),
	state_(STATE::NONE),
	step_(0.0f),
	hoverTime_(0.0f),
	baseHeight_(0.0f)
{
}

EnemyRase::~EnemyRase(void)
{
}

void EnemyRase::Draw(void)
{
	// 基底クラスの描画処理
	CharactorBase::Draw();

	STATE next = state_;

	const char* name = "";

	if (next == STATE::THINK) name = "THINK";
	else if (next == STATE::ATTACK) name = "ATTACK";
	else if (next == STATE::IDLE) name = "IDLE";
	else if (next == STATE::MOVE) name = "MOVE";

	DrawFormatString(0, 400, GetColor(255, 255, 255), "STATE: %s", name);
}

void EnemyRase::Release(void)
{
	for (ShotBase* shot : shots_)
	{
		shot->Release();
	}
}

void EnemyRase::InitLoad(void)
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RASE));
}

void EnemyRase::InitTransform(void)
{
	//大きさ、座標等の初期化
	transform_.scl = { SCALE ,SCALE ,SCALE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	//transform_.pos = { 0.0f, 100.0f, 1500.0f };

	transform_.Update();
}

void EnemyRase::InitCollider(void)
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

void EnemyRase::InitAnimation(void)
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	int type = -1;

	//待機
	type = static_cast<int>(ANIM_TYPE::IDLE);
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_FRY);

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyRase::InitPost(void)
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


	// 初期状態設定
	ChangeState(STATE::THINK);
	
}

void EnemyRase::UpdateProcess(void)
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
		baseHeight_ +sinf(hoverTime_ * HOVER_SPEED) * HOVER_HEIGHT;
	//ちょっとした横揺れ
	transform_.pos.x += sinf(hoverTime_ * 0.7f) * 0.2f;
	

}

void EnemyRase::UpdateProcessPost(void)
{
	stateUpdate_();
}

void EnemyRase::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}

void EnemyRase::ChangeStateIdle(void)
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

void EnemyRase::ChangeStateAttack(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateAttack, this);

	// 有効な弾を取得する
	ShotBase* shot = GetValidShot();
	shot->CreateShot(shot->GetPos(), toPlayer_);

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

void EnemyRase::ChangeStateHit(void)
{
}

void EnemyRase::ChangeStateEnd(void)
{
}

void EnemyRase::ChangeStateThink(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateThink, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

}

void EnemyRase::UpdateIdle(void)
{
	step_ -= scnMng_.GetDeltaTime();
	if (step_ < 0.0f)
	{
		ChangeState(STATE::THINK);
		return;
	}

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::UpdateAttack(void)
{
	
	

	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyRase::UpdateMove(void)
{
	//攻撃範囲に入るまで移動
	if (distance_ < SWICH_DISTANCE)
	{
		ChangeState(STATE::ATTACK);
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
		ChangeState(STATE::ATTACK);
	}
	else
	{
		ChangeState(STATE::MOVE);
	}
}

void EnemyRase::UpdateHit(void)
{
}

void EnemyRase::UpdateDie(void)
{
}

void EnemyRase::UpdateEnd(void)
{
}

ShotBase* EnemyRase::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		// 未使用(生存していない)
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}
	// 新しい弾のインスタンスを生成する
	ShotBase* shot = new ShotStraight(ShotBase::TYPE::STRAIGHT, AttackModelId_);
	// 可変長配列に追加
	shots_.push_back(shot);
	return shot;
}

void EnemyRase::UpdateShot(void)
{
	for (ShotBase* shot : shots_)
	{
		shot->Update();
	}
}

void EnemyRase::DrawShot(void)
{
	for (ShotBase* shot : shots_)
	{
		shot->Draw();
	}
}

