#pragma once
#include <DxLib.h>
#include <functional>
#include "../EnemyBase.h"
class Player;
class ItemManger;


class EnemyRat : public EnemyBase
{
public:
	// ƒAƒjƒ[ƒVƒ‡ƒ“í•Ê
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		END,
		HIT
	};

	// ó‘Ô
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

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	EnemyRat(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// ƒfƒXƒgƒ‰ƒNƒ^
	~EnemyRat() override;

	void Draw() override;
protected:
	// ƒŠƒ\[ƒXƒ[ƒh
	void InitLoad() override;
	// ‘å‚«‚³A‰ñ“]AÀ•W‚Ì‰Šú‰»
	void InitTransform() override;
	// Õ“Ë”»’è‚Ì‰Šú‰»
	void InitCollider() override;
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰Šú‰»
	void InitAnimation() override;
	// ‰Šú‰»Œã‚ÌŒÂ•Êˆ—
	void InitPost() override;
	// XVŒn
	void UpdateProcess() override;
	void UpdateProcessPost() override;


private:


	
	ItemManger* itemManager_ ;

	//ƒAƒjƒ[ƒVƒ‡ƒ““o˜^”Ô†
	static constexpr int  ANIM_INDX_IDLE = 8; 
	static constexpr int  ANIM_INDX_WALK = 13;
	static constexpr int  ANIM_INDX_ATTACK = 1;
	static constexpr int  ANIM_INDX_END = 6;
	static constexpr int  ANIM_INDX_HIT = 7;
	static constexpr int  ANIM_INDX_RUN = 11;


	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	static constexpr float SCALE = 0.5f;
	// ƒ‚ƒfƒ‹‚Ìƒ[ƒJƒ‹‰ñ“]
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// Õ“Ë”»’è—pü•ªŠJn
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// Õ“Ë”»’è—pü•ªI—¹
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// Õ“Ë”»’è—pƒJƒvƒZƒ‹ã•”‹…‘Ì
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‰º•”‹…‘Ì
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‹…‘Ì”¼Œa
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;
	// Õ“Ë”»’è—pƒJƒvƒZƒ‹‹…‘Ì”¼Œa
	static constexpr float COL_SPHERE_RADIUS = 50.0f;

	// UŒ‚”»’è—p‹…‘Ì
	static constexpr VECTOR ATTACK_SPHERE_LOCAL_POS = { 0.0f, 30.0f, 120.0f };
	// s“®Ø‚è‘Ö‚¦—pƒJƒvƒZƒ‹‹…‘Ì”¼Œa
	static constexpr float COL_SWICH_RADIUS = 250.0f;

	//‹——£ŠÖ˜A
	static constexpr float SEARCH_DISTANCE = 500.0f;
	static constexpr float SWICH_DISTANCE = 200.0f;
	static constexpr float MIN_DISTANCE = 150.0f;
	static constexpr float ATTACK_MOVE_SPEED = 3.0f;

	static constexpr int ATTACK_CHARGE = 20;

	int attackCharge_ = 20;

	//UŒ‚”»’è
	bool isAttack_; 
	//¶‘¶”»’è
	bool isAlive_ = true;
	//ƒvƒŒƒCƒ„[‹”F”»’è
	bool look_ = false;
	//˜A‘±UŒ‚”»’è
	bool attackHit_ = false;;

	// ó‘Ô
	STATE state_;

	VECTOR worldPos;
	//ƒvƒŒƒCƒ„[•ûŒü
	VECTOR toPlayer_;
	//ƒvƒŒƒCƒ„[À•W
	VECTOR playerPos_;




	// XVƒXƒeƒbƒv
	float step_;// ó‘ÔŠÇ—(XVƒXƒeƒbƒv)

	float distance_;


	// ó‘Ô‘JˆÚ
	void ChangeState(STATE state);
<<<<<<< HEAD
	void ChangeStateNone();
	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateWander();
	void ChangeStateAttack();
	void ChangeStateHit();
	void ChangeStateDie();
	void ChangeStateEnd();
	void ChangeStateRun();
	void ChangeStateWarnig();

	// XVŒn
	void UpdateNone();
	void UpdateThink();
	void UpdateIdle();
	void UpdateWander();
	void UpdateAttack();
	void UpdateHit();
	void UpdateDie();
	void UpdateEnd();
	void UpdateRun();
	void UpdateWarnig();
=======
	void ChangeStateNone(void);
	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateAttack(void);
	void ChangeStateCharge(void);
	void ChangeStateHit(void);
	void ChangeStateDie(void);
	void ChangeStateEnd(void);
	void ChangeStateRun(void);
	

	// XVŒn
	void UpdateNone(void);
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateCharge(void);
	void UpdateAttack(void);
	void UpdateHit(void);
	void UpdateDie(void);
	void UpdateEnd(void);
	void UpdateRun(void);

>>>>>>> origin/Î±ä¿®æ­£ï¼’



};

