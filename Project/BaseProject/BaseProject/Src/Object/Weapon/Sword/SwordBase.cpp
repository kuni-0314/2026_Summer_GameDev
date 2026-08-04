#include "SwordBase.h"
#include "../../Common/Transform.h"
#include "../../../Object/Collider/Capsule/ColliderCapsule.h"

SwordBase::SwordBase(SWORD_TYPE type, VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform)
	: TYPE(type)
	, LOCAL_POS_START(startPos)
	, LOCAL_POS_END(endPos)
	, RADIUS(radius)
	, ownerTransform_(ownerTransform)
{
}

void SwordBase::InitCollider()
{
	// 攻撃判定の初期化
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER_SWORD, &transform_,
				LOCAL_POS_START, LOCAL_POS_END, RADIUS);
	colCapsule->SetDebugDraw(true);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);
}

void SwordBase::Update()
{
	

	// Transform の更新
	transform_.Update();
}

void SwordBase::Draw()
{
	//ActorBase::Draw();
	if (transform_.modelId != -1)
	{
		MV1DrawModel(transform_.modelId);
	}

#ifdef _DEBUG



	// 所有しているコライダの描画
	for (const auto& own : ownColliders_)
	{
		own.second->Draw();
	}

	// 非常に意味が無い
	transform_.pos = transform_.pos;
	transform_.quaRot = transform_.quaRot;

	// デバッグ用に攻撃判定位置の描画（ワールド座標に変換）
	// スケールを除外して回転と位置のみ適用
	MATRIX matRot = transform_.quaRot.ToMatrix();
	MATRIX matPos = MGetTranslate(transform_.pos);

	MATRIX weaponWorldMat = MMult(matRot, matPos);
	VECTOR worldStart = VTransform(LOCAL_POS_START, weaponWorldMat);
	VECTOR worldEnd = VTransform(LOCAL_POS_END, weaponWorldMat);

	VECTOR a = transform_.quaRot.PosAxis(LOCAL_POS_START);

	VECTOR b = VTransform(
		LOCAL_POS_START,
		transform_.quaRot.ToMatrix()
	);

	DrawFormatString(0, 500, 0xffffff,
		"A %.3f %.3f %.3f",
		a.x, a.y, a.z);

	DrawFormatString(0, 520, 0xffffff,
		"B %.3f %.3f %.3f",
		b.x, b.y, b.z);

	DrawSphere3D(worldStart, 5.0f, RADIUS, 0xff0000, 0xffffff, false);
	DrawSphere3D(worldEnd, 5.0f, RADIUS, 0x0000ff, 0xffffff, true);
	//DrawCapsule3D(worldStart, worldEnd, RADIUS, 8, 0x00ff00, 0x00ff00, false);

	// 座標を表示
	DrawFormatString(0, 320, 0xffffff, "Start X : %.2f", worldStart.x);
	DrawFormatString(0, 340, 0xffffff, "Start Y : %.2f", worldStart.y);
	DrawFormatString(0, 360, 0xffffff, "Start Z : %.2f", worldStart.z);
	DrawFormatString(0, 380, 0xffffff, "End X : %.2f", worldEnd.x);
	DrawFormatString(0, 400, 0xffffff, "End Y : %.2f", worldEnd.y);
	DrawFormatString(0, 420, 0xffffff, "End Z : %.2f", worldEnd.z);
#endif // _DEBUG
}

void SwordBase::BeginAttack(int attackType)
{
}

void SwordBase::EndAttack()
{
}

