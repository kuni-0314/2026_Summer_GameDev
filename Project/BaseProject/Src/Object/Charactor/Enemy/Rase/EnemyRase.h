#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include "../../../../Effect/LoadEffekseer/EffekseerEffect.h"
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
		ATTACK,
		CHARGE,
		END,
		DIE,
		HIT
	};

	// 状態
	enum class STATE
	{
		NONE,
		THINK,
		IDLE,
		MOVE,
		ATTACK,
		WAIT,
		CHARGE,
		HIT,
		DIE,
		END
	};

	//弾のパラメーター(
	struct SHOT
	{
		bool isAlive_ = false;
		float speed = 4.5f;
		int life = 180;
		float homingPower = 0.06f;
		VECTOR dir_;
		Transform shotTransform_;
		std::shared_ptr<EffekseerEffect> effect;
	
	};
	// コンストラクタ

	EnemyRase(const EnemyBase::EnemyData& data, int attackModel,Player* player);
	// デストラクタ
	~EnemyRase() override;

	void Draw(void) override;

	void Release(void) override;
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

	ItemManger* itemManager_;

	std::vector<SHOT> shots_;



	//アニメーション登録番号
	//待機
	static constexpr int  ANIM_INDX_FRY = 2;
	//HIT
	static constexpr int  ANIM_INDX_HIT = 4;
	//END
	static constexpr int  ANIM_INDX_DEAD = 0;
	//チャージ
	static constexpr int  ANIM_INDX_CHARGE = 5;
	
	//弾待機カウント
	static constexpr int  SHOT_CHARGE_COUNT = 30;//120;

	// モデルの大きさ
	static constexpr float SCALE = 0.3f;

	// モデルの大きさ
	static constexpr float SHOT_SCALE = 0.1f;
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

	// 攻撃切り替え距離
	static constexpr float SWICH_DISTANCE = 350.0f;

	static constexpr float ATTACK_MOVE_SPEED = 3.0f;

	//揺れ幅
	const float HOVER_HEIGHT = 20.0f; 
	//揺れる速さ
	const float HOVER_SPEED = 2.0f;

	int shotmodel_;

	int shotCharge_;

	//攻撃判定
	bool isAttack_;
	//生存判定
	bool isAlive_ = true;
	//プレイヤー視認判定
	bool look_ = false;
	//連続攻撃判定
	bool attackHit_ = false;;

	bool shotFired_;


	// 更新ステップ
	float step_;// 状態管理(更新ステップ)
	//プレイヤーとの距離
	float distance_;
	//経過時間
	float hoverTime_;  
	//基準高さ
	float baseHeight_;    
	//プレイヤー判定球の半径
	float playerRad_;
	// 状態
	STATE state_;

	VECTOR worldPos;
	//プレイヤー方向
	VECTOR toPlayer_;
	//プレイヤー座標
	VECTOR playerPos_;


	// 状態遷移
	void ChangeState(STATE state);

	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateAttack(void);
	void ChangeStateMove(void);
	void ChangeStateWait(void);
	void ChangeStateHit(void);
	void ChangeStateEnd(void);
	void ChangeStateDie(void);
	void ChangeStateCharge(void);

	// 更新系
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateAttack(void);
	void UpdateMove(void);
	void UpdateWait(void);
	void UpdateHit(void);
	void UpdateDie(void);
	void UpdateEnd(void);
	void UpdateCharge(void);

	//弾発射時
	void AttackShot(void);

	//弾用更新・描画
	void UpdateShot(void);
	void DrawShot(void);

};




