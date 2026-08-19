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
	isAliveBreath_(false),
	isAliveTornado_(false),
	isAliveClow_(false),
	landing_(false),
	tornadoCoolTime_(0)
{
}

EnemyDragon::~EnemyDragon()
{
}

void EnemyDragon::Draw(void)
{
	EnemyBase::Draw();

#ifdef _DEBUG
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

	//クロー攻撃
	DrawSphere3D(clowInfo_.transform.pos, 200.0f, 10, 0xffffff, 0xffffff, false);

#endif

}

void EnemyDragon::Release(void)
{ 

	EffectManager::GetInstance().Release();
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
	//MV1SetScale(transform_.modelId,transform_.scl);


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
	MATRIX offset2 = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat2);
	// 位置を適用
	bodyPos_ = VGet(offset2.m[3][0], offset2.m[3][1], offset2.m[3][2]);
	// 回転をQuaternionに変換
	Quaternion rot2 = Quaternion::GetRotation(mat2);


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

	animationController_->Add(static_cast<int>(ANIM_TYPE::CLOW)
		, 20.0f, Application::PATH_MODEL + "Enemy/Dragon/AttackWingClaw.mv1");


	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);


	//animationController_->SetupRootMotionControl(true, "Root");
	animationController_->SetRootFrameParams(true, "Root", { 0.0f, 2.458f, 0.0f });
	animationController_->SetDynamicOffset(false);
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

	stateChanges_.emplace(static_cast<int>(STATE::CLOW),
		std::bind(&EnemyDragon::ChangeStateClow, this));

	// 初期状態設定
	ChangeState(STATE::CLOW);
}

void EnemyDragon::UpdateProcess()
{
	//スケール設定
	transform_.scl = { SCALE,SCALE ,SCALE };
	transform_.Update();

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


	//プレイヤー視認状態
	if (look_)
	{
		LookPlayer();
	}

	//==============================
	// クロー位置更新
	//==============================
	MATRIX matclow =
		MV1GetFrameLocalWorldMatrix(
			transform_.modelId, 10);

	MATRIX offsetclow = MMult(
		MGetTranslate(VGet(0.0f, 0.0f, -3.0f)),
		matclow);

	clowInfo_.transform.pos =
		VGet(offsetclow.m[3][0],
			offsetclow.m[3][1],
			offsetclow.m[3][2]);

	VECTOR test = { 0,80,450 };

	clowInfo_.transform.pos = VAdd(clowInfo_.transform.pos, test);

	// Transform更新
	clowInfo_.transform.Update();

	//
	if (tornadoCoolTime_ > 0)
	{
		tornadoCoolTime_--;
	}

	//剣と魔法の当たり判定
	CheckPlayerSwordCollision();
	CheckPlayerMagicCollision();

	ResolvePushWithPlayer();

	if (hp_ <= 0)
	{
		MV1DeleteModel(transform_.modelId);
	}
}

void EnemyDragon::UpdateProcessPost()
{
	//状態更新
	stateUpdate_();

}

void EnemyDragon::LandingEffect(const VECTOR& pos, const VECTOR& normal, float size)
{
	auto effect = std::make_shared<EffekseerEffect>(
		L"Data/Effect/Smoke/Smoke2.efkefc",
		transform_.pos);

	effect->Play(transform_.pos, transform_.quaRot);

	EffectManager::GetInstance().RegisterEffect(effect);
}

void EnemyDragon::CreateClow()
{
	// 既にコライダが残っていたら先に削除（安全措置）
	if (clowInfo_.collider != nullptr)
	{
		DestroyClowColier(clowInfo_);
	}

	//ブレス生成時にブレスの情報を初期化
	clowInfo_ = ClowInfo();
	clowInfo_.colliderRegistered = false;
	clowInfo_.colliderActiveDelay = COLLIDER_ACTIVE_DELAY;

	CreateClowCollider(clowInfo_);
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
	useGrabity_ = true;
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
	pow = 1;  // 離陸エフェクト未再生

	isTakeOffEffect_ = false;
	
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::FRY_IDLE), true);
}

