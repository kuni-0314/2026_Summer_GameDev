#include <DxLib.h>
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/InputManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Collider/Capsule/ColliderCapsule.h"
#include "../../../Collider/Sphere/ColliderSphere.h"
#include "../../../Collider/Line/ColliderLine.h"
#include "../../../Collider/Model/ColliderModel.h"
#include "../../../Common/AnimationController.h"
#include "../../../../Utility/AsoUtility.h"
#include "../../../../Object/Item/HP/HpItem.h"
#include "../../../../Object/Item/ItemManger.h"
#include "../../../../Sound/AudioManager.h"
#include "../../../../Scene/GameScene.h"
#include "./../../../../Application.h"
#include "../../Player/Player.h"
#include "EnemyDragon.h"

EnemyDragon::EnemyDragon(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:EnemyBase(data, attackModel, player),
	state_(STATE::NONE),
	step_(0.0f),
	isAliveBreath_(false)
{
}

EnemyDragon::~EnemyDragon()
{
}

void EnemyDragon::Draw(void)
{
	EnemyBase::Draw();

	
	DrawSphere3D(breathTopPos_, 10.0f, 10, 0xffffff, 0xffffff, true);

	
	DrawFormatString(300, 500, GetColor(255, 255, 255), "Pos: %f,%f,%f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

	DrawSphere3D(breathInfo_.transform.pos, 100, 5, 0xffffff, 0xffffff, false);

	//DrawCapsule3D(breathInfo_.transform.pos, breathDownPos_, 100, 5, 0xffffff, 0xffffff, false);

	for (int i = 0; i < tornadoCount_; i++)
	{
		if (tornadoInfo_[i].isDestory) continue;
		VECTOR Pos = tornadoInfo_[i].transform.pos;
		DrawSphere3D(Pos, 100.0f, 16, 0xff00ff, 0xff00ff, false);

	}

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
	//スケール設定
	transform_.scl = { SCALE,SCALE ,SCALE };

	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT);
	transform_.Update();

	//スケール設定
	MV1SetScale(transform_.modelId,transform_.scl);


	//ブレス位置取得
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(transform_.modelId, 16);
	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);
	// 位置を適用
	breathTopPos_ = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
	breathDownPos_ = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot = Quaternion::GetRotation(mat);
	//ブレス発射位置
	breathTopPos_ = VAdd(breathTopPos_, CAPSULE_ADD_BREATH_POS);
	//ブレス終了位置
	breathDownPos_ = VAdd(breathDownPos_, CAPSULE_DOWN_BREATH_POS);

	// フレーム22のワールドマトリクスを取得
	MATRIX mat2 = MV1GetFrameLocalWorldMatrix(transform_.modelId, 4);
	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset2 = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);
	// 位置を適用
	bodyPos_ = VGet(offset2.m[3][0], offset2.m[3][1], offset2.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot2 = Quaternion::GetRotation(mat);


}

void EnemyDragon::InitCollider()
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
		220.0f);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
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

	animationController_->Add(static_cast<int>(ANIM_TYPE::TORNADO)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/FlyAttack.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::LANDING)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/Landing.mv1");

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void EnemyDragon::InitPost()
{
	weight_ = WEIGHT::IMMOBILE;

	stateChanges_.emplace(static_cast<int>(STATE::IDLE),
		std::bind(&EnemyDragon::ChangeStateIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::FLY_IDLE),
		std::bind(&EnemyDragon::ChangeStateFlayIdle, this));

	stateChanges_.emplace(static_cast<int>(STATE::THINK),
		std::bind(&EnemyDragon::ChangeStateThink, this));

	stateChanges_.emplace(static_cast<int>(STATE::BREARH),
		std::bind(&EnemyDragon::ChangeStateBreath, this));

	stateChanges_.emplace(static_cast<int>(STATE::TORNADO),
		std::bind(&EnemyDragon::ChangeStateTornado, this));

	stateChanges_.emplace(static_cast<int>(STATE::LANDING),
		std::bind(&EnemyDragon::ChangeStateLanding, this));

	// 初期状態設定
	ChangeState(STATE::IDLE);
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
	breathTopPos_ = VAdd(breathTopPos_, CAPSULE_ADD_BREATH_POS);

	if (tornadoCoolTime_ > 0)
	{
		tornadoCoolTime_--;
	}

	//剣と魔法の当たり判定
	CheckPlayerSwordCollision();
	CheckPlayerMagicCollision();
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

void EnemyDragon::ChangeStateThink()
{
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

	useGrabity_ = false;
	pow = 1;

	animationController_->SetIgnoreRootMove(true);
	animationController_->SetDynamicOffset(true);

	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::FRY_IDLE), true);

}

