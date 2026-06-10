#include "SkillBase.h"


SkillBase::SkillBase(const SkillBase::SkillData& data, Player* player)
	:player_(player),
	type_(data.type),
	attribute_(data.attribute),
	physAtkMag_(data.physAtkMag),
	physDefMag_(data.physDefMag),
	magicAtkMag_(data.magicAtkMag),
	magicDefMag_(data.magicDefMag),
	recoveryMag_(data.recoveryMag)
{
}

SkillBase::~SkillBase(void)
{
}

void SkillBase::Init(void)
{

}

void SkillBase::Update(void)
{

}

void SkillBase::Draw(void)
{

}

void SkillBase::Release(void)
{

}

void SkillBase::LoadSkillData(void)
{

}