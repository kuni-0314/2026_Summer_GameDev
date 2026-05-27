#pragma once
#include "../Actor/ActorBase.h"

class AnimationController;
class CharactorBase : public ActorBase
{
public:


	// ステータス
	struct Status
	{
		int level;		// レベル
		int hp;			// 体力
		int mp;			// 魔力
		int physAtk;	// 物理攻撃力
		int physDef;	// 物理防御力
		int magicAtk;	// 魔法攻撃力
		int magicDef;	// 魔法防御力
		int wisdom;		// 賢さ
		int luck;		// 運
	};

	// コンストラクタ
	CharactorBase(void);
	// デストラクタ
	virtual ~CharactorBase(void) override;

	//更新
	virtual void Update(void) override;

	virtual void Draw(void) override;

	virtual void Release(void) override;

	// ダメージ処理
	void Damege(int damege);

	// HPの取得
	int GetHp() const { return hp_; }

	void SetAlive(bool alive) { isAlive_ = alive; }

	bool IsAnimEnd();

	

protected:


	// 最大落下速度
	static constexpr float MAX_FALL_SPEED = -15.0f;
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

	// ジャンプの入力受付時間
	float stepJump_;
	//移動スピード
	float moveSpeed_;

	// ジャンプ判定
	bool isJump_;

	// 空中判定
	bool isAir_;

	// 丸影画像
	int imgShadow_;

	//体力
	int hp_;

	// リソースロード
	virtual void InitLoad(void) override;

	virtual void InitAnimation(void) override;

	// 更新系
	virtual void UpdateProcess(void) = 0;
	virtual void UpdateProcessPost(void) = 0;
	// 移動方向に応じた遅延回転
	void DelayRotate(void);

	// 重力計算
	void CalcGravityPow(void);


	// 衝突判定
	virtual void  CollisionReserve(void) {}
	void Collision(void);
	void CollisionGravity(void);
	//カプセル型当たり判定（flag:trueなら高精度処理を行う）
	void CollisionCapsule(void);


	// 丸影描画
	void DrawShadow(void);

	AnimationController* animationController_;

private:

	int stage_; //影の判定用ステージハンドル

	//ここから
	float PLAYER_SHADOW_HEIGHT = 800.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;

	VECTOR SlideVec;//影のベクトル
	VERTEX3D Vertex[3];
	
};

