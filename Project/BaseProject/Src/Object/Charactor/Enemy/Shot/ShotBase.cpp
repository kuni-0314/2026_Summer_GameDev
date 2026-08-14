#include "ShotBase.h"
#include "../../../Actor/ActorBase.h"
#include <DxLib.h>


ShotBase::ShotBase(const Transform& parentTransform, const ShotData& data,
	int baseModelId, const VECTOR& dir, Player* player)
	:ActorBase()
	,speed_(data.speed)
	,life_(data.life)
	,power_(data.power)
	,shotScl_(data.shotScl)
	,isAlive_(true)
	,moveDir_(dir)
	,movePow_(AsoUtility::VECTOR_ZERO)
	,player_(player)
{
	//座標などのコピー
	transform_ = parentTransform;

	shotModel_ = MV1DuplicateModel(baseModelId);
	transform_.SetModel(shotModel_);

	transform_.scl = { shotScl_,shotScl_,shotScl_ };

}

ShotBase::~ShotBase(void)
{
	MV1DeleteModel(shotModel_);
}

void ShotBase::Init(void)
{
	InitLoad();

	transform_.Update();
}

void ShotBase::Update(void)
{
	if (!isAlive_) return;

	life_--;
	if (life_ <= 0)
	{
		isAlive_ = false;
		return;
	}

	UpdateProcess();

	if (transform_.pos.y <= 0.0f)
	{
		transform_.pos.y = 0.0f;
	}

	transform_.Update();

	UpdateProcessPost();
}

void ShotBase::Draw(void)
{
	MV1SetPosition(transform_.modelId, transform_.pos);
	MV1DrawModel(transform_.modelId);
}

void ShotBase::Release(void)
{
}





