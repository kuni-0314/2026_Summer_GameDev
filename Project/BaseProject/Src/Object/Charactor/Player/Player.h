#pragma once
#include "../../Charactor/CharactorBase.h"
class AnimationController;
class PlayerState;

class Player : public CharactorBase
{

public:
	// 状態
	enum class STATE
	{
		IDLE,
		WALK, 
		DASH, 
		JUMP, 
		ATTACK, 
		JET, 
	};

	//アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
	};

	//コンストラクタ
	Player(void);
	//デストラクタ
	~Player(void) override;

	void Update(void) override;

	

	void Draw(void) override;

	void ChangeState(STATE newState);
	AnimationController* GetAnimationController(void) const { return animationController_;}
	VECTOR GetMovePow(void) const { return movePow_; }
	void SetMovePow(const VECTOR& pow) { movePow_ = pow; }

	//スケール
	static constexpr float SCL_PlAYER = 1.0f;
	//初期座標
	static constexpr VECTOR POS_PLAYER = { 0.0f, 0.0f, 0.0f };
	//初期回転
	static constexpr VECTOR ROT_LOCAL_PLAYER = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体(ジャンプ時)
	static constexpr VECTOR COL_CAPSULE_TOP_JUMP_LOCAL_POS = { 0.0f, 160.0f, 0.0f };
	// 衝突判定用カプセル下部球体(ジャンプ時)
	static constexpr VECTOR COL_CAPSULE_DOWN_JUMP_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 移動速度(通常)
	static constexpr float SPEED_MOVE = 5.0f;
	// 移動速度(ダッシュ)
	static constexpr float SPEED_DASH = 10.0f;

	// 衝突判定用線分開始(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_START_LOCAL_POS = { 0.0f, 130.0f, 0.0f };
	// 衝突判定用線分終了(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_END_LOCAL_POS = { 0.0f, 50.0f, 0.0f };;
	
	// ジャンプ力
	static constexpr float POW_JUMP_INIT = 35.0f;
	// 持続ジャンプ力
	static constexpr float POW_JUMP_KEEP = 8.0f;
	// ジャンプ受付時間
	static constexpr float TIME_JUMP_INPUT = 0.5f;

	
	static constexpr float POW_JET = 100.0f;

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// 衝突判定のサブステップ数（高速移動時のすり抜け防止）
	static constexpr int COLLISION_SUB_STEPS = 4;
	// 壁沿いスライドの最大試行回数
	static constexpr int MAX_SLIDE_ITERATIONS = 4;
	// スライド判定用の最小移動量
	static constexpr float MIN_MOVE_THRESHOLD = 0.01f;
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

	//更新系
	virtual void UpdateProcess(void) override;
	virtual void UpdateProcessPost(void) override;


private:

	void GrantStatus(int index);
	void RevokeStatus(int index);

	// 操作
	void ProcessMove(void);
	//ジャンプ
	void ProcessJump(void);
	//ジェット
	void ProcessJet(void);
	//
	void ProcessAttack(void);

	// 衝突判定
	void CollisionReserve(void) override;

	void InitState(void);

	Status status_;
	int pendingPoints_;
	int currentGrantStatusIndex_;
	static constexpr int MAX_STATUS_INDEX = 9;

	// 初期ステータス（後々csvにでも）4
	static constexpr int DEFAULT_LEVEL = 1;	// 最初だからあたりまえではある
	static constexpr int DEFAULT_HP = 25;
	static constexpr int DEFAULT_MP = 5;
	static constexpr int DEFAULT_PHYS_ATK = 5;
	static constexpr int DEFAULT_PHYS_DEF = 5;
	static constexpr int DEFAULT_MAGIC_ATK = 5;
	static constexpr int DEFAULT_MAGIC_DEF = 5;
	static constexpr int DEFAULT_WISDOM = 5;
	static constexpr int DEFAULT_LUCK = 5;

	bool isJet_;
	float jetTime_;
	static constexpr float TIME_JET = 0.2f;

	PlayerState* currentState_;
	std::map<STATE, PlayerState*> states_;
};

