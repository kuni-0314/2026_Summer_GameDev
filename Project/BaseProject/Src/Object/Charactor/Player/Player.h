#pragma once
#include <memory>
#include "../../Charactor/CharactorBase.h"
#include "../../../Effect/LoadEffekseer/EffekseerEffect.h"
class GameScene;
class AnimationController;
class PlayerState;
class SwordBase;
class EffekseerEffect;
class ColliderSphere;

class Player : public CharactorBase
{

public:
	// ó‘Ô
	enum class STATE
	{
		IDLE,
		RUN,
		//FAST_RUN, 
		JUMP,
		JET,
		FALL,
		ATTACK,
		MAGIC,  // ’Ç‰Á
		MAX
	};

	//ƒAƒjƒ[ƒVƒ‡ƒ“í•Ê
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
		ROLLING,
		DAMAGE,
	};

	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	Player(int padNum);
	//ƒfƒXƒgƒ‰ƒNƒ^
	~Player() override;

	void Update() override;



	void Draw() override;

	void ChangeState(STATE newState);

	bool IsShortCut();

	AnimationController* GetAnimationController() const { return animationController_; }
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
	void DeleteFireEffect();

	//ƒXƒP[ƒ‹
	static constexpr float SCL_PlAYER = 0.75f;
	//‰ŠúÀ•W
	static constexpr VECTOR POS_PLAYER = { 0.0f, 0.0f, 0.0f };
	//‰Šú‰ñ“]
	static constexpr VECTOR ROT_LOCAL_PLAYER = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// Õ“Ë”»’è—pü•ªŠJn
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// Õ“Ë”»’è—pü•ªI—¹
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// Õ“Ë”»’è—pƒJƒvƒZƒ‹ã•”‹…‘Ì(ƒWƒƒƒ“ƒv)
	static constexpr VECTOR COL_CAPSULE_TOP_JUMP_LOCAL_POS = { 0.0f, 160.0f, 0.0f };
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‰º•”‹…‘Ì(ƒWƒƒƒ“ƒv)
	static constexpr VECTOR COL_CAPSULE_DOWN_JUMP_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// ˆÚ“®‘¬“x(’Êí)
	static constexpr float SPEED_MOVE = 8.0f;
	// ˆÚ“®‘¬“x(ƒ_ƒbƒVƒ…)
	//static constexpr float SPEED_DASH = 10.0f;

	// Õ“Ë”»’è—pü•ªŠJn(ƒWƒƒƒ“ƒv)
	static constexpr VECTOR COL_LINE_JUMP_START_LOCAL_POS = { 0.0f, 130.0f, 0.0f };
	// Õ“Ë”»’è—pü•ªI—¹(ƒWƒƒƒ“ƒv)
	static constexpr VECTOR COL_LINE_JUMP_END_LOCAL_POS = { 0.0f, 50.0f, 0.0f };;

	// ƒWƒƒƒ“ƒv—Í
	static constexpr float POW_JUMP_INIT = 40.0f;
	// ˜A‘ÅƒWƒƒƒ“ƒv—Í
	static constexpr float POW_JUMP_KEEP = 8.5f;
	// ƒWƒƒƒ“ƒv—Í•t—^ŠÔ
	static constexpr float TIME_JUMP_INPUT = 1.5f;
	// ƒjƒ…[ƒgƒ‰ƒ‹ó‘Ô‚Å‚ÌƒWƒƒƒ“ƒv—Í
	static constexpr float POW_JUMP_NEUTRAL = 7.0f;


	static constexpr float POW_ROLL = 20.0f;

	static constexpr float ROLL_TIME = 0.25f;

	// ’nã‚Å‚ÌˆÚ“®Œ¸Š—¦
	static constexpr float GROUND_MOVE_DEC_RATE = 0.8f;

	// ‹ó’†‚Å‚ÌˆÚ“®Œ¸Š—¦
	static constexpr float AIR_MOVE_DEC_RATE = 0.975f;


	// Õ“Ë”»’è—pƒJƒvƒZƒ‹ã•”À•W
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‰º•”À•W
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‚Ì”¼Œa
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// Õ“ËŒŸo‚ÌƒTƒuƒXƒeƒbƒv”(‚‘¬ˆÚ“®‚Ì‚·‚è”²‚¯–h~)
	static constexpr int COLLISION_SUB_STEPS = 4;
	// ’Ç‰ÁƒXƒ‰ƒCƒh‚ÌÅ‘åÀs”
	static constexpr int MAX_SLIDE_ITERATIONS = 4;
	// ƒXƒ‰ƒCƒh•â³—p‚ÌÅ¬ˆÚ“®—Ê
	static constexpr float MIN_MOVE_THRESHOLD = 0.01f;

	VECTOR GetPos() const { return transform_.pos; }
	void SetPos(const VECTOR& pos) { transform_.pos = pos; }
	float GetCollRadius() const { return COL_CAPSULE_RADIUS; }

	int GetPadNum() const { return padNum_; }

	SwordBase* GetSword() const { return sword_; }

	Quaternion GetRot(void) const { return transform_.quaRot; }

	// UŒ‚”»’è‚ÌŠJnƒtƒŒ[ƒ€
	static constexpr int ATTACK_HITBOX_START_FRAME = 22;
	static constexpr int ATTACK_HITBOX_END_FRAME = 29;

	// ƒ_ƒ[ƒWˆ—
	void Damage(int damage, const VECTOR& hitDir) override;

	// HP‚Ì‰ñ•œˆ—
	void HealHp(int heal) override;

	static constexpr int MAX_HP = 20;

	int GetThunderCoolTime();
	int GetFireCoolTime();
	int GetHealCoolTime();

	// ”ÍˆÍUŒ‚
	void ExecuteRangeAttack();
	static constexpr float ATTACK_RANGE = 200.0f; // ”ÍˆÍUŒ‚‚Ì”¼Œa 

