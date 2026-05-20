#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Actor/ActorBase.h"
#include "../Object/Collider/Capsule/ColliderCapsule.h"
#include "../Object/Collider/Line/ColliderLine.h"
#include "../Object/Collider/Model/ColliderModel.h"
#include "../Object/Collider/Sphere/ColliderSphere.h"
#include "../Object/Actor/Stage/Stage.h"
#include "Camera.h"

Camera::Camera(void)
	:
	followTransform_(nullptr),
	mode_(MODE::NONE),
	angles_(AsoUtility::VECTOR_ZERO),
	rotY_(Quaternion::Identity()),
	targetPos_(AsoUtility::VECTOR_ZERO),
	targetPosGoal_(AsoUtility::VECTOR_ZERO),
	posGoal_(AsoUtility::VECTOR_ZERO),
	anglesGoal_(AsoUtility::VECTOR_ZERO),
	ActorBase()

{
	// DxLibの初期設定では、
	// カメラの位置が x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)、
	// 注視点の位置は x = 320.0f, y = 240.0f, z = 1.0f
	// カメラの上方向は x = 0.0f, y = 1.0f, z = 0.0f
	// 右上位置からZ軸のプラス方向を見るようなカメラ

	
		
}

Camera::~Camera(void)
{
}


void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::MOUSE:
		SetBeforeDrawMouse();
		break;
	case Camera::MODE::TARGETING:
		SetBeforeDrawTargeting();
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		transform_.pos,
		targetPos_, 
		transform_.quaRot.GetUp()
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::DrawDebug(void)
{
	DrawSphere3D(debugTargetPos_, 10.0f, 16, 0xff00ff, 0xff00ff, true);	

	DrawFormatString(10, 200, 0xffffff, "camera angles: %f, %f, %f", angles_.x, angles_.y, angles_.z);
}

void Camera::Release(void)
{
	// コライダの解放
	for (auto& pair : ownColliders_)
	{
		if (pair.second != nullptr)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}
	ownColliders_.clear();

	// 追従対象の参照をクリア
	followTransform_ = nullptr;

	// 衝突対象のコライダリストをクリア
	hitColliders_.clear();
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetTargetPos(const VECTOR& pos)
{
	targetPos_ = pos;
}

void Camera::InitCollider(void)
{



	// 主に地面との衝突で使用する球体コライダ
	ColliderSphere* colliderSphere = new ColliderSphere(
		ColliderBase::TAG::CAMERA,
		&transform_,
		AsoUtility::VECTOR_ZERO,
		COL_CAPSULE_SPHERE
	);
	ownColliders_.emplace(
		static_cast<int>(COLLIDER_TYPE::SPHERE), colliderSphere);

}

void Camera::InitPost(void)
{
	ChangeMode(MODE::FIXED_POINT);
}

const VECTOR& Camera::GetPos(void) const
{
	return transform_.pos;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}

const VECTOR& Camera::GetTargetPos(void) const
{
	return targetPos_;
}

const Quaternion& Camera::GetQuaRot(void) const
{
	return transform_.quaRot;
}

const Quaternion& Camera::GetQuaRotY(void) const
{
	return rotY_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, transform_.pos));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::MOUSE:
		// マウスカーソルを画面中央に固定
		SetMouseDispFlag(FALSE);
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	transform_.pos = DERFAULT_POS;

	// カメラ角
	angles_ = DERFAULT_ANGLES;
	transform_.quaRot = Quaternion::Identity();

	// 注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// カメラの上方向
	transform_.quaRot.GetUp();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// Y軸
	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// Y軸 + X軸
	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点
	localPos = transform_.quaRot.PosAxis(FOLLOW_TARGET_LOCAL_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = transform_.quaRot.PosAxis(FOLLOW_CAMERA_LOCAL_POS);
	transform_.pos = VAdd(pos, localPos);

	// カメラの上方向
	transform_.quaRot.GetUp();

}

void Camera::ProcessRot(bool isLimit)
{

	if (GetJoypadNum() == 0)
	{
		// 方向回転によるXYZの移動(キーボード)
		RotKeyboard(isLimit);
	}
	else
	{
		// 方向回転によるXYZの移動(ゲームパッド)
		RotGamePad(isLimit);
	}

}

