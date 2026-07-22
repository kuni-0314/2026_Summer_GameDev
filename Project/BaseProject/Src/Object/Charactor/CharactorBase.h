#pragma once
#include "../Actor/ActorBase.h"

class AnimationController;
class CharactorBase : public ActorBase
{
public:

	//// 衝突判定種別
	//enum class COLLIDER_TYPE
	//{
	//	LINE,
	//	CAPSULE,
	//	SPHERE,
	//	VIEW_RAGE,
	//	MAX,
	//};

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
	CharactorBase();
	// デストラクタ
	virtual ~CharactorBase() override;

	//更新
	virtual void Update() override;

	virtual void Draw() override;

	virtual void Release() override;

	// ダメージ処理
	virtual void Damage(int damage);

	virtual void Damage(int damage, const VECTOR& hitDir);

	// HPの取得
	int GetHp() const { return hp_; }

	void SetAlive(bool alive) { isAlive_ = alive; }

	bool IsAnimEnd();

	virtual void HealHp(int heal) { hp_ += heal; };

	bool IsInvincible() const { return isInvincible_; }
	void SetInvincible(bool invincible);

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

	// ノックバック量
	VECTOR knockbackPow_;

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

	//空中に浮く用
	bool useGrabity_ = true;

	// リソースロード
	virtual void InitLoad() override;

	virtual void InitAnimation() override;

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

	AnimationController* animationController_;

	int GetInvincibleFrameCount() const { return invincibleFrameCount_; }

	// 体重
	enum class WEIGHT
	{
		NONE,		// 体重なし
		LIGHT,		// 軽い
		NORMAL,		// 普通
		HEAVY,		// 重い
		IMMOBILE	// 不動
	};
	WEIGHT weight_;
	// 無敵フレーム数
	static constexpr int INVINCIBLE_FRAME_COUNT = 30;
	int invincibleFrameCount_ = 0;
	bool isInvincible_;
private:

	int stage_; //影の判定用ステージハンドル

	float PLAYER_SHADOW_HEIGHT = 800.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;

	VECTOR SlideVec;//影のベクトル
	VERTEX3D Vertex[3];
	

};

