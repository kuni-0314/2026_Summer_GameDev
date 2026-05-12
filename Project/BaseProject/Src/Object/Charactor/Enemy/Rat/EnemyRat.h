#pragma once
#include <DxLib.h>
#include <functional>
#include "../../Player/Player.h"
#include "../EnemyBase.h"
class Player;
class AttackBase;
class AttackRat;


class EnemyRat: public EnemyBase
{
public:
	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		ATTACK,
		END
	};

	// 状態
	enum class STATE
	{
		NONE,
		THINK,
		IDLE,
		WANDER,
		ATTACK,
		CHASE,
		END
	};

	// コンストラクタ
	EnemyRat(const EnemyBase::EnemyData& data,Player* player);
	// デストラクタ
	~EnemyRat(void) override;
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
private:
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
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;
	// 追跡開始距離
	static constexpr float CHASE_DISTANCE = 500.0f;     
	static constexpr float ATTACK_DISTANCE = 150.0f;     // 攻撃範囲
	static constexpr float CHASE_SPEED = 5.0f;           // 追跡速度
	static constexpr float ATTACK_COOLDOWN = 2.0f;       // 攻撃クールタイム

	// 状態
	STATE state_;

	Player* player_;

	//攻撃
	AttackBase* attackBase_;
	AttackRat* attackRat_;


	// 状態管理(状態遷移時初期処理)
	/*std::map<int, std::function<void(void)>> stateChanges_;*/

	// 更新ステップ
	float step_;// 状態管理(更新ステップ)
	
	float attackCooldown_;  // 攻撃クールタイム管理

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateWander(void);
	void ChangeStateAttack(void);
	void ChangeStateChaseRat(void);
	void ChangeStateEnd(void);

	// 更新系
	void UpdateNone(void);
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateWander(void);
	void UpdateAttack(void);
	void UpdateChaseRat(void);
	void UpdateEnd(void);

	// ユーティリティ関数
	bool IsPlayerInAttackRange(void) const;
	bool IsPlayerInChaseRange(void) const;
	void FacePlayer(void);
};