void EnemyDragon::ChangeStateBreath()
{
	stateUpdate_ = std::bind(&EnemyDragon::UpdateBreath, this);

	look_ = true;

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

void EnemyDragon::ChangeStateClow()
{

	stateUpdate_ = std::bind(&EnemyDragon::UpdateClow, this);
	look_ = false;

	// ランダムな待機時間
	step_ = 3.0f + static_cast<float>(GetRand(3));
	// 移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	// 待機アニメーション再生
	animationController_->Play(
		static_cast<int>(ANIM_TYPE::CLOW), false);
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

	// 地上から飛び立つ時だけ
	if (!isTakeOffEffect_ && transform_.pos.y <= 0.0f)
	{
		isTakeOffEffect_ = true;

		AudioManager::GetInstance()->SetSeVolume(200);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_DRAGON_LANDING);

		// 離陸エフェクトを再生
		LandingEffect(transform_.pos, AsoUtility::VECTOR_ZERO, 1.0f);
	}


	if(landing_)
	{
		transform_.pos.y -= 5;

		if (transform_.pos.y < -40)
		{
			landing_ = false;

			AudioManager::GetInstance()->SetSeVolume(200);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_DRAGON_LANDING);

			LandingEffect(transform_.pos, AsoUtility::VECTOR_ZERO, 1.0f);
		
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
					//地面に降りる
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
		float breathTriggerTime = anim.totalTime * ATTACK_FREAM_BREATH_TIME;
		if (anim.step >= breathTriggerTime)
		{
			//まだ生成されていない場合
			if (!isAliveBreath_)
			{
				//一度だけ生成
				CreateBreath();
				isAliveBreath_ = true;
				look_ = false;
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
		if (breathInfo_.effect)
		{
			breathInfo_.effect->Stop();
			breathInfo_.effect.reset();
		}
	}
	//ブレス位置更新
	breathInfo_.transform.pos = breathTopPos_;

	//// ドラゴンの正面方向
	//VECTOR dir = transform_.quaRot.GetForward();

	//// 回転を作成
	//Quaternion breathRot = Quaternion::LookRotation(dir);

	//breathInfo_.transform.quaRot = breathRot;
	breathInfo_.transform.Update();

	if (breathInfo_.effect)
	{
		breathInfo_.effect->SetPosition(breathTopPos_);
		//breathInfo_.effect->SetRotation(breathRot);
	}
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
		if (tornadoInfo_[i].effect)
		{
			tornadoInfo_[i].effect->SetPosition(
				tornadoInfo_[i].transform.pos);

			tornadoInfo_[i].effect->SetRotation(
				Quaternion::LookRotation(
					tornadoInfo_[i].moveDir));
		}

		//移動距離
		float dist = VSize(VSub(tornadoInfo_[i].transform.pos, tornadoInfo_[i].startPos)); //移動距離

	
		if (dist >= MAX_DIST)
		{
			if (tornadoInfo_[i].effect)
			{
				tornadoInfo_[i].effect->Stop();
				tornadoInfo_[i].effect.reset();
			}
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
		isTakeOffEffect_ = false;
		ChangeState(STATE::IDLE);
	}
}

void EnemyDragon::UpdateClow()
{
	if (!isAliveClow_)
	{
		//アニメーションコントローラーの取得
		if (animationController_ == nullptr) return;
		const auto& anim = animationController_->GetPlayAnim();
		if (anim.totalTime <= 0.0f) return;

		//アニメーションの指定時間までブレスを生成しない
		float breathTriggerTime = anim.totalTime * ATTACK_FRAME_CLOW_TIME;
		if (anim.step >= breathTriggerTime)
		{
			//ブレスが生成されていない時のみ
			if (!isAliveClow_)
			{
				//一度だけ生成
				CreateClow();
				isAliveClow_ = true;
			}
		}
	
	}

	//アニメーションが終了した場合
	if (animationController_->IsEnd())
	{
		//衝突情報削除
		DestroyClowColier(clowInfo_);
		//ブレス生存状態
		isAliveClow_ = false;
		player_->SetWasHitClow(false);
		//ブレス終了後、次の状態へ
		ChangeState(STATE::IDLE);
	
	}
	
	clowInfo_.transform.Update();

}

void EnemyDragon::ResolvePushWithPlayer()
{
	if (player_ == nullptr) return;

	// プレイヤーとドラゴンの位置
	VECTOR playerPos = player_->GetPos();
	VECTOR enemyPos = transform_.pos;

	// 水平方向のみで判定
	VECTOR dir = VSub(playerPos, enemyPos);
	dir.y = 0.0f;

	float dist = VSize(dir);

	// 距離がほぼゼロの場合
	if (dist < 0.0001f)
	{
		dir = VGet(1.0f, 0.0f, 0.0f);
		dist = 1.0f;
	}

	// 最低許容距離より近ければプレイヤーだけ押し出す
	if (dist < PUSH_MIN_SEPARATION)
	{
		float overlap = PUSH_MIN_SEPARATION - dist;

		// プレイヤーを押す方向
		VECTOR pushDir = VNorm(dir);

		// プレイヤーだけ移動
		VECTOR movePlayer = VScale(pushDir, overlap);

		VECTOR newPlayerPos = VAdd(playerPos, movePlayer);

		// Y座標は変更しない
		newPlayerPos.y = playerPos.y;

		// プレイヤーだけ位置を更新
		player_->SetPos(newPlayerPos);
	}
}

void EnemyDragon::CreateBreathCollider(BreathInfo& breathInfo)
{
	//コライダー生成
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::ENEMY_DRAGON_BREATH, &breathInfo.transform,
		CAPSULE_ADD_BREATH_POS,CAPSULE_DOWN_BREATH_POS, 100.0f);

	breathInfo.collider = colCapsule;

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

	AudioManager::GetInstance()->SetSeVolume(150);
	AudioManager::GetInstance()->PlaySE(SoundID::SE_DRAGON_SHOUT);
	AudioManager::GetInstance()->PlaySE(SoundID::SE_DRAGON_BREATH);

	CreateBreathCollider(breathInfo_);
	//ドラゴンが生きてる間
	if (isAlive_ == true)
	{
		// ブレスエフェクト生成
		breathInfo_.effect =
			std::make_shared<EffekseerEffect>(
				L"Data/Effect/Breath/Breath2.efkefc",
				breathTopPos_);

		breathInfo_.effect->Play(breathTopPos_, transform_.quaRot);

		EffectManager::GetInstance().RegisterEffect(breathInfo_.effect);
	}
	else
	{
		// エフェクト停止
		if (breathInfo_.effect)
		{
			breathInfo_.effect->Stop();
			breathInfo_.effect.reset();
		}
	}
};


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

void EnemyDragon::CreateClowCollider(ClowInfo& clowInfo)
{
	ColliderSphere* colSphere = new ColliderSphere(
		ColliderBase::TAG::ENEMY_DRAGON_CLOW,
		&clowInfo.transform,
		AsoUtility::VECTOR_ZERO,
		200.0f);

	clowInfo.collider = colSphere;

	ownColliders_.emplace(
		static_cast<int>(COLLIDER_TYPE::SPHERE),
		colSphere);

	//衝突情報追加
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->AddPlayerHitCollider(colSphere);
}

void EnemyDragon::DestroyClowColier(ClowInfo& clowInfo)
{
	//衝突情報削除
	GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetInstance().GetScene());
	gameScene->RemovePlayerHitCollider(clowInfo.collider);

	delete clowInfo.collider;
	clowInfo.collider = nullptr;
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
	AudioManager::GetInstance()->SetSeVolume(150);
	AudioManager::GetInstance()->PlaySE(SoundID::SE_DRAGON_TORNADO);

	const float spawnRadius = 1500.0f;
	const float tornadoY = 30.0f;

	// プレイヤー座標
	VECTOR playerPos = player_->GetPos();

	for (int i = 0; i < tornadoCount_; i++)
	{
		// 円周上に均等配置
		float angle =
			2.0f * DX_PI_F * static_cast<float>(i)
			/ static_cast<float>(tornadoCount_);

		// トルネードの生成位置
		tornadoInfo_[i].transform.pos =
		{
			playerPos.x + cosf(angle) * spawnRadius,
			tornadoY,
			playerPos.z + sinf(angle) * spawnRadius
		};

		tornadoInfo_[i].startPos =
			tornadoInfo_[i].transform.pos;

		// プレイヤー方向を計算
		VECTOR dir =
			VSub(playerPos, tornadoInfo_[i].transform.pos);

		dir.y = 0.0f;

		tornadoInfo_[i].moveDir = VNorm(dir);

		tornadoInfo_[i].transform.Update();

		tornadoInfo_[i].isDestory = false;
		tornadoInfo_[i].wasHitPlayer = false;

		// コライダー生成
		CreateTornadoCollider(tornadoInfo_[i]);

		// ドラゴンが生きている間
		if (isAlive_)
		{
			// エフェクト生成
			tornadoInfo_[i].effect =
				std::make_shared<EffekseerEffect>(
					L"Data/Effect/Breath/Breath3.efkefc",
					tornadoInfo_[i].transform.pos);

			tornadoInfo_[i].effect->Play(
				tornadoInfo_[i].transform.pos,
				Quaternion::LookRotation(
					tornadoInfo_[i].moveDir));

			EffectManager::GetInstance().RegisterEffect(
				tornadoInfo_[i].effect);
		}
		else
		{
			// エフェクト停止
			if (tornadoInfo_[i].effect)
			{
				tornadoInfo_[i].effect->Stop();
				tornadoInfo_[i].effect.reset();
				tornadoInfo_[i].effect->IsDead();
			}
		}
	}

}



