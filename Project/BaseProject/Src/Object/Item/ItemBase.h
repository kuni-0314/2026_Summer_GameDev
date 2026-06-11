#pragma once
#include "../Actor/ActorBase.h"

class AnimationController;

class ItemBase : public ActorBase
{
public:


	//アイテム種別
	enum class TYPE
	{
		HP,
		SKILL,
	};


	// コンストラクタ
	ItemBase();
	// デストラクタ
	virtual ~ItemBase() override;

	//更新
	virtual void Update() override;

	virtual void Draw() override;

	virtual void Release() override;

	void SetPos(const VECTOR& pos) { transform_.pos = pos; };

	// プレイヤーのコライダを渡して判定を行う
	bool InSearchModel();

protected:


	// 最大落下速度
	static constexpr float MAX_FALL_SPEED = -30.0f;
	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION = 20;
	// 衝突時の押し戻し量
	static constexpr float COLLISION_BACK_DIS = 1.0f;

	//移動量
	VECTOR movePow_;
	// 移動方向
	VECTOR moveDir_;
	// ジャンプ量
	VECTOR jumpPow_;
	// 移動前の座標
	VECTOR prevPos_;


	bool isPlayer_;

	// ジャンプの入力受付時間
	float stepJump_;
	//移動スピード
	float moveSpeed_;


	//モデルID
	int modelId_;

	// 丸影画像
	int imgShadow_;

	// リソースロード
	virtual void InitLoad() override;

	// 更新系
	virtual void UpdateProcess() = 0;
	virtual void UpdateProcessPost() = 0;

	// 移動方向に応じた遅延回転
	void DelayRotate();

	// 重力計算
	void CalcGravityPow();


	// 衝突判定
	virtual void  CollisionReserve() {}
	void Collision();
	void CollisionGravity();
	//カプセル型当たり判定（flag:trueなら高精度処理を行う）
	void CollisionCapsule();

	// 丸影描画
	void DrawShadow();



private:


	int stage_; //影の判定用ステージハンドル

	float ITEM_SHADOW_HEIGHT = 800.0f;
	float ITEM_SHADOW_SIZE = 30.0f;

	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;

	VECTOR SlideVec;//影のベクトル
	VERTEX3D Vertex[3];

};