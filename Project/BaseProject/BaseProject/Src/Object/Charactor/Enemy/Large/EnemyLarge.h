#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include <vector>
#include "../EnemyBase.h"
#include "../../../../Effect/LoadEffekseer/EffekseerEffect.h"

class Player;
class ItemManger;
class ColliderCapsule; // 前方/後方用カプセルのポインタを保持するための前方宣言


class EnemyLarge : public EnemyBase
{
public:
	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		ATTACK_PUNCH,
		ATTACK_DROP,
		CHARGE,
		MOVE,
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
		ATTACK_PUNCH,
		ATTACK_RUN,
		ATTACK_DROP,
		WAIT,
		CHARGE,
		HIT,
		DIE,
		END
	};

	// コンストラクタ

	EnemyLarge(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// デストラクタ
	~EnemyLarge() override;

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

	void ATfield(const VECTOR& pos);


private:

	ItemManger* itemManager_;

	//アニメーション登録番号
	static constexpr int  ANIM_INDX_FRY = 2;//待機
	static constexpr int  ANIM_INDX_HIT = 4;//HIT
	static constexpr int  ANIM_INDX_DEAD = 0;//END
	static constexpr int  ANIM_INDX_CHARGE = 5;	//チャージ

	static constexpr float SCALE = 2.5f;				// モデルの大きさ

	static constexpr float RING_SCALE = 0.3f;			//衝撃波初期サイズ
	static constexpr float RING_MAX_SCALE = 15.0f;		//衝撃波最大サイズ

	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体（前半分）
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 200.0f, 60.0f };
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 70.0f, 60.0f };

	// 後半分用のカプセルローカル座標（体を前半/後半に分割）
	static constexpr VECTOR COLBODY_CAPSULE_TOP_LOCAL_POS = { 0.0f, 200.0f, -90.0f };
	static constexpr VECTOR COLBODY_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 70.0f, -90.0f };

	static constexpr VECTOR RING_ADD_SCL = { 0.1f, 0.1f, 0.1f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 40.0f;
	// 衝突判定用カプセル球体半径（球体判定用）
	static constexpr float COL_SPHERE_RADIUS = 80.0f;


	//固有で管理するコライダー番号
	static constexpr int COLLIDER_KEY_BODY_FRONT = 100;
	static constexpr int COLLIDER_KEY_BODY_BACK = 101;

	//チャージ用カウント
	int countMax = 180;		//チャージ・待機用MAXカウント		
	int countUp = 0;		//チャージ・待機用初期カウント

	//衝撃波モデルハンドルID
	int ringModelHandle_;							
	
	// 行動切り替え用カプセル球体半径
	static constexpr float COL_SWICH_RADIUS = 250.0f;

	const float SWICH_DISTANCE = 450.0f;			// 攻撃切り替え距離
	const float CONTIUATION_DISTANCE = 450.0f;		// 攻撃継続距離
	const float ATTACK_RUN_END_POINT = 600.0f;		//突進１の移動終了距離

	// ATTACK_DROP内でジャンプを行うタイミング(フレーム)
	static constexpr float ATTACK_DROP_JUMP_TIME_RATIO = 0.42f;	//ジャンプ開始
	static constexpr float ATTACK_DROP_JUMP_TIME_WAVE = 0.56f;	//衝撃波開始

	//パンチの振りかぶり切り時間
	static constexpr float ATTACK_PUNCH_TINE = 0.45f;	

	// 対象フレーム
	const std::vector<std::string> TARGET_FRAME_NAMES = { "トーラス", };
	// 除外フレーム名称
	const std::vector<std::string> EXCLUDE_FRAME_NAMES = { };
	// 更新ステップ
	float step_;// 状態管理(更新ステップ)
	//プレイヤーとの距離
	float distance_;
	//経過時間
	float hoverTime_;
	//プレイヤー判定球の半径
	float playerRad_;
	// 状態
	STATE state_;


	bool isAttack_;			//攻撃判定
	bool isAlive_ = true;	//生存判定
	bool look_ = false;		//LookPlayer用フラグ(true:ON)
	bool attackHit_ = false;//連続攻撃判定
	bool jumpApplied_ = false;	//ジャンプ処理実行判定
	bool isDrop_ = false;		//衝撃破生存判定
	bool attackTriggerRing_ = false;	//衝撃波生成判定
	bool isATField_ = false;

	VECTOR worldPos;
	//プレイヤー方向
	VECTOR toPlayer_;
	//プレイヤー座標
	VECTOR playerPos_;
	//突進用開始地点
	VECTOR startPos;
	//パンチ用攻撃座標
	VECTOR attackWorldPos_;


	//衝撃波用トランスフォーム
	std::unique_ptr<Transform> ringTransform_;
	std::shared_ptr<EffekseerEffect> shockWaveEffect_;

	// 実体の前後コライダへの直接参照（必要なら使う）
	ColliderCapsule* colFrontCapsule_ = nullptr;
	ColliderCapsule* colBackCapsule_ = nullptr;
	

	// 状態遷移
	void ChangeState(STATE state);

	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateAttackPunch();
	void ChangeStateAttackRun();
	void ChangeStateAttackDrop();
	void ChangeStateWait();
	void ChangeStateHit();
	void ChangeStateEnd();
	void ChangeStateDie();
	void ChangeStateCharge();

	// 更新系
	void UpdateThink();
	void UpdateIdle();
	void UpdateAttackPunch();
	void UpdateAttackRun();
	void UpdateAttackDrop();
	void UpdateWait();
	void UpdateHit();
	void UpdateDie();
	void UpdateEnd();
	void UpdateCharge();

	static constexpr VECTOR RING_INIT_POS = { 0.0f, -1000.0f, 0.0f };	//衝撃波初期位置
	bool wasHitRing_;
	//正面からの攻撃を取得する
	bool InFront();
};






