#pragma once
#include "SwordBase.h"

class KeyBlade2 : public SwordBase
{
public:
	KeyBlade2(VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform);
	~KeyBlade2();

	// çXêV
	void Update() override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitAnimation() override;
	void InitPost() override;
};