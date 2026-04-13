#include "SkyDome.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/SceneManager.h"

SkyDome::SkyDome(const Transform& followTransform)
	:
	followTransform_(followTransform),
	state_(STATE::NONE),
	ActorBase()
{
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Update()
{

	//スカイドームをゆっくり回転させる（Y軸）
	/*transform_.quaRot = transform_.quaRot.Quaternion::Mult(
		Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(0.1f), 0.0f));*/

	switch (state_)
	{
	case SkyDome::STATE::NONE:
		UpdateNone();
		break;
	case SkyDome::STATE::STAY:
		UpdateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		UpdateFollow();
		break;
	default:
		break;
	}

	transform_.Update();

}

void SkyDome::Draw()
{
	SetUseLighting(FALSE);
	MV1DrawModel(transform_.modelId);
	SetUseLighting(TRUE);
}

void SkyDome::InitLoad(void)
{
	transform_.SetModel(resMng_.Load(ResourceManager::SRC::SKY_DOME).handleId_);
}

void SkyDome::InitTransform(void)
{
	transform_.scl = { SCL_SKYDOME ,SCL_SKYDOME ,SCL_SKYDOME };
	transform_.quaRot = Quaternion::Identity();
	transform_.quaRotLocal = Quaternion::Euler(ROT_LOCAL_SKYDOME);
	transform_.pos = POS_SKYDOME;

	transform_.Update();
}

void SkyDome::InitCollider(void)
{
}

void SkyDome::InitAnimation(void)
{
}

void SkyDome::InitPost(void)
{
	// Zバッファ無効(突き抜け対策)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	SceneManager::SCENE_ID sceneId = scnMng_.GetSceneID();

	if (sceneId == SceneManager::SCENE_ID::GAME)
	{
		ChangeState(STATE::FOLLOW);
	}
	else
	{
		ChangeState(STATE::STAY);
	}

}

void SkyDome::ChangeState(STATE state)
{
	state_ = state;
	//状態ごとの初期化
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		ChangeStateNone();
		break;
	case SkyDome::STATE::STAY:
		ChangeStateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		ChangeStateFollow();
		break;
	default:
		break;
	}
}

void SkyDome::ChangeStateNone(void)
{
}

void SkyDome::ChangeStateStay(void)
{
}

void SkyDome::ChangeStateFollow(void)
{
	transform_.pos = followTransform_.pos;
	transform_.Update();
}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{

	//モデルのY軸回転
	Quaternion rot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(0.1f), AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rot);
	transform_.Update();
}

void SkyDome::UpdateFollow(void)
{
	Quaternion rot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(0.1f), AsoUtility::AXIS_Y);
	transform_.quaRot = transform_.quaRot.Mult(rot);
	//追従
	transform_.pos = followTransform_.pos;

	transform_.Update();
}
