//#include "ShotBase.h"
//#include "../../../../Manager/ResourceManager.h"
//#include "ShotStraight.h"
//
//
//ShotStraight::ShotStraight(TYPE type, int baseModelId)
//	:ShotBase(type,baseModelId)
//{
//	
//}
//ShotStraight::~ShotStraight(void)
//{
//}
//
//void ShotStraight::InitLoad(void)
//{
//	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RASE_BALL));
//}
//
//void ShotStraight::InitTransform(void)
//{
//	//ëÂÇ´Ç≥ÅAç¿ïWìôÇÃèâä˙âª
//	transform_.scl = { SCALE ,SCALE ,SCALE };
//	transform_.quaRot = Quaternion::Identity();
//	transform_.quaRotLocal = Quaternion::Euler(ROT);
//
//	transform_.Update();
//}
//
//void ShotStraight::InitCollider(void)
//{
//}
//
//void ShotStraight::InitAnimation(void)
//{
//}
//
//void ShotStraight::InitPost(void)
//{
//}
//
//void ShotStraight::SetParam(void)
//{
//	scales_ = { 0.4f, 0.4f, 0.4f };
//	speed_ = 8.0f;
//	isAlive_ = true;
//	cntAlive_ = 300;
//	collisionRadius_ = 30.0f;
//}
