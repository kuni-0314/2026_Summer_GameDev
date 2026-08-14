#pragma once
#include <DxLib.h>
#include "../ColliderBase.h"
class Transform;
class ColliderModel;
class ColliderSphere;
class ColliderCapsule : public ColliderBase
{
public:
	// コンストラクタ
	//posTopとposDownでカプセルコライダーのサイズ設定(定数推奨）
	ColliderCapsule(
		TAG tag, const Transform* follow,
		const VECTOR& localPosTop, const VECTOR& localPosDown, float radius);
	// デストラクタ
	~ColliderCapsule();
	// 親Transformからの相対位置を取得
	const VECTOR& GetLocalPosTop() const;
	const VECTOR & GetLocalPosDown() const;
	// 親Transformからの相対位置をセット
	void SetLocalPosTop(const VECTOR& pos);
	void SetLocalPosDown(const VECTOR& pos);
	// ワールド座標を取得
	VECTOR GetPosTop() const;
	VECTOR GetPosDown() const;
	// 半径
	float GetRadius() const;
	void SetRadius(float radius);
	// 高さ
	float GetHeight() const;
	// カプセルの中心座標
	VECTOR GetCenter() const;

	VECTOR GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitColPoly,
		int maxTryCnt,
		float pushDistance) const override;

	// 指定された回数と距離で三角形の法線方向に押し戻す
	void PushBackAlongNormal(
		const ColliderModel* colliderModel, Transform& transform,
		int maxTryCnt, float pushDistance,
		bool isExclude = false, bool isTarget = false) const;

	//衝突判定の判定渡し（モデル用）
	bool IsHit(const ColliderModel* colliderModel, 
		bool isExclude = false, bool isTarget = false) const;

	// 衝突判定の判定渡し（カプセル用）
	bool IsHit(const ColliderCapsule* colliderCapsule,
		bool isExclude = false, bool isTarget = false) const;

	// 衝突判定の判定渡し（球用）
	bool IsHit(const ColliderSphere* colliderSphere,
		bool isExclude = false, bool isTarget = false) const;

	// デバッグ用描画のON/OFF
	void SetDebugDraw(bool isDebugDraw);

	

	//複数コライダ区別用
	bool wasHit_ = false;

	void  SetWasHit(bool washit);
	
	bool GetwasHit() const;
	

protected:
	// デバッグ用描画
	void DrawDebug(int color) override;
private:
	// 親Transformからの相対位置(上側)
	VECTOR localPosTop_;
	// 親Transformからの相対位置(下側)
	VECTOR localPosDown_;
	// 半径
	float radius_;

	// デバッグ用
	bool isDebugDraw_ = true;


};
