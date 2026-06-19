#pragma once
#include <functional>
#include "../CharactorBase.h"
#include <DxLib.h>
class Player;
class ShotBase;
class ShotStraight;

class EnemyBase : public CharactorBase
{
public:

	// í•Ê
	enum class TYPE
	{
		RAT,
		RASE,
	};
	// ƒGƒlƒ~[ƒf[ƒ^
	struct EnemyData
	{
		int id;
		EnemyBase::TYPE type;
		int hp;
		VECTOR defaultPos;
		float movableRange;
		int power;
	};

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	EnemyBase(const EnemyBase::EnemyData& data,int attackModel,Player* player);
	// ƒfƒXƒgƒ‰ƒNƒ^
<<<<<<< HEAD
	virtual ~EnemyBase() override;
	
=======
	virtual ~EnemyBase(void) override;
>>>>>>> origin/Î±ä¿®æ­£ï¼’

	//•`‰æ
	virtual void Draw() override;

<<<<<<< HEAD
	// ƒvƒŒƒCƒ„[‚ÌŒ•‚Æ‚ÌÕ“Ë”»’è‚ğs‚¤
	void CheckPlayerSwordCollision();
=======
	void Release(void) override;

>>>>>>> origin/Î±ä¿®æ­£ï¼’

protected:

	Player* player_;
	// í•Ê
	TYPE type_;

	// ‰ŠúˆÊ’u
	const VECTOR defaultPos_;
	float movableRange_;

	float playerRad_;
	int power_;
	//UŒ‚—pƒ‚ƒfƒ‹(–‚–@‚Ì’e‚Æ‚©j
	int attackModle_;

	// ó‘ÔŠÇ—
	int stateBase_;
	// ó‘ÔŠÇ—(ó‘Ô‘JˆÚ‰Šúˆ—)
	std::map<int, std::function<void()>> stateChanges_;
	// ó‘ÔŠÇ—(XVƒXƒeƒbƒv)
	std::function<void()> stateUpdate_;


	// ƒŠƒ\[ƒXƒ[ƒh
	void InitLoad() override {}
	// ‘å‚«‚³A‰ñ“]AÀ•W‚Ì‰Šú‰»
	void InitTransform() override {}
	// Õ“Ë”»’è‚Ì‰Šú‰»
	void InitCollider() override {}
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰Šú‰»
	void InitAnimation() override {}
	// ‰Šú‰»Œã‚ÌŒÂ•Êˆ—
	void InitPost() override {}

	// ó‘Ô‘JˆÚ
	void ChangeState(int state);

	// XVŒn
	virtual void UpdateProcessPost() override {}
	// ˆÚ“®‰Â”\”ÍˆÍ”»’è
	bool InMovableRange() const;

	//‹üƒxƒNƒgƒ‹
	void LookPlayer();


};