void EnemyDragon::ChangeStateBreath()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateBreath, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::BREARH), false);
}

void EnemyDragon::ChangeStateTornado()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateTornado, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::TORNADO), true);
}

void EnemyDragon::ChangeStateLanding()
{

	stateUpdate_ = std::bind(&EnemyDragon::UpdateLanding, this);

	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::LANDING), false);

}

void EnemyDragon::UpdateThink()
{
}

void EnemyDragon::UpdateIdle()
{
	if (idleTime_ > changetime)
	{
		idleTime_ = 0;
		int rand = GetRand(100);
		if (rand > 70)
		{
			ChangeState(STATE::FLY_IDLE);
		}
		else
		{
			ChangeState(STATE::BREARH);
		}
	}

	idleTime_++;

	movePow_ = AsoUtility::VECTOR_ZERO;
}

void EnemyDragon::UpdateFlayIdle()
{
	int max = 500;


	//上昇量増加
	if (pow < max)
	{
		pow += 0.5;
	}
	if (transform_.pos.y < max)
	{
		transform_.pos.y += pow;
	}

	if(landing_)
	{
		transform_.pos.y -= 5;

		if (transform_.pos.y < -40)
		{
			landing_ = false;
			ChangeState(STATE::LANDING);
		}
	}

	transform_.Update();


	//最高高度に来たらトルネードを放つ
	if (transform_.pos.y >= max)
	{
		transform_.pos.y = max;

		//クールタイム
		if (tornadoCoolTime_ <= 0)
		{
			//地面に降りる場合
			if (!landing_)
			{
				int rand = GetRand(100);

				if (rand > 50)
				{
					ChangeState(STATE::FLY_IDLE);
				}
				else
				{
					rand = GetRand(100);
				}

				if (rand > 30)
				{
					ChangeState(STATE::TORNADO);
				}
				else
				{
					landing_ = true;
				}
			}
		}
	}
}

void EnemyDragon::UpdateBreath()
{
	//ブレスが生成されていない時のみ
	if (!isAliveBreath_)
	{
		//アニメーションコントローラーの取得
		if (animationController_ == nullptr) return;
		const auto& anim = animationController_->GetPlayAnim();
		if (anim.totalTime <= 0.0f) return;

		//アニメーションの指定時間までブレスを生成しない
		float breathTriggerTime = anim.totalTime * ATTACK_BREATH_TIME;
		if (anim.step >= breathTriggerTime)
		{
			//まだ生成されていない場合
			if (!isAliveBreath_)
			{
				//一度だけ生成
				CreateBreath();
				isAliveBreath_ = true;
			}
		}
	}
	//アニメーションが終了した場合
	if (animationController_->IsEnd())
	{
		//衝突情報削除
		DestoryBreathCollider(breathInfo_);
		//ブレス生存状態
		isAliveBreath_ = false;
		player_->SetWasHitDamage(false);
		//ブレス終了後、次の状態へ
		ChangeState(STATE::IDLE);
	}
	//ブレス位置更新
	breathInfo_.transform.pos = breathTopPos_;
	//更新
	breathInfo_.transform.Update();
}

