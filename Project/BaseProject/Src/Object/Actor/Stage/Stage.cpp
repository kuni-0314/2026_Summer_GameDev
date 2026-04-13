#include "Stage.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Object/Collider/Model/ColliderModel.h"

Stage::Stage(void)
	:
	ActorBase()
{
}

Stage::~Stage(void)
{
}

void Stage::Update()
{
}



void Stage::InitLoad(void)
{
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::MAIN_STAGE).handleId_);
}

void Stage::InitTransform(void)
{

	transform_.scl = { SCL_MAIN_STAGE ,SCL_MAIN_STAGE ,SCL_MAIN_STAGE };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Identity();
	transform_.pos =POS_MAIN_STAGE;

	transform_.Update();

}

void Stage::InitCollider(void)
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

void Stage::InitAnimation(void)
{
}

void Stage::InitPost(void)
{
}
