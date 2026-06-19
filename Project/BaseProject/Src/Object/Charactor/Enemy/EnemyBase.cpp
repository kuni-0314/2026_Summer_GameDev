#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../Charactor/Player/Player.h"
<<<<<<< HEAD
#include "../../Collider/ColliderBase.h"
#include "../../Collider/Capsule/ColliderCapsule.h"
=======
#include "../Enemy/Shot/ShotBase.h"
#include "../Enemy/Shot/ShotStraight.h"
>>>>>>> origin/Î±ä¿®æ­£ï¼’
#include "EnemyBase.h"


EnemyBase::EnemyBase(const EnemyBase::EnemyData& data, int attackModel, Player* player)
	:
CharactorBase(),
player_(player),
type_(data.type),
defaultPos_(data.defaultPos),
movableRange_(data.movableRange),
power_(data.power),
attackModle_(attackModel)
{

	// ‰ŠúÀ•W‚Ìİ’è
	transform_.pos = data.defaultPos;

	hp_ = data.hp;
}
<<<<<<< HEAD
EnemyBase::~EnemyBase()
=======

EnemyBase::~EnemyBase(void)
>>>>>>> origin/Î±ä¿®æ­£ï¼’
{
}

void EnemyBase::Draw()
{
	CharactorBase::Draw();

#ifdef _DEBUG
	// ˆÚ“®‰Â”\”ÍˆÍ‚ÌƒfƒoƒbƒO•`‰æ
	//DrawSphere3D(defaultPos_, 500.0f, 16, 0x000099, 0x000099, false);
#endif // _DEBUG
}

<<<<<<< HEAD
bool EnemyBase::InMovableRange() const
=======
void EnemyBase::Release(void)
{
	CharactorBase::Release();
}


bool EnemyBase::InMovableRange(void) const
>>>>>>> origin/Î±ä¿®æ­£ï¼’
{
	bool ret = false;
	// ‰ŠúˆÊ’u‚©‚ç‚Ì‹——£
	float dis = static_cast<float>(
		AsoUtility::SqrMagnitude(defaultPos_, transform_.pos));
	// w’è‹——£”»’è
	if (dis < movableRange_ * movableRange_)
	{
		return true;
	}
	return ret;

}

void EnemyBase::LookPlayer()
{

	VECTOR playerPos = player_->GetPos();

	//ƒxƒNƒgƒ‹ŒvZ
	VECTOR diff = VSub(playerPos, transform_.pos);
	diff.y = 0.0f;

	//ƒxƒNƒgƒ‹‚Ì³‹K‰»‚Å’PˆÊƒxƒNƒgƒ‹‚Ìæ“¾
	moveDir_ = VNorm(diff);

	transform_.rot.y = atan2(moveDir_.x, moveDir_.z);
	transform_.rot.y += AsoUtility::Deg2RadF(180.0f);

	//‰ñ“]‚ÍY²‚Ì‚İ‚É‚·‚é
	transform_.rot.x = transform_.rot.z = 0.0f;

	MV1SetRotationXYZ(transform_.modelId, transform_.rot);
}



void EnemyBase::ChangeState(int state)
{
	stateBase_ = state;


	auto it = stateChanges_.find(stateBase_);

	if (it != stateChanges_.end())
	{
		// “o˜^‚³‚ê‚½‰Šúˆ—‚ğÀsiEnemyRat::ChangeStateXXX ‚ª stateUpdate_ ‚ğİ’è‚·‚éj
		it->second();
	}
	else
	{
		// ƒ}ƒbƒsƒ“ƒO‚ª‚È‚¢ê‡‚Í–¾¦“I‚É‹ó‚É‚·‚éi•ÛŒ¯j
		stateUpdate_ = std::function<void()>{};
	}




}

void EnemyBase::CheckPlayerSwordCollision()
{
	// €–Só‘Ô‚È‚çˆ—‚µ‚È‚¢
	if (!isAlive_) return;

	// ©g‚ÌƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_‚ğæ“¾
	ColliderCapsule* ownColCapsule = nullptr;
	for (const auto& ownCol : ownColliders_)
	{
		if (ownCol.second->GetTag() == ColliderBase::TAG::ENEMY)
		{
			ownColCapsule =
				dynamic_cast<ColliderCapsule*>(ownCol.second);
			//if (ownColCapsule == nullptr) return;
		}
	}

	// ƒvƒŒƒCƒ„[‚ÌŒ•ƒRƒ‰ƒCƒ_‚ÍhitColliders_‚É“o˜^‚³‚ê‚Ä‚¢‚é‚Í‚¸‚È‚Ì‚ÅA‘S‚Äƒ`ƒFƒbƒN
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() == ColliderBase::TAG::PLAYER_SWORD)
		{
			// Œ•‚ÍƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// “G‚àƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_
			// ƒJƒvƒZƒ‹ƒRƒ‰ƒCƒ_“¯m‚ÅÕ“Ë”»’è
			const ColliderCapsule* swordColCapsule =
				dynamic_cast<const ColliderCapsule*>(hitCol);

			if (swordColCapsule == nullptr) return;

			// Õ“Ë”»’è
			if (ownColCapsule->IsHit(swordColCapsule))
			{
				// ƒ_ƒ[ƒWˆ—
				Damege(1);
			}
		}
	}


}

<<<<<<< HEAD


=======
>>>>>>> origin/Î±ä¿®æ­£ï¼’