void EnemyDragon::UpdateTornado()
{
	if (!isAliveTornado_)
	{

		isAliveTornado_ = true;
		CreateTornado();
	
	}

	bool allDelete = true;

	for (int i = 0; i < tornadoCount_; i++)
	{
		//コライダーを削除していた場合は次の配列へ
		if (tornadoInfo_[i].isDestory) continue;
		//トルネードのスピード設定
		const float speed = 10.0f;

		//トルネード更新処理
		tornadoInfo_[i].transform.pos =
			VAdd(tornadoInfo_[i].transform.pos,VScale(tornadoInfo_[i].moveDir, speed));

		tornadoInfo_[i].transform.Update();

		//移動距離
		float dist = VSize(VSub(tornadoInfo_[i].transform.pos, tornadoInfo_[i].startPos)); //移動距離

	
		if (dist >= MAX_DIST)
		{
			DestoryTornadoCollider(tornadoInfo_[i]);
			tornadoInfo_[i].isDestory = true;
		}
		else
		{
			allDelete = false;
		}
	}

	if (allDelete)
	{
		isAliveTornado_ = false;
		player_->SetWasHitTornadoDamage(false);
		tornadoCoolTime_ = TORNADO_RESET_TIME;
		ChangeState(STATE::FLY_IDLE);
	}
	
}

void EnemyDragon::UpdateLanding()
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void EnemyDragon::CreateBreathCollider(BreathInfo& breathInfo)
{
	//コライダー生成
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY_DRAGON_BREATH, &breathInfo.transform,
		CAPSULE_ADD_BREATH_POS,CAPSULE_DOWN_BREATH_POS, 100.0f);

	breathInfo_.collider = colCapsule;

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	//衝突情報追加
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->AddPlayerHitCollider(colCapsule);
}

void EnemyDragon::DestoryBreathCollider(BreathInfo& breathInfo)
{
	//衝突情報削除
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->RemovePlayerHitCollider(breathInfo.collider);

	delete breathInfo.collider;
	breathInfo.collider = nullptr;
}

void EnemyDragon::CreateBreath()
{
	//ブレス生成時にブレスの情報を初期化
	breathInfo_ = BreathInfo();
	//ブレスの開始位置設定
	breathInfo_.transform.pos = breathTopPos_;

	CreateBreathCollider(breathInfo_);
}


void EnemyDragon::CreateTornadoCollider(TornadoInfo& tornadoInfo)
{
	//コライダー生成
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY_DRAGON_TORNADO, &tornadoInfo.transform,
		CAPSULE_TOP_TORUNADO_POS, CAPSULE_DOWN_TORUNADO_POS, 100.0f);

	tornadoInfo.collider = colCapsule;
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	//衝突情報追加
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->AddPlayerHitCollider(colCapsule);

}


void EnemyDragon::DestoryTornadoCollider(TornadoInfo& tornadoInfo)
{
	//衝突情報削除
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->RemovePlayerHitCollider(tornadoInfo.collider);

	delete tornadoInfo.collider;
	tornadoInfo.collider = nullptr;
}

void EnemyDragon::CreateTornado()
{
	for (int i = 0; i < tornadoCount_; i++)
	{
		const float spawnRadius = 1000.0f;
		const float tornadoY = 90.0f;

		// プレイヤー座標
		VECTOR playerPos = player_->GetPos();

		// 0～360度
		float angle = AsoUtility::Deg2RadF(static_cast<float>(GetRand(359)));

		//トルネードの高さを固定
		tornadoInfo_[i].transform.pos =
		{
			playerPos.x + cosf(angle) * spawnRadius,
			tornadoY,
			playerPos.z + cosf(angle) * spawnRadius
		};
		tornadoInfo_[i].startPos = tornadoInfo_[i].transform.pos;

		VECTOR dir = VSub(playerPos, tornadoInfo_[i].transform.pos);
		dir.y = 0.0f;
		tornadoInfo_[i].moveDir = VNorm(dir);

		tornadoInfo_[i].transform.Update();
		tornadoInfo_[i].isDestory = false;
		tornadoInfo_[i].wasHitPlayer = false;


		//コライダー生成
		CreateTornadoCollider(tornadoInfo_[i]);
	}

}



