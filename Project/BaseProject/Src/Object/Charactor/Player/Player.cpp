#include "Player.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Common/AnimationController.h"
#include "../../../Manager/Camera.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Object/Collider/ColliderBase.h"
#include "../../../Object/Collider/Line/ColliderLine.h"
#include "../../../Object/Collider/Capsule/ColliderCapsule.h"
#include "PlayerIdleState.h"


Player::Player(void)
	:
	CharactorBase()
{
}

Player::~Player(void)
{
}

void Player::Update()
{
	// 移動前座標を更新
	prevPos_ = transform_.pos;

	// 各キャラクターごとの更新処理
	UpdateProcess();
	// 移動方向に応じた遅延回転
	DelayRotate();
	// 重力による移動量
	CalcGravityPow();
	// 衝突判定前準備
	CollisionReserve();
	// 衝突判定
	Collision();
	// モデル制御更新
	transform_.Update();
	// アニメーション再生
	animationController_->Update();
	// 各キャラクターごとの更新後処理
	UpdateProcessPost();


	// hpのindexは4、luckのindexは9
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_UP))
	//{
	//	if (currentGrantStatusIndex_ <= 4)
	//	{
	//		currentGrantStatusIndex_ = MAX_STATUS_INDEX;
	//	}
	//	else
	//	{
	//		currentGrantStatusIndex_--;
	//	}
	//}
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_DOWN))
	//{
	//	if (currentGrantStatusIndex_ >= MAX_STATUS_INDEX)
	//	{
	//		currentGrantStatusIndex_ = 4;
	//	}
	//	else
	//	{
	//		currentGrantStatusIndex_++;
	//	}
	//}

	//// 矢印キー右でステータスにポイント割り振り
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_RIGHT))
	//{
	//	if (pendingPoints_ > 0)
	//	{
	//		GrantStatus(currentGrantStatusIndex_);
	//		pendingPoints_--;
	//	}
	//}

	//// 矢印キー左でステータスからポイントを戻す
	//if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_LEFT))
	//{
	//	RevokeStatus(currentGrantStatusIndex_);
	//}



}

//void Player::Update()
//{
//	animationController_->Update();
//
//	// 移動操作
//	ProcessMove();
//
//	// 移動処理
//	transform_.pos = VAdd(transform_.pos, movePow_);
//
//	transform_.Update();
//
//	
//}



void Player::InitLoad(void)
{
	//基底クラスのリソースロード
	CharactorBase::InitLoad();

	transform_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);
}

void Player::InitTransform(void)
{

	//大きさ、座標等の初期化
	transform_.scl = { SCL_PlAYER ,SCL_PlAYER ,SCL_PlAYER };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT_LOCAL_PLAYER);
	transform_.pos = POS_PLAYER;
	

	transform_.Update();
	
}

void Player::InitCollider(void)
{
	// 主に地面との衝突で仕様する線分コライダ
	ColliderLine* colLine = new ColliderLine(
	ColliderBase:: TAG::PLAYER, &transform_,
	COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::LINE), colLine);

	// 主に壁や木などの衝突で仕様するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS,
		COL_CAPSULE_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}

void Player::InitAnimation(void)
{
	//アニメーションコントローラー
	animationController_ = new AnimationController(transform_.modelId);
	//待機状態アニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE)
		, 20.0f, Application::PATH_MODEL + "Player/Idle.mv1");

	//走るアニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN)
		, 20.0f, Application::PATH_MODEL + "Player/Run.mv1");

	//走り始めアニメーション
	animationController_->Add(static_cast<int>(ANIM_TYPE::FAST_RUN)
		, 20.0f, Application::PATH_MODEL + "Player/FastRun.mv1");

	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP)
		, 60.0f, Application::PATH_MODEL + "Player/JumpRising.mv1");

	//初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Player::InitPost(void)
{
	status_.level = DEFAULT_LEVEL;
	status_.hp = DEFAULT_HP;
	status_.mp = DEFAULT_MP;
	status_.physAtk = DEFAULT_PHYS_ATK;
	status_.physDef = DEFAULT_PHYS_DEF;
	status_.magicAtk = DEFAULT_MAGIC_ATK;
	status_.magicDef = DEFAULT_MAGIC_DEF;
	status_.wisdom = DEFAULT_WISDOM;
	status_.luck = DEFAULT_LUCK;
	currentGrantStatusIndex_ = 4;
	pendingPoints_ = 30;
}

