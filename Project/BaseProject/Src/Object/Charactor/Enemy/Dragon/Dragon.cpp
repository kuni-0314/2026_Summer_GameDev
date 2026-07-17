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
#include "Dragon.h"

EnemyDragon::EnemyDragon(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f)
{
}

EnemyDragon::~EnemyDragon()
{
}

void EnemyDragon::Draw(void)
{
	EnemyBase::Draw();

	MV1DrawModel(transform_.modelId);
	DrawSphere3D(breathTopPos_, 10.0f, 10, 0xffffff, 0xffffff, true);
	DrawSphere3D(breathDownPos_, 10.0f, 10, 0xffffff, 0xffffff, true);


	DrawFormatString(300, 400, GetColor(255, 255, 255), "TopPos: %f,%f,%f", breathTopPos_.x, breathTopPos_.y, breathTopPos_.z); 
	DrawFormatString(300, 500, GetColor(255, 255, 255), "DownPos: %f,%f,%f", breathDownPos_.x, breathDownPos_.y, breathDownPos_.z);
	//DrawCapsule3D(breathTopPos_, breathDownPos_, 50.0f, 10, 0xffffff, 0xffffff,false);
}

void EnemyDragon::Release(void)
{
}

void EnemyDragon::InitLoad()
{
	//規定クラスのロード
	CharactorBase::InitLoad();
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_DRAGON));

} 


void EnemyDragon::InitTransform()
{
	
	transform_.scl = { SCALE,SCALE ,SCALE };

	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();

	//スケール設定
	MV1SetScale(transform_.modelId,transform_.scl);

	VECTOR testPos = { 0,50,500 };

	//座標設定
	MV1SetPosition(transform_.modelId, testPos);


	//ブレス位置取得
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(transform_.modelId, 16);
	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);
	// 位置を適用
	breathTopPos_ = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot = Quaternion::GetRotation(mat);

	//ブレス発射位置
	breathTopPos_ = VAdd(breathTopPos_, ADD_BREATH_POS);


	
	//breathTransform_.get()->pos = breathTopPos_;
}

void EnemyDragon::InitCollider()
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::ENEMY, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);


	// 主に壁や木などの衝突で使用するカプセルコライダ（前半分）
	//ColliderCapsule* colCapsule = new ColliderCapsule(
	//	ColliderBase::TAG::ENEMY, &transform_,
	//	COLBODY_CAPSULE_TOP_LOCAL_POS, COLBODY_CAPSULE_DOWN_LOCAL_POS,
	//	100.0f);

	//ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	//ブレスカプセルコライダ(player用)
	CreateBreathCollider(breathInfo_);

}

void EnemyDragon::InitAnimation()
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);

	//待機
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/Idle_1.mv1");
	//空中待機
	animationController_->Add(static_cast<int>(ANIM_TYPE::FRY_IDLE)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/FlyIdle.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::BREARH)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/Flame_Attack.mv1");

	animationController_->Play(static_cast<int>(ANIM_TYPE::BREARH), true);
}

void EnemyDragon::InitPost()
{

	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyDragon::ChangeStateIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::FLY_IDLE),
		std::bind(&EnemyDragon::ChangeStateFlayIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::BREARH),
		std::bind(&EnemyDragon::ChangeStateBreath, this));

	// 初期状態設定
	ChangeState(STATE::BREARH);
}

void EnemyDragon::UpdateProcess()
{

	//ブレス位置取得
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(transform_.modelId, 16);
	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);
	// 位置を適用
	breathTopPos_ = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot = Quaternion::GetRotation(mat);

	//ブレス発射位置
	breathTopPos_ = VAdd(breathTopPos_, ADD_BREATH_POS);

	VECTOR PbreathTopPos_ = breathTopPos_;
	VECTOR testPos = { 0,-100,200 };

	breathDownPos_ = VAdd(PbreathTopPos_, testPos);

	breathInfo_.transform.Update();

	CreateBreathCollider(breathInfo_);

}

void EnemyDragon::UpdateProcessPost()
{
	//状態更新
	stateUpdate_();
}


void EnemyDragon::ChangeState(STATE state)
{
	state_ = state;
	EnemyBase::ChangeState(static_cast<int>(state_));
}

void EnemyDragon::ChangeStateIdle()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateIdle, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyDragon::ChangeStateFlayIdle()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateFlayIdle, this);



	movePow_ = AsoUtility::VECTOR_ZERO;


	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::FRY_IDLE), true);

}

void EnemyDragon::ChangeStateBreath()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateIdle, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::BREARH), true);
}

void EnemyDragon::UpdateIdle()
{
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyDragon::UpdateFlayIdle()
{
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyDragon::UpdateBreath()
{
	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyDragon::CreateBreathCollider(BreathInfo& breathInfo)
{
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY_DRAGON_BREATH, &breathInfo.transform,
		breathTopPos_, breathDownPos_, 30.0f); 


	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}


