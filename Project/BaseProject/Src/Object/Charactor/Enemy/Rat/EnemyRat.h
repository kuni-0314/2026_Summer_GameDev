#pragma once
#include <DxLib.h>
#include <functional>
#include "../EnemyBase.h"
class Player;
class ItemManger;


class EnemyRat : public EnemyBase
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
		CHARGE,
		ATTACK,
		HIT,
		DIE,
		END
	};

	// コンストラクタ
	EnemyRat(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// デストラクタ
	~EnemyRat() override;

	void Draw() override;
protected:
	// リソースロード
	void InitLoad() override;
	// 大きさ、回転、座標の初期化
	void InitTransform() override;
	// 衝突判定の初期化
	void InitCollider() override;
	// アニメーションの初期化
	void InitAnimation() override;
	// 初期化後の個別処理
	void InitPost() override;
	// 更新系
	void UpdateProcess() override;
	void UpdateProcessPost() override;


private:


	
	ItemManger* itemManager_ ;

	//アニメーション登録番号
	static constexpr int  ANIM_INDX_IDLE = 8; 
	static constexpr int  ANIM_INDX_WALK = 13;
	static constexpr int  ANIM_INDX_ATTACK = 1;
	static constexpr int  ANIM_INDX_END = 6;
	static constexpr int  ANIM_INDX_HIT = 7;
	static constexpr int  ANIM_INDX_RUN = 11;


	// モデルの大きさ
	static constexpr float SCALE = 0.5f;
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 70.0f, 0.0f };
	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 70.0f;
	// 衝突判定用カプセル球体半径
	static constexpr float COL_ATTACK_RADIUS = 50.0f;

	// 攻撃判定用球体
	static constexpr VECTOR ATTACK_SPHERE_LOCAL_POS = { 0.0f, 30.0f, 120.0f };
	// 行動切り替え用カプセル球体半径
	static constexpr float COL_SWICH_RADIUS = 250.0f;

	//距離関連
	static constexpr float SEARCH_DISTANCE = 500.0f;
	static constexpr float SWICH_DISTANCE = 200.0f;
	static constexpr float MIN_DISTANCE = 150.0f;
	static constexpr float ATTACK_MOVE_SPEED = 3.0f;

	static constexpr int ATTACK_CHARGE = 20;

	int attackCharge_ = 20;

	//攻撃判定
	bool isAttack_; 

	//プレイヤー視認判定
	//連続攻撃判定
	bool attackHit_ = false;;

	// 状態
	STATE state_;

	VECTOR attackWorldPos_;
	//プレイヤー方向
	VECTOR toPlayer_;
	//プレイヤー座標
	VECTOR playerPos_;




	// 更新ステップ
	float step_;// 状態管理(更新ステップ)

	float distance_;


	// 状態遷移
	void ChangeState(STATE state);

	void ChangeStateNone(void);
	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateAttack(void);
	void ChangeStateCharge(void);
	void ChangeStateHit(void);
	void ChangeStateDie(void);
	void ChangeStateEnd(void);
	void ChangeStateRun(void);
	

	// 更新系
	void UpdateNone(void);
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateCharge(void);
	void UpdateAttack(void);
	void UpdateHit(void);
	void UpdateDie(void);
	void UpdateEnd(void);
	void UpdateRun(void);


};