void Player::UpdateProcess(void)
{
	// 移動操作
	ProcessMove();

	// ジャンプ処理
	ProcessJump();

	// ジェット処理
	ProcessJet();

	// 
	ProcessAttack();

	

}

void Player::UpdateProcessPost(void)
{
}

void Player::Draw(void)
{
	//基底クラスの描画処理
	ActorBase::Draw();
	// 丸影の描画
	DrawShadow();

	// ステータス描画
	int x = 20;
	int y = 20;
	int lineHeight = 25;

	// 背景描画(半透明の黒)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(x - 10, y - 10, x + 250, y + lineHeight * 11 + 10, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// タイトル
	DrawFormatString(x, y, 0xFFFFFF, "=== Player Status ===");
	y += lineHeight;

	// ステータス情報を描画（選択中の項目を黄色でハイライト）
	unsigned int color = 0xFFFFFF;
	unsigned int highlightColor = 0xFFFF00;

	DrawFormatString(x, y, color, "Level  : %d", status_.level);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 2) ? highlightColor : color, "HP     : %d", status_.hp);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 3) ? highlightColor : color, "MP     : %d", status_.mp);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 4) ? highlightColor : color, "PhysAtk: %d", status_.physAtk);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 5) ? highlightColor : color, "PhysDef: %d", status_.physDef);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 6) ? highlightColor : color, "MagicAtk: %d", status_.magicAtk);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 7) ? highlightColor : color, "MagicDef: %d", status_.magicDef);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 8) ? highlightColor : color, "Wisdom : %d", status_.wisdom);
	y += lineHeight;

	DrawFormatString(x, y, (currentGrantStatusIndex_ == 9) ? highlightColor : color, "Luck   : %d", status_.luck);
	y += lineHeight;

	DrawFormatString(x, y, color, "PendingPoints   : %d", pendingPoints_);

	y += lineHeight;
	DrawFormatString(x, y, color, "jumpPow   : %f.", jumpPow_.y);

	// プレイヤーの周りを回転するオブジェクトの描画
	static float angle = 0.0f;
	angle += 0.01f;

	// プレイヤーの座標を中心に半径100.0fで回転
	float radius = 100.0f;
	VECTOR rot = transform_.pos;
	rot.x += sinf(angle) * radius;
	rot.z += cosf(angle) * radius;

	// プレイヤーの座標系を描画
	VECTOR lineX = { transform_.pos.x + 50.0f, transform_.pos.y, transform_.pos.z };
	VECTOR lineY = { transform_.pos.x, transform_.pos.y + 50.0f, transform_.pos.z };
	VECTOR lineZ = { transform_.pos.x, transform_.pos.y, transform_.pos.z + 50.0f };
	DrawLine3D(transform_.pos, lineX, 0xFF0000);
	DrawLine3D(transform_.pos, lineY, 0x00FF00);
	DrawLine3D(transform_.pos, lineZ, 0x0000FF);
	DrawSphere3D(lineX, 5.0f, 16, 0xFF0000, 0xFF0000, true);
	DrawSphere3D(lineY, 5.0f, 16, 0x00FF00, 0x00FF00, true);
	DrawSphere3D(lineZ, 5.0f, 16, 0x0000FF, 0x0000FF, true);
	DrawSphere3D(rot, 5.0f, 16, 0xFFFF00, 0xFFFF00, true);
}

void Player::GrantStatus(int index)
{
	switch (index)
	{
	case 4:
		status_.physAtk += 1;
		break;
	case 5:
		status_.physDef += 1;
		break;
	case 6:
		status_.magicAtk += 1;
		break;
	case 7:
		status_.magicDef += 1;
		break;
	case 8:
		status_.wisdom += 1;
		break;
	case 9:
		status_.luck += 1;
		break;
	default:
		break;
	}
}

