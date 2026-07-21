#include <cmath>
#include "CharactorBase.h"
#include "../../Object/Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Collider/Line/ColliderLine.h"
#include "../../Object/Collider/Model/ColliderModel.h"
#include "../../Object/Collider/Capsule/ColliderCapsule.h"
#include "../../Manager/ResourceManager.h"

CharactorBase::CharactorBase()
	:
	ActorBase(),
	animationController_(nullptr),
	moveDir_(AsoUtility::VECTOR_ZERO),
	moveSpeed_(0.0f),
	movePow_(AsoUtility::VECTOR_ZERO),
	hp_(0)
{
}
CharactorBase::~CharactorBase()
{
}

void CharactorBase::InitLoad()
{
	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	stage_ = resMng_.Load(ResourceManager::SRC::MAIN_STAGE).handleId_;

}

void CharactorBase::InitAnimation()
{
	animationController_ = new AnimationController(transform_.modelId);
}

void CharactorBase::Update()
{
	// 移動前座標を更新
	prevPos_ = transform_.pos;

	// 無敵フレームのカウントダウン
	if (invincibleFrameCount_ > 0)
	{
		invincibleFrameCount_--;
	}

	if (isInvincible_ <= 0)
	{
		isInvincible_ = false;
	}
	else
	{
		isInvincible_ = true;
	}

	movePow_ = knockbackPow_;
	knockbackPow_ = VScale(knockbackPow_, 0.99f);

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

}

void CharactorBase::Draw()
{
	//基底クラスの描画処理
	ActorBase::Draw();
	// 丸影の描画
	DrawShadow();

}

void CharactorBase::Release()
{
	//アニメーションコントローラー解放
	if (animationController_ != nullptr)
	{
		animationController_->Release();
		delete animationController_;
	}
	//基底クラスの開放
	ActorBase::Release();
}

void CharactorBase::Damage(int damage)
{
	if (isInvincible_) return;

	hp_ -= damage;
	if (hp_ <= 0)
	{
		hp_ = 0;
		
		return;
	}

	// 原則として無敵フレームを設定する
	invincibleFrameCount_ = INVINCIBLE_FRAME_COUNT;
}

void CharactorBase::Damage(int damage, const VECTOR& hitDir)
{
	if (isInvincible_) return;

	float knowbackPow = 0.0f;
	switch (weight_)
	{
	case CharactorBase::WEIGHT::NONE:
		// めっちゃ吹っ飛ばす
		knowbackPow = 100.0f;
		break;
	case CharactorBase::WEIGHT::LIGHT:
		// よく吹っ飛ぶ
		knowbackPow = 10.0f;
		break;
	case CharactorBase::WEIGHT::NORMAL:
		// 飛ぶっちゃ飛ぶ
		knowbackPow = 4.0f;
		break;
	case CharactorBase::WEIGHT::HEAVY:
		// 若干飛ぶ
		knowbackPow = 1.0f;
		break;
	case CharactorBase::WEIGHT::IMMOBILE:
		// 飛ばねぇ
		knowbackPow = 0.0f;
		break;
	default:
		break;
	}
	
	auto dir = VNorm(hitDir);
	knockbackPow_ = VScale(dir, knowbackPow);

	hp_ -= damage;
	if (hp_ <= 0)
	{
		hp_ = 0;

		return;
	}

	// 原則として無敵フレームを設定する
	invincibleFrameCount_ = INVINCIBLE_FRAME_COUNT;
}

bool CharactorBase::IsAnimEnd()
{
	return animationController_->IsEnd();
}

void CharactorBase::SetInvincible(bool invincible)
{
	if (!invincible) invincibleFrameCount_ = 0;	// 強制的に無敵解除
	isInvincible_ = invincible;
}

void CharactorBase::DelayRotate()
{
	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(moveDir_);
	// 回転の補間
	transform_.quaRot =
		Quaternion::Slerp(transform_.quaRot, goalRot, 0.2f);
}

void CharactorBase::CalcGravityPow()
{
	if (!useGrabity_)
	{
		jumpPow_ = AsoUtility::VECTOR_ZERO;
		return;
	}

	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;
	// 重力の強さ
	float gravityPow = Application::GetInstance().GetGravityPow() * 0.01f/*scnMng_.GetDeltaTime()*/;
	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 重力速度の制限
	if (jumpPow_.y < MAX_FALL_SPEED)
	{
		jumpPow_.y = MAX_FALL_SPEED;
	}
}



void CharactorBase::Collision()
{
	// 衝突(カプセル)
	CollisionCapsule();
	
	if (useGrabity_)
	{
		// ジャンプ量を加算
		transform_.pos = VAdd(transform_.pos, jumpPow_);
		// 衝突(重力)
		CollisionGravity();
	}
}

