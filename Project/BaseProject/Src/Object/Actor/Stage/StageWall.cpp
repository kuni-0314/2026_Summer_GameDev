#include "StageWall.h"
#include "Stage.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Collider/Model/ColliderModel.h"

StageWall::StageWall(void)
	:
	ActorBase()
{
}

StageWall::~StageWall(void)
{
}

void StageWall::Update()
{
}



void StageWall::InitLoad(void)
{
 	transform_.SetModel(resMng_.Load(ResourceManager::SRC::BATTLE_WALL).handleId_);
}

void StageWall::InitTransform(void)
{

	transform_.scl = { SCL_MAIN_STAGE_X, SCL_MAIN_STAGE_Y, SCL_MAIN_STAGE_Z };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity();
	transform_.pos = POS_MAIN_STAGE;

	transform_.Update();

}

void StageWall::InitCollider(void)
{
	// DxLib側の衝突情報セットアップ
	MV1SetupCollInfo(transform_.modelId);
	// モデルのコライダ
	ColliderModel* colModel =
		new ColliderModel(ColliderBase::TAG::STAGE, &transform_);
	for (const std::string& name : EXCLUDE_FRAME_NAMES)
	{
		colModel->AddExcludeFrameIds(name);
	}

	//対象とするフレーム
	for (const std::string& name : TARGET_FRAME_NAMES)
	{
		colModel->AddTargetFrameIds(name);
	}


	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::MODEL), colModel);

}

void StageWall::InitAnimation(void)
{
}

void StageWall::InitPost(void)
{
}