void Player::RevokeStatus(int index)
{
	// 各ステータスが初期値以下にならないようにチェック
	switch (index)
	{
	case 2:
		if (status_.hp > DEFAULT_HP)
		{
			status_.hp -= 1;
			pendingPoints_++;
		}
		break;
	case 3:
		if (status_.mp > DEFAULT_MP)
		{
			status_.mp -= 1;
			pendingPoints_++;
		}
		break;
	case 4:
		if (status_.physAtk > DEFAULT_PHYS_ATK)
		{
			status_.physAtk -= 1;
			pendingPoints_++;
		}
		break;
	case 5:
		if (status_.physDef > DEFAULT_PHYS_DEF)
		{
			status_.physDef -= 1;
			pendingPoints_++;
		}
		break;
	case 6:
		if (status_.magicAtk > DEFAULT_MAGIC_ATK)
		{
			status_.magicAtk -= 1;
			pendingPoints_++;
		}
		break;
	case 7:
		if (status_.magicDef > DEFAULT_MAGIC_DEF)
		{
			status_.magicDef -= 1;
			pendingPoints_++;
		}
		break;
	case 8:
		if (status_.wisdom > DEFAULT_WISDOM)
		{
			status_.wisdom -= 1;
			pendingPoints_++;
		}
		break;
	case 9:
		if (status_.luck > DEFAULT_LUCK)
		{
			status_.luck -= 1;
			pendingPoints_++;
		}
		break;
	default:
		break;
	}
}

void Player::ProcessMove(void)
{
	auto ins = InputManager::GetInstance();

	//移動量
	if (!isJump_ && !isJet_) movePow_ = AsoUtility::VECTOR_ZERO;
	else if (isJump_ && !isJet_) VScale(movePow_, 0.9f); // ジャンプ中は移動量を徐々に減少させる

	if (jetTime_ <= TIME_JET)
	{
		jetTime_ += scnMng_.GetDeltaTime();
	}
	else
	{
		isJet_ = false;
	}

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// カメラの角度を取得
	VECTOR camAngles =
		SceneManager::GetInstance().GetCamera()->GetAngles();

	// ダッシュ判定
	bool isDash_ = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		if (ins->IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins->IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins->IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins->IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }
		// ダッシュキー
		if (ins->IsNew(KEY_INPUT_RSHIFT)) { isDash_ = true; }
		if (ins->IsNew(KEY_INPUT_LSHIFT)) { isDash_ = true; }
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		//InputManager::JOYPAD_IN_STATE padState =
		//	ins->GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// アナログキーの入力値から方向を取得
		short leftX, leftY;
		ins->GetLeftStick(0, leftX, leftY);

		// アナログスティックの入力値を正規化して方向ベクトルを作成
		float magnitude = sqrtf(leftX * leftX + leftY * leftY);
		if (magnitude > 0.0f)
		{
			dir.x = leftX / magnitude;
			dir.z = leftY / magnitude;
		}

		if (ins->IsGamepadNew(InputManager::PadInput::RB, 0))
		{
			isDash_ = true;
		}
	}


	if (!AsoUtility::EqualsVZero(dir))
	{
		if (isDash_)
		{
			moveSpeed_ = SPEED_DASH;
		}
		else
		{
			//移動スピード
			moveSpeed_ = SPEED_MOVE;
		}


		// ジャンプ中はアニメーションを変えない
		if (!isJump_)
		{
			// アニメーション
			if (isDash_)
			{
				
				animationController_->Play(
					static_cast<int>(ANIM_TYPE::FAST_RUN), true);
			}
			else
			{
				animationController_->Play(
					static_cast<int>(ANIM_TYPE::RUN), true);
			}
		}


		//Y軸のみのカメラ角度を取得
		Quaternion cameraRot = scnMng_.GetCamera()->GetQuaRotY();
		//移動方向をカメラに合わせる
		moveDir_ = Quaternion::PosAxis(cameraRot, dir);

		//移動量を計算
		if (!isJet_) movePow_ = VScale(moveDir_, moveSpeed_);

	}
	else
	{
		// ジャンプ中はアニメーションを変えない
		if (!isJump_)
		{
			// IDLE状態に戻す
			animationController_->Play(
				static_cast<int>(ANIM_TYPE::IDLE), true);
		}
	}
}
	

