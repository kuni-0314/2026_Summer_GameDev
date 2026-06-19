
#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include "../EnemyBase.h"

class Player;
class ItemManger;


class EnemyRase : public EnemyBase
{
public:
	// ƒAƒjƒ[ƒVƒ‡ƒ“í•Ê
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		ATTACK,
		CHARGE,
		END,
		HIT
	};

	// ó‘Ô
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

	//’e‚Ìƒpƒ‰ƒ[ƒ^[(
	struct SHOT
	{
		bool isAlive_ = false;
		float speed = 4.5f;
		int life = 180;
		float homingPower = 0.06f;
		VECTOR dir_;
		Transform shotTransform_;
	
	};
	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^

	EnemyRase(const EnemyBase::EnemyData& data, int attackModel,Player* player);
	// ƒfƒXƒgƒ‰ƒNƒ^
	~EnemyRase() override;

<<<<<<< HEAD
	void Draw() override;
=======
	void Draw(void) override;

	void Release(void) override;
>>>>>>> origin/Î±ä¿®æ­£ï¼’
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

	ItemManger* itemManager_;

	std::vector<SHOT> shots_;


	//ƒAƒjƒ[ƒVƒ‡ƒ““o˜^”Ô†
	//‘Ò‹@
	static constexpr int  ANIM_INDX_FRY = 2;
	//HIT
	static constexpr int  ANIM_INDX_HIT = 4;
	//END
	static constexpr int  ANIM_INDX_DEAD = 0;
	//ƒ`ƒƒ[ƒW
	static constexpr int  ANIM_INDX_CHARGE = 5;
	
	//’e‘Ò‹@ƒJƒEƒ“ƒg
	static constexpr int  SHOT_CHARGE_COUNT = 60;

	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	static constexpr float SCALE = 0.3f;

	// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³
	static constexpr float SHOT_SCALE = 0.1f;
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

	// UŒ‚Ø‚è‘Ö‚¦‹——£
	static constexpr float SWICH_DISTANCE = 350.0f;

	static constexpr float ATTACK_MOVE_SPEED = 3.0f;

	//—h‚ê•
	const float HOVER_HEIGHT = 20.0f; 
	//—h‚ê‚é‘¬‚³
	const float HOVER_SPEED = 2.0f;

	int shotmodel_;

	int shotCharge_;

	//UŒ‚”»’è
	bool isAttack_;
	//¶‘¶”»’è
	bool isAlive_ = true;
	//ƒvƒŒƒCƒ„[‹”F”»’è
	bool look_ = false;
	//˜A‘±UŒ‚”»’è
	bool attackHit_ = false;;

	bool shotFired_;


	// XVƒXƒeƒbƒv
	float step_;// ó‘ÔŠÇ—(XVƒXƒeƒbƒv)
	//ƒvƒŒƒCƒ„[‚Æ‚Ì‹——£
	float distance_;
	//Œo‰ßŠÔ
	float hoverTime_;  
	//Šî€‚‚³
	float baseHeight_;    
	//ƒvƒŒƒCƒ„[”»’è‹…‚Ì”¼Œa
	float playerRad_;
	// ó‘Ô
	STATE state_;

	VECTOR worldPos;
	//ƒvƒŒƒCƒ„[•ûŒü
	VECTOR toPlayer_;
	//ƒvƒŒƒCƒ„[À•W
	VECTOR playerPos_;


	// ó‘Ô‘JˆÚ
	void ChangeState(STATE state);
<<<<<<< HEAD
	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateAttack();
	void ChangeStateHit();
	void ChangeStateEnd();


	// XVŒn
	void UpdateThink();
	void UpdateIdle();
	void UpdateAttack();
	void UpdateHit();
	void UpdateDie();
	void UpdateEnd();
=======
	void ChangeStateThink(void);
	void ChangeStateIdle(void);
	void ChangeStateAttack(void);
	void ChangeStateMove(void);
	void ChangeStateWait(void);
	void ChangeStateHit(void);
	void ChangeStateEnd(void);
	void ChangeStateCharge(void);

	// XVŒn
	void UpdateThink(void);
	void UpdateIdle(void);
	void UpdateAttack(void);
	void UpdateMove(void);
	void UpdateWait(void);
	void UpdateHit(void);
	void UpdateDie(void);
	void UpdateEnd(void);
	void UpdateCharge(void);
>>>>>>> origin/Î±ä¿®æ­£ï¼’

	//’e”­Ë
	void AttackShot(void);

	//’e—pXVE•`‰æ
	void UpdateShot(void);
	void DrawShot(void);

};




