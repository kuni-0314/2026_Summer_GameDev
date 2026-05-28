#pragma once
#include "../../Charactor/CharactorBase.h"
class GameScene;
class AnimationController;
class PlayerState;

class Player : public CharactorBase
{

public:
	// 状態
	enum class STATE
	{
		IDLE,
		RUN, 
		FAST_RUN, 
		JUMP, 
		JET, 
		FALL,
		ATTACK,
		MAX
	};

	//アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		ATTACK,
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
	void SetMoveSpeed(const float speed) { moveSpeed_ = speed; }
	VECTOR GetMoveDir(void) const { return moveDir_; }
	void SetMoveDir(const VECTOR& dir) { moveDir_ = dir; }
	VECTOR GetJumpPow(void) const { return jumpPow_; }
	void SetJumpPow(const VECTOR& pow) { jumpPow_ = pow; }
	float GetStepJump(void) const { return stepJump_; }
	void SetStepJump(const float step) { stepJump_ = step; }
	bool IsJump(void) const { return isJump_; }
	void SetJump(const bool isJump) { isJump_ = isJump; }
	bool IsAir(void) const { return isAir_; }
	void SetAir(const bool isAir) { isAir_ = isAir; }
	bool IsJet(void) const { return isJet_; }
	void SetJet(const bool isJet) { isJet_ = isJet; }
	float GetJetTime(void) const { return jetTime_; }
	void SetJetTime(const float time) { jetTime_ = time; }
	int GetAttackCoolTime(void) const { return attackCoolTime_; }
	void SetAttackCoolTime(const int time) { attackCoolTime_ = time; }
	int GetComboTimer(void) const { return comboTimer_; }
	void SetComboTimer(const int time) { comboTimer_ = time; }
	GameScene* GetGameScene(void) const { return gameScene_; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


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
	static constexpr float POW_JUMP_INIT = 40.0f;
	// 連打ジャンプ力
	static constexpr float POW_JUMP_KEEP = 8.5f;
	// ジャンプ力付与時間
	static constexpr float TIME_JUMP_INPUT = 0.5f;

	
	static constexpr float POW_JET = 80.0f;

	static constexpr float JET_TIME = 0.25f;

	// 地上での移動減衰率
	static constexpr float GROUND_MOVE_DEC_RATE = 0.8f;

	// 空中での移動減衰率
	static constexpr float AIR_MOVE_DEC_RATE = 0.975f;


	// 衝突判定用カプセル上部座標
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部座標
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// 衝突判定用カプセルの半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// 衝突検出のサブステップ数(高速移動時のすり抜け防止)
	static constexpr int COLLISION_SUB_STEPS = 4;
	// 追加スライドの最大実行数
	static constexpr int MAX_SLIDE_ITERATIONS = 4;
	// スライド補正用の最小移動量
	static constexpr float MIN_MOVE_THRESHOLD = 0.01f;

	VECTOR GetPos(void) const { return transform_.pos; }
	float GetCollRadius(void) const { return COL_CAPSULE_RADIUS; }


protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の後処理
	void InitPost(void) override;

	//更新系
	virtual void UpdateProcess(void) override;
	virtual void UpdateProcessPost(void) override;

private:

	void GrantStatus(int index);
	void RevokeStatus(int index);

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

	// 攻撃のクールタイム
	int attackCoolTime_;
	// コンボタイマー(前回の攻撃からの経過フレーム)
	int comboTimer_;

	GameScene* gameScene_;
};