protected:

	// ƒŠƒ\[ƒXƒ[ƒh
	void InitLoad() override;

	// ‘å‚«‚³A‰ñ“]AÀ•W‚Ì‰Šú‰»
	void InitTransform() override;

	// Õ“Ë”»’è‚Ì‰Šú‰»
	void InitCollider() override;

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰Šú‰»
	void InitAnimation() override;

	// ‰Šú‰»Œã‚ÌŒãˆ—
	void InitPost() override;

	//XVŒn
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
		std::shared_ptr<EffekseerEffect> effect;
	};
	void CreateThunderCollider(ThunderInfo& thunderInfo);

	void DestroyThunderCollider(const ThunderInfo& thunderInfo);

	struct FireInfo
	{
		Transform transform = {};
		ColliderSphere* collider = nullptr;
<<<<<<< HEAD
		int timer = 0;
		VECTOR dir = {};
=======

		VECTOR moveDir;

		int timer = 0;

		std::shared_ptr<EffekseerEffect> effect;
>>>>>>> origin/ã«ã‚“
	};
	void CreateFireCollider(FireInfo& fireInfo);

	void DestroyFireCollider(const FireInfo& fireInfo);

	const int padNum_;

	STATE state_;

	Status status_;
	int pendingPoints_;
	int currentGrantStatusIndex_;
	int maxHp_;
	static constexpr int MAX_STATUS_INDEX = 9;

	// ‰ŠúƒXƒe[ƒ^ƒXiŒãXcsv‚É‚Å‚àj4
	static constexpr int DEFAULT_LEVEL = 1;	// Å‰‚¾‚©‚ç‚ ‚½‚è‚Ü‚¦‚Å‚Í‚ ‚é
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

	// ƒpƒ[ƒAƒbƒvó‘Ôƒtƒ‰ƒOi‚ ‚È‚½‚ÌƒvƒƒWƒFƒNƒg‚Ìó‘ÔŠÇ—‚É‡‚í‚¹‚Ä’²®‚µ‚Ä‚­‚¾‚³‚¢j
	bool m_isPowerUp = false;       // ƒpƒ[ƒAƒbƒv’†
	int m_powerUpTimer = 0;         // ƒGƒtƒFƒNƒgŠÔ

	PlayerState* currentState_;
	std::map<STATE, PlayerState*> states_;

	// Œ»İƒvƒŒƒCƒ„[‚É’Ç]‚µ‚Ä‚¢‚éƒGƒtƒFƒNƒg‚Ìƒ|ƒCƒ“ƒ^
	std::shared_ptr<EffekseerEffect> m_powerUpEffect;

	std::shared_ptr<EffekseerEffect> blinkEffect_;

	std::shared_ptr<EffekseerEffect> recoveryEffect_;

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int attackCoolTime_;
	// ƒRƒ“ƒ{ƒ^ƒCƒ}[(‘O‰ñ‚ÌUŒ‚‚©‚ç‚ÌŒo‰ßƒtƒŒ[ƒ€)
	int comboTimer_;

	GameScene* gameScene_;

	// ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶‘O‚Ìƒ‚ƒfƒ‹À•W
	VECTOR animStartModelPos_;

	// ƒ‹[ƒgƒ‚[ƒVƒ‡ƒ“‚ğ“K—p‚·‚é‚©‚Ç‚¤‚©
	bool applyRootMotion_;
	VECTOR debugPos_;
	VECTOR debugPosPrev_;

	// ·•ª
	VECTOR animDiffPos_;

	// Œ•
	SwordBase* sword_;

	//UŒ‚’†”»’èƒtƒ‰ƒO
	bool isAttacking_;

	// ˆê’U‚±‚±‚É‘‚¢‚Æ‚­
	static constexpr VECTOR KEY_BLADE_1_LOCAL_POS_START = { 0.0f, 15.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_1_LOCAL_POS_END = { 0.0f, 90.0f, 0.0f };
	static constexpr float KEY_BLADE_1_RADIUS = 40.0f;
	static constexpr VECTOR KEY_BLADE_2_LOCAL_POS_START = { 0.0f, 15.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_2_LOCAL_POS_END = { 0.0f, 70.0f, 0.0f };
	static constexpr float KEY_BLADE_2_RADIUS = 40.0f;
	static constexpr VECTOR KEY_BLADE_3_LOCAL_POS_START = { 0.0f, 30.0f, 0.0f };
	static constexpr VECTOR KEY_BLADE_3_LOCAL_POS_END = { 0.0f, 120.0f, 0.0f };
	static constexpr float KEY_BLADE_3_RADIUS = 40.0f;


	//bool isAliveThunder_;
	int thunderTimer_;

	static constexpr int THUNDER_COUNT = 10;			// —‹‚Ì”
	ThunderInfo thunderInfos_[THUNDER_COUNT];			// —‹‚Ìî•ñ”z—ñ
	static constexpr int THUNDER_LIFETIME = 30;			// ¶‘¶ŠÔ
	static constexpr int THUNDER_INTERVAL = 5;			// ”­¶ŠÔŠu
	static constexpr float THUNDER_RADIUS = 100.0f;		// —‹‚Ì”¼Œa
	static constexpr float THUNDER_FALL_SPEED = 100.0f;	// —‹‚Ì—‰º‘¬“x
	static constexpr float THUNDER_SPAWN_Y = 500.0f;	// —‹‚Ì”­¶ˆÊ’uYÀ•W

	void CreateFireMagic();
	void CreateThunderMagic();
	void CreateHealMagic();

	void MagicCoolTime();//–‚–@‚ÌƒN[ƒ‹ƒ^ƒCƒ€	

	void UpdateMagic();
	FireInfo fireInfo_;

	static constexpr int FIRE_LIFETIME = 90;		// ¶‘¶ŠÔ
	static constexpr float FIRE_RADIUS = 100.0f;	// ‰Î‚Ì”¼Œa
	static constexpr float FIRE_SPEED = 50.0f;		// ‰Î‚ÌˆÚ“®‘¬“x

	static constexpr int FIRE_COOL_TIME = 300;		// ƒN[ƒ‹ƒ^ƒCƒ€
	bool isAliveMagic_ = false;	// –‚–@‚Ì¶‘¶ó‘Ô
	int fireCoolTime_ = 0;	// ƒN[ƒ‹ƒ^ƒCƒ€ƒJƒEƒ“ƒ^
	bool isAliveFire_ = false;	// ‰Î‚Ì¶‘¶ó‘Ô
	//VECTOR fireDir_ = { 0.0f, 0.0f, 1.0f };	// ‰Î‚ÌˆÚ“®•ûŒü
	static constexpr int THUNDER_COOL_TIME = 300;	// ƒN[ƒ‹ƒ^ƒCƒ€
	int thunderCoolTime_ = 0;	// ƒN[ƒ‹ƒ^ƒCƒ€ƒJƒEƒ“ƒ^
	bool isAliveThunder_ = false;	// —‹‚Ì¶‘¶ó‘Ô
	static constexpr int HEAL_COOL_TIME = 600;	// ƒN[ƒ‹ƒ^ƒCƒ€
	int healCoolTime_ = 0;	// ƒN[ƒ‹ƒ^ƒCƒ€ƒJƒEƒ“ƒ^
	static constexpr int HEAL_AMOUNT = 5;	// ‰ñ•œ—Ê

	bool isShortCut_ = false;

};
