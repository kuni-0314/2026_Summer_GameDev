#pragma once
#include "SwordBase.h"

class KeyBlade3 : public SwordBase
{
public:
	KeyBlade3(VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform);
	~KeyBlade3();
	// çXêV
	void Update() override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitAnimation() override;
	void InitPost() override;
};