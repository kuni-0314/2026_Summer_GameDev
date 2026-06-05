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
#include "EnemyRase.h"

EnemyRase::EnemyRase(const EnemyBase::EnemyData& data, Player* player)
	:EnemyBase(data, player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

EnemyRase::~EnemyRase(void)
{
}

void EnemyRase::Draw(void)
{
	// 基底クラスの描画処理
	CharactorBase::Draw();
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
	animationController_->AddInFbx(type, 20.0f, ANIM_INDX_IDLE);
}

void EnemyRase::InitPost(void)
{
	// 初期状態設定
	ChangeState(STATE::IDLE);
}

void EnemyRase::UpdateProcess(void)
{
}

void EnemyRase::UpdateProcessPost(void)
{
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

void EnemyRase::ChangeStateThink(void)
{
	stateUpdate_ = std::bind(&EnemyRase::UpdateThink, this);

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);

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

void EnemyRase::UpdateThink(void)
{
	ChangeState(STATE::IDLE);
}