void Camera::ProcessMove(void)
{

	auto ins = InputManager::GetInstance();

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (GetJoypadNum() == 0)
	{
		if (ins->IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
		if (ins->IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
		if (ins->IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
		if (ins->IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	}
	else
	{
		// アナログキーの入力値から方向を取得
		short leftX, leftY;
		ins->GetLeftStick(0, leftX, leftY);

		// アナログスティックの入力値を正規化して方向ベクトルを作成
		float magnitude = sqrtf(leftX * leftX + leftY * leftY);
		if (magnitude > 0.0f)
		{
			moveDir.x = leftX / magnitude;
			moveDir.z = leftY / magnitude;
		}
	}

	// 移動処理
	if (!AsoUtility::EqualsVZero(moveDir))
	{

		// 移動させたい方向(ベクトル)に変換

		// 現在の向きからの進行方向を取得
		VECTOR direction = VNorm(transform_.quaRot.PosAxis(moveDir));

		// 移動させたい方向に移動量をかける(=移動量)
		VECTOR movePow = VScale(direction, SPEED);

		// カメラ位置も注視点も移動させる
		transform_.pos = VAdd(transform_.pos, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFree(void)
{

	// カメラ操作(回転)
	ProcessRot(false);
	
	// カメラ操作(移動)
	ProcessMove();

	// Y軸
	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// Y軸 + X軸
	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	// 注視点更新
	targetPos_ = VAdd(transform_.pos, transform_.quaRot.PosAxis(FOLLOW_TARGET_LOCAL_POS));

	// カメラの上方向更新
	transform_.quaRot.GetUp();

}

void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作(回転)
	ProcessRot(true);

	// 追従対象との相対位置を同期
	SyncFollow();

	//当たり判定
	Collision();
	
}

void Camera::SetBeforeDrawMouse(void)
{

	// マウスによるカメラ回転
	RotMouse(true);

	// カメラ操作(移動)
	ProcessMove();

	// 追従対象との相対位置を同期
	if (followTransform_ != nullptr)
	{
		SyncFollow();

		//// プレイヤーの位置
		//VECTOR playerPos = followTransform_->pos;

		//// カメラ位置をプレイヤーの後ろに配置
		//VECTOR localPos = transform_.quaRot.PosAxis(MOUSE_CAMERA_LOCAL_POS);
		//transform_.pos = VAdd(playerPos, localPos);

		// 当たり判定
		Collision();
	}
	else
	{
		// 追従対象がない場合はフリーカメラのように動作
		// Y軸
		rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

		// Y軸 + X軸
		transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

		// 注視点更新
		targetPos_ = VAdd(transform_.pos, transform_.quaRot.PosAxis(MOUSE_CAMERA_LOCAL_POS));

		// カメラの上方向更新
		transform_.quaRot.GetUp();
	}

}

void Camera::SetBeforeDrawTargeting(void)
{
	// 注視点はGameSceneで設定済み（敵の位置）
	debugTargetPos_ = targetPos_;

	// プレイヤーの位置
	VECTOR playerPos = followTransform_->pos;

	// プレイヤーから敵への方向ベクトル
	VECTOR toEnemy = VSub(targetPos_, playerPos);

	// Y軸回転を敵の方向に向ける
	float angleYGoal = atan2f(toEnemy.x, toEnemy.z);
	
	// 角度をイージング
	angles_.y = LerpAngle(angles_.y, angleYGoal, EASING_RATE_ANGLE);
	
	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// Y軸 + X軸回転
	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	// カメラ位置の目標を計算
	VECTOR localPos = transform_.quaRot.PosAxis(TARGETING_CAMERA_LOCAL_POS);
	posGoal_ = VAdd(playerPos, localPos);
	
	// カメラ位置をイージング
	transform_.pos = LerpVector(transform_.pos, posGoal_, EASING_RATE_POS);

	// カメラの上方向
	transform_.quaRot.GetUp();

	// 当たり判定
	Collision();
}

void Camera::Collision(void)
{
	// プレイヤーのルートフレーム
	VECTOR start = MV1GetFramePosition(followTransform_->modelId, 1);

	// 登録されている諸突物をすべてチェック
	for (const auto& hitCol : hitColliders_)
	{
		// モデル以外は処理を飛ばす
		if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		// 線分で衝突判定
		auto hits = MV1CollCheck_LineDim(
			colliderModel->GetFollow()->modelId,
			-1,
			transform_.pos,
			start
		);

		// 追従対象に一番近い衝突点を探す
		bool isCollision = false;

		MV1_COLL_RESULT_POLY hitPoly;

		double minDist = DBL_MAX;

		for (int i = 0; i < hits.HitNum; i++)
		{

			const auto& hit = hits.Dim[i];

			//対象のフレームは無視する
			if (!colliderModel->IsTargetFrame(hit.FrameIndex))
			{
				continue;
			}

			// 衝突判定
			isCollision = true;

			// 距離判定
			double dist = AsoUtility::Distance(start, hit.HitPosition);

			if (dist < minDist)
			{
				// 追従対象に一番近い衝突点を優先
				minDist = dist;
				hitPoly = hit;
			}

		
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);

		if (!isCollision)
		{
			// 衝突していなければ次のコライダへ
			continue;
		}

	
		// カメラ位置から注視点への方向
		VECTOR dirToTarget = VNorm(VSub(targetPos_, transform_.pos));

		// 衝突点の少し手前にカメラを置く
		transform_.pos =
			VAdd(hitPoly.HitPosition,
				VScale(dirToTarget, COLLISION_BACK_DIS));
	
#pragma region 球体の衝突で押し戻す
		// カメラ位置の球体コライダ
		int typeSphere = static_cast<int>(COLLIDER_TYPE::SPHERE);
		
		// 球体コライダが無ければ処理を抜ける
		if (ownColliders_.count(typeSphere) == 0) continue;

		//// 球体コライダ情報
		//ColliderSphere* colliderSphere =
		//	dynamic_cast<ColliderSphere*>(ownColliders_.at(typeSphere));
		//if (colliderSphere == nullptr) return;


		// 衝突していたら法線方向に押し戻し
		transform_.pos = ownColliders_.at(typeSphere)->GetPosPushBackAlongNormal
		(hitPoly, CNT_TRY_COLLISION_CAMERA, COLLISION_BACK_DIS);
		
#pragma endregion

	}


}

void Camera::RotKeyboard(bool isLimit)
{

	const auto ins = InputManager::GetInstance();

	// カメラ回転
	if (ins->IsNew(KEY_INPUT_RIGHT))
	{
		// 右回転
		angles_.y += ROT_POW_RAD;
	}
	if (ins->IsNew(KEY_INPUT_LEFT))
	{
		// 左回転
		angles_.y -= ROT_POW_RAD;
	}

	// 上回転
	if (ins->IsNew(KEY_INPUT_UP))
	{
		angles_.x += ROT_POW_RAD;
		if (isLimit && angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}

	// 下回転
	if (ins->IsNew(KEY_INPUT_DOWN))
	{
		angles_.x -= ROT_POW_RAD;
		if (isLimit && angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}

}

void Camera::RotGamePad(bool isLimit)
{

	auto ins = InputManager::GetInstance();

	// アナログキーの入力値から方向を取得
	short rightX, rightY;
	ins->GetRightStick(0, rightX, rightY);

	// アナログスティックの入力値を正規化して方向ベクトルを作成
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	float magnitude = sqrtf(rightX * rightX + rightY * rightY);
	if (magnitude > 0.0f)
	{
		dir.x = rightX / magnitude;
		dir.z = rightY / magnitude;
	}

	// 右スティック左右の傾き
	angles_.y += dir.x * ROT_POW_RAD;

	// 右スティック上下の傾き
	angles_.x += dir.z * ROT_POW_RAD;
	
	// 角度制限
	if (isLimit && angles_.x < -LIMIT_X_DW_RAD)
	{
		angles_.x = -LIMIT_X_DW_RAD;
	}
	if (isLimit && angles_.x > LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}

}

void Camera::RotMouse(bool isLimit)
{

	// マウスの移動量を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 画面中央の座標を取得
	int screenW, screenH;
	GetScreenState(&screenW, &screenH, nullptr);
	int centerX = screenW / 2;
	int centerY = screenH / 2;

	// 画面中央からのマウスの移動量を計算
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// マウスの移動量からカメラの回転角度を計算
	// マウス感度を調整（ROT_POW_RADの半分程度に設定）
	const float MOUSE_SENSITIVITY = 0.002f;
	
	angles_.y += deltaX * MOUSE_SENSITIVITY;
	angles_.x += deltaY * MOUSE_SENSITIVITY;

	// 角度制限
	if (isLimit)
	{
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}


	}

	// マウスカーソルを画面中央に戻す
	SetMousePoint(centerX, centerY);

}


// イージング処理の実装を追加（ファイル末尾に追加）
VECTOR Camera::LerpVector(const VECTOR& current, const VECTOR& target, float rate)
{
	VECTOR result;
	result.x = current.x + (target.x - current.x) * rate;
	result.y = current.y + (target.y - current.y) * rate;
	result.z = current.z + (target.z - current.z) * rate;
	return result;
}

float Camera::LerpAngle(float current, float target, float rate)
{
	// 角度の差分を-π～πの範囲に正規化
	float diff = target - current;
	while (diff > DX_PI_F) diff -= DX_TWO_PI_F;
	while (diff < -DX_PI_F) diff += DX_TWO_PI_F;
	
	return current + diff * rate;
}
