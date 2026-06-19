
#include "../../Common/Transform.h"
#include "ColliderCapsule.h"
#include "../Model/ColliderModel.h"
ColliderCapsule::ColliderCapsule(
	TAG tag, const Transform * follow,
	const VECTOR & localPosTop, const VECTOR & localPosDown, float radius)
	:
	ColliderBase(SHAPE::CAPSULE, tag, follow),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown),
	radius_(radius)
{
}
ColliderCapsule::~ColliderCapsule()
{
}
const VECTOR& ColliderCapsule::GetLocalPosTop() const
{
	return localPosTop_;
}
const VECTOR& ColliderCapsule::GetLocalPosDown() const
{
	return localPosDown_;
}
void ColliderCapsule::SetLocalPosTop(const VECTOR& pos)
{
	localPosTop_ = pos;
}
void ColliderCapsule::SetLocalPosDown(const VECTOR& pos)
{
	localPosDown_ = pos;
}
VECTOR ColliderCapsule::GetPosTop() const
{
	return GetRotPos(localPosTop_);
}
VECTOR ColliderCapsule::GetPosDown() const
{
	return GetRotPos(localPosDown_);
}
float ColliderCapsule::GetRadius() const
{
	return radius_;
}
void ColliderCapsule::SetRadius(float radius)
{
	radius_ = radius;
}
float ColliderCapsule::GetHeight() const
{
	return localPosTop_.y;
}
VECTOR ColliderCapsule::GetCenter() const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();
	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}



void ColliderCapsule::DrawDebug(int color)
{
	// 非常に意味が無い
	auto a = follow_->pos;
	auto b = follow_->quaRot;

	// 上の球体
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, color, color, false);
	DrawSphere3D(pos1, radius_ * 0.125f, 5, color, color, true);
	// 下の球体
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, color, color, false);
	DrawSphere3D(pos2, radius_ * 0.125f, 5, color, color, true);
	VECTOR dir;
	VECTOR s;
	VECTOR e;
	// 球体を繋ぐ線(X+)
	dir = follow_->GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);
	// 球体を繋ぐ線(X-)
	dir = follow_->GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);
	// 球体を繋ぐ線(Z+)
	dir = follow_->GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);
	// 球体を繋ぐ線(Z-)
	dir = follow_->GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);
	// カプセルの中心
	//DrawSphere3D(GetCenter(), 5.0f, 10, color, color, true);

	// 座標を表示（１要素ずつ改行）
	if (!isDebugDraw_)
	{
		return;
	}
	DrawFormatString(0, 200, color, "Capsule Pos Top X : %.2f", pos1.x);
	DrawFormatString(0, 220, color, "Capsule Pos Top Y : %.2f", pos1.y);
	DrawFormatString(0, 240, color, "Capsule Pos Top Z : %.2f", pos1.z);
	DrawFormatString(0, 260, color, "Capsule Pos Down X : %.2f", pos2.x);
	DrawFormatString(0, 280, color, "Capsule Pos Down Y : %.2f", pos2.y);
	DrawFormatString(0, 300, color, "Capsule Pos Down Z : %.2f", pos2.z);
}

VECTOR ColliderCapsule::GetPosPushBackAlongNormal(
	const MV1_COLL_RESULT_POLY& hitColPoly,
	int maxTryCnt, float pushDistance) const
{
	// コピー生成
	Transform tmpTransform = *follow_;
	ColliderCapsule tmpCapsule = *this;
	tmpCapsule.SetFollow(&tmpTransform);
	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < maxTryCnt)
	{
		// カプセルと三角形の当たり判定
		if (!HitCheck_Capsule_Triangle(
			tmpCapsule.GetPosTop(), tmpCapsule.GetPosDown(),
			tmpCapsule.GetRadius(),
			hitColPoly.Position[0], hitColPoly.Position[1],
			hitColPoly.Position[2]))
		{
			break;
		}
		// 衝突していたら法線方向に押し戻し
		tmpTransform.pos =
		VAdd(tmpTransform.pos, VScale(hitColPoly.Normal, pushDistance));
		tryCnt++;
	}
	return tmpTransform.pos;
}

void ColliderCapsule::PushBackAlongNormal(
	const ColliderModel* colliderModel, Transform& transform,
	int maxTryCnt, float pushDistance, bool isExclude, bool isTarget) const
{
	// モデルとカプセルの衝突判定
	auto hits = MV1CollCheck_Capsule(
		colliderModel->GetFollow()->modelId, -1,
		GetPosTop(), GetPosDown(), GetRadius());
	// 衝突した複数のポリゴンと衝突回避するまで、位置を移動させる
	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hitPoly = hits.Dim[i];
		// 除外フレームは無視する
		if (isExclude && colliderModel->IsExcludeFrame(hitPoly.FrameIndex))
		{
			continue;
		}

		// 対象フレーム以外は無視する
		if (isTarget && !colliderModel->IsTargetFrame(hitPoly.FrameIndex))
		{
			continue;
		}
		// 指定された回数と距離で三角形の法線方向に押し戻す
		transform.pos =
			GetPosPushBackAlongNormal(hitPoly, maxTryCnt, pushDistance);
	}
	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);
}

bool ColliderCapsule::IsHit(const ColliderModel* colliderModel, bool isExclude, bool isTarget) const
{


	bool ret = false;

	// モデルとカプセルの衝突判定
	auto hits = MV1CollCheck_Capsule(
		colliderModel->GetFollow()->modelId, -1,
		GetPosTop(), GetPosDown(), GetRadius());

	// 衝突した複数のポリゴンと衝突回避するまで、位置を移動させる
	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hitPoly = hits.Dim[i];
		// 除外フレームは無視する
		if (isExclude && colliderModel->IsExcludeFrame(hitPoly.FrameIndex))
		{
			continue;
		}

		// 対象フレーム以外は無視する
		if (isTarget && !colliderModel->IsTargetFrame(hitPoly.FrameIndex))
		{
			continue;
		}

		ret = true;
		break;
	}
	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return ret;
}

bool ColliderCapsule::IsHit(const ColliderCapsule* colliderCapsule, bool isExclude, bool isTarget) const
{
	return HitCheck_Capsule_Capsule(
		GetPosTop(), GetPosDown(), GetRadius(),
		colliderCapsule->GetPosTop(), colliderCapsule->GetPosDown(), colliderCapsule->GetRadius());
}
