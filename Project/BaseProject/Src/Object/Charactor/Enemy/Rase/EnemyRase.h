
#pragma once
#include <DxLib.h>
#include <functional>
#include "../EnemyBase.h"
class Player;
class ItemManger;


class EnemyRase : public EnemyBase
{
public:
	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		END,
		HIT
	};

	// 状態
	enum class STATE
	{
		NONE,
		THINK,
		IDLE,
		RUN,
		WANDER,
		ATTACK,
		HIT,
		DIE,
		WARNIG,
		END
	};

	// コンストラクタ
	EnemyRase(const EnemyBase::EnemyData& data, Player* player);
	// デストラクタ
	~EnemyRase(void) override;

	void Draw(void) override;
protected:
	// リソースロード
	void InitLoad(void) override;
	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;
	// 衝突判定の初期化
	void InitCollider(void) override;
	// アニメーションの初期化
	void InitAnimation(void) override;
	// 初期化後の個別処理
	void InitPost(void) override;
	// 更新系
	void UpdateProcess(void) override;
	void UpdateProcessPost(void) override;

	void 

private:



	ItemManger* itemManager_;

	//アニメーション登録番号
	//待機
	static constexpr int  ANIM_INDX_IDLE = 2;
	//歩く
	static constexpr int  ANIM_INDX_WALK = 13;



	// モデルの大きさ
	static constexpr float SCALE = 0.3f;
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// 衝突判定用カプセル球体半径
	static constexpr float COL_SPHERE_RADIUS = 50.0f;

	// 攻撃判定用球体
	static constexpr VECTOR ATTACK_SPHERE_LOCAL_POS = { 0.0f, 30.0f, 120.0f };

	// 行動切り替え用カプセル球体半径
	static constexpr float COL_SWICH_RADIUS = 250.0f;

	// RUN切り替え距離
	static constexpr float RUN_SWICH_DISTANCE = 400.0f;

	static constexpr float ATTACK_MOVE_SPEED = 3.0f;

	//攻撃判定
	bool isAttack_;
	//生存判定
	bool isAlive_ = true;
	//プレイヤー視認判定
	bool look_ = false;
	//連続攻撃判定
	bool attackHit_ = false;;

	// 状態
	STATE state_;

	VECTOR worldPos;
	//プレイヤー方向
	VECTOR toPlayer_;
	//プレイヤー座標
	VECTOR playerPos_;
	//プレイヤー判定球の半径
	float playerRad_;



	// 更新ステップ
	float step_;// 状態管理(更新ステップ)

	float distance_;


	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateWander(void);
	void ChangeStateAttack(void);
	void ChangeStateHit(void);
	void ChangeStateDie(void);
	void ChangeStateEnd(void);
	void ChangeStateRun(void);
	void ChangeStateWarnig(void);

	// 更新系
	void UpdateNone(void);
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateWander(void);
	void UpdateAttack(void);
	void UpdateHit(void);
	void UpdateDie(void);
	void UpdateEnd(void);
	void UpdateRun(void);
	void UpdateWarnig(void);



};