void Player::ProcessJump(void)
{
	auto ins = InputManager::GetInstance();

	bool isHitKey = ins->IsTrgDown(KEY_INPUT_SPACE)
		|| ins->IsGamepadTrgDown(InputManager::PadInput::B, 0);

	if (ins->IsTrgDown(KEY_INPUT_SPACE)
		|| ins->IsGamepadTrgDown(InputManager::PadInput::B, 0) && !isJump_)
	{
		jumpPow_ = VAdd(jumpPow_, VScale(AsoUtility::DIR_U, POW_JUMP_INIT));
		isJump_ = true;
		stepJump_ = 0.0f;
		
		// アニメーション再生
		animationController_->Play(
			static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	if (ins->IsNew(KEY_INPUT_SPACE)
		|| ins->IsGamepadNew(InputManager::PadInput::B, 0))
	{
		if (isJump_ && stepJump_ < TIME_JUMP_INPUT)
		{
			jumpPow_ = VAdd(jumpPow_, VScale(AsoUtility::DIR_U, POW_JUMP_KEEP));
			stepJump_ += scnMng_.GetDeltaTime();
		}
	}

	//// ジャンプ開始
	//if (isHitKey && !isJump_)
	//{
	//	// ジャンプ量の計算
	//	float jumpSpeed = POW_JUMP * scnMng_.GetDeltaTime();
	//	jumpPow_ = VScale(AsoUtility::DIR_U, jumpSpeed);
	//	isJump_ = true;
	//	stepJump_ = 0.0f;  // ジャンプ開始時にリセット
	//	// アニメーション再生
	//	animationController_->Play(
	//		static_cast<int>(ANIM_TYPE::JUMP), false);
	//}

	//// 持続ジャンプ処理（ジャンプ中のみ）
	//bool isHitKeyNew = ins->IsNew(KEY_INPUT_SPACE)
	//	|| ins->IsGamepadNew(InputManager::PadInput::Down, 0);
	//
	//if (isHitKeyNew && isJump_ && stepJump_ < TIME_JUMP_INPUT)
	//{
	//	// ジャンプの入力受付時間を加算
	//	stepJump_ += scnMng_.GetDeltaTime();
	//	// ジャンプ量の計算
	//	float jumpSpeed = POW_JUMP_KEEP * scnMng_.GetDeltaTime();
	//	jumpPow_ = VAdd(jumpPow_, VScale(AsoUtility::DIR_U, jumpSpeed));
	//}
}

void Player::ProcessJet(void)
{
	auto ins = InputManager::GetInstance();
	if (ins->IsTrgDown(KEY_INPUT_E)
		|| ins->IsGamepadTrgDown(InputManager::PadInput::X, 0))
	{
		movePow_ = VScale(moveDir_, POW_JET);
		isJet_ = true;
		jetTime_ = 0.0f;
	}
}

void Player::ProcessAttack(void)
{
	auto ins = InputManager::GetInstance();
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		jumpPow_ = VAdd(jumpPow_, VScale(AsoUtility::DIR_U, 20.0f));
	}
}

void Player::CollisionReserve(void)
{

	// アニメーションごとの線分調整
	if (animationController_->GetPlayType() == static_cast<int>(ANIM_TYPE::JUMP))
	{
		// ジャンプ中は線分を伸ばす
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_JUMP_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_JUMP_END_LOCAL_POS);
		}

		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_JUMP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_JUMP_LOCAL_POS);
			colCapsule->SetRadius(COL_CAPSULE_RADIUS);
		}
	}
	else
	{
		//ここから

		// 通常時の線分に戻す
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::LINE)) != 0)
		{
			ColliderLine* colLine = dynamic_cast<ColliderLine*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::LINE)));
			colLine->SetLocalPosStart(COL_LINE_START_LOCAL_POS);
			colLine->SetLocalPosEnd(COL_LINE_END_LOCAL_POS);
		}
		//通常時のカプセル
		if (ownColliders_.count(static_cast<int>(COLLIDER_TYPE::CAPSULE)) != 0)
		{
			ColliderCapsule* colCapsule = dynamic_cast<ColliderCapsule*>(
				ownColliders_.at(static_cast<int>(COLLIDER_TYPE::CAPSULE)));
			colCapsule->SetLocalPosTop(COL_CAPSULE_TOP_LOCAL_POS);
			colCapsule->SetLocalPosDown(COL_CAPSULE_DOWN_LOCAL_POS);
		
		}
	}
}

void Player::InitState(void)
{
	states_[STATE::IDLE] = new PlayerIdleState();
	currentState_ = states_[STATE::IDLE];
}