void CharactorBase::CollisionGravity()
{
	// 線分コライダ
	int lineType = static_cast<int>(COLLIDER_TYPE::LINE);
	// 線分コライダが無ければ処理を抜ける
	if (ownColliders_.count(lineType) == 0) return;
	// 線分コライダ情報
	ColliderLine* colliderLine_ = dynamic_cast<ColliderLine*>(ownColliders_.at(lineType));

	if (colliderLine_ == nullptr) return;
	

	// 登録されている衝突物を全てチェック
	for (const auto& hitCol : hitColliders_)
	{
		// ステージ以外は処理を飛ばす
		if (hitCol->GetTag() != ColliderBase::TAG::STAGE) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);
		if (colliderModel == nullptr) continue;

		bool isHit_ = colliderLine_->PushBackUp(colliderModel, transform_, 2.0f,
			true, false);

		// ジャンプ判定
		if (isHit_)
		{
			isJump_ = false;
			isAir_ = false;
		}
		else
		{

			isAir_ = true;
		}
		
	}
	if (!isJump_ && !isAir_)
	{
		// ジャンプリセット
		jumpPow_ = AsoUtility::VECTOR_ZERO;
		// ジャンプの入力受付時間をリセット
		stepJump_ = 0.0f;
	}

}

void CharactorBase::CollisionCapsule()
{
	// カプセルコライダ
	int capsuleType = static_cast<int>(COLLIDER_TYPE::CAPSULE);
	// カプセルコライダが無ければ処理を抜ける
	if (ownColliders_.count(capsuleType) == 0) return;
	// カプセルコライダ情報
	ColliderCapsule* colliderCapsule =
		dynamic_cast<ColliderCapsule*>(ownColliders_.at(capsuleType));
	if (colliderCapsule == nullptr) return;

	// 移動量がカプセルの半径より大きい場合、移動量を分割して衝突判定を行う
	float moveLen = VSize(movePow_);
	float capRadius = colliderCapsule->GetRadius();
	
	// 移動開始位置を保存
	VECTOR startPos = transform_.pos;
	
	if (moveLen > capRadius)
	{
		// 分割数を計算（最低でも1回は分割）
		int splits = static_cast<int>(std::ceil(moveLen / capRadius));
		VECTOR currentPos = startPos;

		// 分割された各ステップで衝突判定
		for (int i = 1; i <= splits; i++)
		{
			// 前の位置を保存
			VECTOR prevStepPos = currentPos;
			
			// 次の目標位置を計算
			float t = static_cast<float>(i) / static_cast<float>(splits);
			VECTOR targetPos = VAdd(startPos, VScale(movePow_, t));
			
			// 一時的なTransformを設定
			Transform tempTrans = transform_;
			tempTrans.prevPos = prevStepPos;
			tempTrans.pos = targetPos;

			// 全ての衝突物に対してチェック
			for (const auto& hitCol : hitColliders_)
			{
				if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;
				const ColliderModel* colliderModel =
					dynamic_cast<const ColliderModel*>(hitCol);
				if (colliderModel == nullptr) continue;

				colliderCapsule->PushBackAlongNormal(colliderModel, tempTrans, CNT_TRY_COLLISION,
					COLLISION_BACK_DIS, true, false);
			}
			
			// 修正された位置を次のステップに引き継ぐ
			currentPos = tempTrans.pos;
		}
		
		// 最終的な位置を反映
		transform_.pos = currentPos;
	}
	else
	{
		// 移動後の位置を設定
		transform_.prevPos = startPos;
		transform_.pos = VAdd(startPos, movePow_);
		
		// 登録されている衝突物を全てチェック
		for (const auto& hitCol : hitColliders_)
		{
			// モデル以外は処理を飛ばす
			if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;
			// 派生クラスへキャスト
			const ColliderModel* colliderModel =
				dynamic_cast<const ColliderModel*>(hitCol);
			if (colliderModel == nullptr) continue;

			colliderCapsule->PushBackAlongNormal(colliderModel, transform_, CNT_TRY_COLLISION,
				COLLISION_BACK_DIS, true, false);
		}
	}
}

void CharactorBase::DrawShadow()
{

	int i, j;


	// ライティングを無効にする
	SetUseLighting(false);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(true);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResDim = MV1CollCheck_Capsule(stage_, -1,transform_.pos, VAdd(transform_.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitRes = HitResDim.Dim;
	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitRes->Position[0];
		Vertex[1].pos = HitRes->Position[1];
		Vertex[2].pos = HitRes->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitRes->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
			Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

		if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
			Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

		if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
			Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, imgShadow_, true);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResDim);

	// ライティングを有効にする
	SetUseLighting(true);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(false);
}


