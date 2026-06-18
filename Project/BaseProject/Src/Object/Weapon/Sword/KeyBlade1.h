#pragma once
#include "SwordBase.h"

class KeyBlade1 : public SwordBase
{
public:
	KeyBlade1(Transform& transform);
	~KeyBlade1();

	// XV
	void Update() override;

protected:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

private:
	// UŒ‚ƒ^ƒCƒv
	enum class ATTACK_TYPE
	{
		// ƒAƒjƒ[ƒVƒ‡ƒ“‚ª–³‚¢‚©‚ç‚P‚¾‚¯À‘•‚µ‚Ä‚¨‚­
		NORMAL1 = 0,    // ’ÊíUŒ‚1
		//NORMAL2 = 1,    // ’ÊíUŒ‚2
		//NORMAL3 = 2,    // ’ÊíUŒ‚3
		//NORMAL4 = 3,    // ’ÊíUŒ‚4
		//NORMAL5 = 4,    // ’ÊíUŒ‚5
		//HEAVY = 5,      // ‹­UŒ‚
		//DASH = 6,       // ƒ_ƒbƒVƒ…UŒ‚
		//AIR1 = 7,       // ‹ó’†UŒ‚1
		//AIR2 = 8,       // ‹ó’†UŒ‚2
		//AIR3 = 9,       // ‹ó’†UŒ‚3
		//AIR4 = 10,      // ‹ó’†UŒ‚4
		//AIR5 = 11,      // ‹ó’†UŒ‚5
		//FALL = 12,      // —‰ºUŒ‚
		MAX
	};

	// ŠeUŒ‚ƒ^ƒCƒv‚Ì”»’è‚ğİ’è
	void SetupNormalAttack1();
	//void SetupNormalAttack2();
	//void SetupNormalAttack3();
	//void SetupNormalAttack4();
	//void SetupNormalAttack5();
	//void SetupHeavyAttack();
	//void SetupDashAttack();
	//void SetupAirAttack1();
	//void SetupAirAttack2();
	//void SetupAirAttack3();
	//void SetupAirAttack4();
	//void SetupAirAttack5();
	//void SetupFallAttack();
};