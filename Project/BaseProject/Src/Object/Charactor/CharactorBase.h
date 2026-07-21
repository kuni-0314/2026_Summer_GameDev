#pragma once
#include "../Actor/ActorBase.h"

class AnimationController;
class CharactorBase : public ActorBase
{
public:

	//// Õ“Ë”»’èí•Ê
	//enum class COLLIDER_TYPE
	//{
	//	LINE,
	//	CAPSULE,
	//	SPHERE,
	//	VIEW_RAGE,
	//	MAX,
	//};

	// ƒXƒe[ƒ^ƒX
	struct Status
	{
		int level;		// ƒŒƒxƒ‹
		int hp;			// ‘Ì—Í
		int mp;			// –‚—Í
		int physAtk;	// •¨—UŒ‚—Í
		int physDef;	// •¨—–hŒä—Í
		int magicAtk;	// –‚–@UŒ‚—Í
		int magicDef;	// –‚–@–hŒä—Í
		int wisdom;		// Œ«‚³
		int luck;		// ‰^
	};

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	CharactorBase();
	// ƒfƒXƒgƒ‰ƒNƒ^
	virtual ~CharactorBase() override;

	//XV
	virtual void Update() override;

	virtual void Draw() override;

	virtual void Release() override;

	// ƒ_ƒ[ƒWˆ—
	virtual void Damage(int damage);

	virtual void Damage(int damage, const VECTOR& hitDir);

	// HP‚Ìæ“¾
	int GetHp() const { return hp_; }

	void SetAlive(bool alive) { isAlive_ = alive; }

	bool IsAnimEnd();

	virtual void HealHp(int heal) { hp_ += heal; };

<<<<<<< HEAD
	bool IsInvincible() const { return isInvincible_; }
	void SetInvincible(bool invincible);
=======
>>>>>>> origin/FIXï¼’

protected:


	// Å‘å—‰º‘¬“x
	static constexpr float MAX_FALL_SPEED = -15.0f;
	// Õ“Ë‚Ì‰Ÿ‚µ–ß‚µs‰ñ”
	static constexpr int CNT_TRY_COLLISION = 20;
	// Õ“Ë‚Ì‰Ÿ‚µ–ß‚µ—Ê
	static constexpr float COLLISION_BACK_DIS = 1.0f;

	//ˆÚ“®—Ê
	VECTOR movePow_;
	// ˆÚ“®•ûŒü
	VECTOR moveDir_;
	// ƒWƒƒƒ“ƒv—Ê
	VECTOR jumpPow_;
	// ˆÚ“®‘O‚ÌÀ•W
	VECTOR prevPos_;

	// ƒmƒbƒNƒoƒbƒN—Ê
	VECTOR knockbackPow_;

	// ƒWƒƒƒ“ƒv‚Ì“ü—Íó•tŠÔ
	float stepJump_;
	//ˆÚ“®ƒXƒs[ƒh
	float moveSpeed_;

	// ƒWƒƒƒ“ƒv”»’è
	bool isJump_;

	// ‹ó’†”»’è
	bool isAir_;

	// ŠÛ‰e‰æ‘œ
	int imgShadow_;

	//‘Ì—Í
	int hp_;

	//‹ó’†‚É•‚‚­—p
	bool useGrabity_ = true;

	// ƒŠƒ\[ƒXƒ[ƒh
	virtual void InitLoad() override;

	virtual void InitAnimation() override;

	// XVŒn
	virtual void UpdateProcess() = 0;
	virtual void UpdateProcessPost() = 0;
	// ˆÚ“®•ûŒü‚É‰‚¶‚½’x‰„‰ñ“]
	void DelayRotate();

	// d—ÍŒvZ
	void CalcGravityPow();

	// Õ“Ë”»’è
	virtual void  CollisionReserve() {}
	void Collision();
	void CollisionGravity();
	//ƒJƒvƒZƒ‹Œ^“–‚½‚è”»’èiflag:true‚È‚ç‚¸“xˆ—‚ğs‚¤j
	void CollisionCapsule();


	// ŠÛ‰e•`‰æ
	void DrawShadow();

	AnimationController* animationController_;

	int GetInvincibleFrameCount() const { return invincibleFrameCount_; }

	// ‘Ìd
	enum class WEIGHT
	{
		NONE,		// ‘Ìd‚È‚µ
		LIGHT,		// Œy‚¢
		NORMAL,		// •’Ê
		HEAVY,		// d‚¢
		IMMOBILE	// •s“®
	};
	WEIGHT weight_;
	// –³“GƒtƒŒ[ƒ€”
	static constexpr int INVINCIBLE_FRAME_COUNT = 10;
	int invincibleFrameCount_ = 0;
	bool isInvincible_;
private:

	int stage_; //‰e‚Ì”»’è—pƒXƒe[ƒWƒnƒ“ƒhƒ‹

	float PLAYER_SHADOW_HEIGHT = 800.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;

	VECTOR SlideVec;//‰e‚ÌƒxƒNƒgƒ‹
	VERTEX3D Vertex[3];
	

};

