#pragma once
#include <memory>
#include "../../Charactor/CharactorBase.h"
class GameScene;
class AnimationController;
class PlayerState;
class SwordBase;
class EffekseerEffect;
class ColliderSphere;

class Player : public CharactorBase
{

public:
	// 状態
	enum class STATE
	{
		IDLE,
		RUN, 
		//FAST_RUN, 
		JUMP, 
		JET, 
		FALL,
		ATTACK,
		MAGIC,  // 追加
		MAX
	};

	//アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		ATK_N1,
		ATK_N2,
		ATK_N3,
		ATK_N4,
		ATK_N5,
		ATK_H,
		ATK_D,
		ATK_A1,
		ATK_A2,
		ATK_A3,
		ATK_A4,
		ATK_A5,
		ATK_F,
		MAGIC,
	};

	//コンストラクタ
	Player(int padNum);
	//デストラクタ
	~Player() override;

	void Update() override;

	

	void Draw() override;
	
	void ChangeState(STATE newState);
 
	AnimationController* GetAnimationController() const { return animationController_;}
	VECTOR GetMovePow() const { return movePow_; }
	void SetMovePow(const VECTOR& pow) { movePow_ = pow; }
	void SetMoveSpeed(const float speed) { moveSpeed_ = speed; }
	VECTOR GetMoveDir() const { return moveDir_; }
	void SetMoveDir(const VECTOR& dir) { moveDir_ = dir; }
	VECTOR GetJumpPow() const { return jumpPow_; }
	void SetJumpPow(const VECTOR& pow) { jumpPow_ = pow; }
	float GetStepJump() const { return stepJump_; }
	void SetStepJump(const float step) { stepJump_ = step; }
	bool IsJump() const { return isJump_; }
	void SetJump(const bool isJump) { isJump_ = isJump; }
	bool IsAir() const { return isAir_; }
	void SetAir(const bool isAir) { isAir_ = isAir; }
	bool IsJet() const { return isJet_; }
	void SetJet(const bool isJet) { isJet_ = isJet; }
	float GetJetTime() const { return jetTime_; }
	void SetJetTime(const float time) { jetTime_ = time; }
	int GetAttackCoolTime() const { return attackCoolTime_; }
	void SetAttackCoolTime(const int time) { attackCoolTime_ = time; }
	int GetComboTimer() const { return comboTimer_; }
	void SetComboTimer(const int time) { comboTimer_ = time; }
	GameScene* GetGameScene() const { return gameScene_; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	bool GetApplyRootMotion() const { return applyRootMotion_; }
	void SetApplyRootMotion(const bool apply) { applyRootMotion_ = apply; }
	VECTOR GetAnimStartModelPos() const { return animStartModelPos_; }
	void SetAnimStartModelPos(const VECTOR& pos) { animStartModelPos_ = pos; }
	bool IsAttacking() const { return isAttacking_; }
	void SetAttacking(const bool attacking) { isAttacking_ = attacking; }
	bool IsAliveMagic() const { return isAliveMagic_; }
	void SetAliveMagic(const bool alive) { isAliveMagic_ = alive; }
	void ActivatePowerUp();
	void PlayBlinkEffect();


	//スケール
	static constexpr float SCL_PlAYER = 0.75f;
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
	static constexpr float SPEED_MOVE = 8.0f;
	// 移動速度(ダッシュ)
	//static constexpr float SPEED_DASH = 10.0f;

	// 衝突判定用線分開始(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_START_LOCAL_POS = { 0.0f, 130.0f, 0.0f };
	// 衝突判定用線分終了(ジャンプ時)
	static constexpr VECTOR COL_LINE_JUMP_END_LOCAL_POS = { 0.0f, 50.0f, 0.0f };;
	
	// ジャンプ力
	static constexpr float POW_JUMP_INIT = 40.0f;
	// 連打ジャンプ力
	static constexpr float POW_JUMP_KEEP = 8.5f;
	// ジャンプ力付与時間
	static constexpr float TIME_JUMP_INPUT = 1.5f;
	// ニュートラル状態でのジャンプ力
	static constexpr float POW_JUMP_NEUTRAL = 7.0f;

	
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

	VECTOR GetPos() const { return transform_.pos; }
	void SetPos(const VECTOR& pos) { transform_.pos = pos; }
	float GetCollRadius() const { return COL_CAPSULE_RADIUS; }

	int GetPadNum() const { return padNum_; }

	SwordBase* GetSword() const { return sword_; }

	Quaternion GetRot(void) const { return transform_.quaRot; }

	// 攻撃判定の開始フレーム
	static constexpr int ATTACK_HITBOX_START_FRAME = 22;
	static constexpr int ATTACK_HITBOX_END_FRAME = 29;

	// ダメージ処理
	void Damege(int damege);

	// HPの回復処理
	void HealHp(int heal) override;

	static constexpr int MAX_HP = 20;

protected:

	// リソースロード
	void InitLoad() override;

	// 大きさ、回転、座標の初期化
	void InitTransform() override;

	// 衝突判定の初期化
	void InitCollider() override;

	// アニメーションの初期化
	void InitAnimation() override;

	// 初期化後の後処理
	void InitPost() override;

	//更新系
	virtual void UpdateProcess() override;
	virtual void UpdateProcessPost() override;


private:

	void CheckPlayerRingCollision();

	void GrantStatus(int index);
	void RevokeStatus(int index);

	void InitState();

	struct ThunderInfo
	{
		Transform transform = {};
		ColliderSphere* collider = nullptr;
		//VECTOR pos;
		int timer = 0;
		bool isActive = false;
		bool isDestroyed = false;
	};
	void CreateThunderCollider(ThunderInfo& thunderInfo);

	void DestroyThunderCollider(const ThunderInfo& thunderInfo);

	const int padNum_;

	Status status_;
	int pendingPoints_;
	int currentGrantStatusIndex_;
	int maxHp_;
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

	// パワーアップ状態フラグ（あなたのプロジェクトの状態管理に合わせて調整してください）
	bool m_isPowerUp = false;       // パワーアップ中
	int m_powerUpTimer = 0;         // エフェクト時間

	PlayerState* currentState_;
	std::map<STATE, PlayerState*> states_;

	// 現在プレイヤーに追従しているエフェクトのポインタ
	std::shared_ptr<EffekseerEffect> m_powerUpEffect;

	std::shared_ptr<EffekseerEffect> blinkEffect_;

	// 攻撃のクールタイム
	int attackCoolTime_;
	// コンボタイマー(前回の攻撃からの経過フレーム)
	int comboTimer_;

	GameScene* gameScene_;

	// アニメーション再生前のモデル座標
	VECTOR animStartModelPos_;
	
	// ルートモーションを適用するかどうか
	bool applyRootMotion_;
	VECTOR debugPos_;
	VECTOR debugPosPrev_;
	
	// 差分
	VECTOR animDiffPos_;

	// 剣
	SwordBase* sword_;


	bool isAttacking_;

	// 一旦ここに書いとく
	static constexpr VECTOR KEY_BLADE_1_LOCAL_POS_START = { 0.0f, 15.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_1_LOCAL_POS_END = { 0.0f, 90.0f, 0.0f };
	static constexpr float KEY_BLADE_1_RADIUS = 40.0f;
	static constexpr VECTOR KEY_BLADE_2_LOCAL_POS_START = { 0.0f, 15.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_2_LOCAL_POS_END = { 0.0f, 70.0f, 0.0f };
	static constexpr float KEY_BLADE_2_RADIUS = 40.0f;
	static constexpr VECTOR KEY_BLADE_3_LOCAL_POS_START = { 0.0f, 30.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_3_LOCAL_POS_END = { 0.0f, 120.0f, 0.0f };
	static constexpr float KEY_BLADE_3_RADIUS = 40.0f;


	bool isAliveMagic_;
	int magicTimer_;

	static constexpr int THUNDER_COUNT = 10;		// 雷の数
	ThunderInfo thunderInfos_[THUNDER_COUNT];		// 雷の情報配列
	static constexpr int THUNDER_LIFETIME = 60;		// 生存時間
	static constexpr int THUNDER_INTERVAL = 5;		// 発生間隔
	static constexpr float THUNDER_RADIUS = 100.0f;	// 雷の半径
	//VECTOR thunderPosOffsets_[THUNDER_COUNT];


};
