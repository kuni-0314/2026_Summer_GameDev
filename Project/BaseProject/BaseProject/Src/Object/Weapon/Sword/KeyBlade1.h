#pragma once
#include "SwordBase.h"

class KeyBlade1 : public SwordBase
{
public:
	KeyBlade1(VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform);
	~KeyBlade1();

	// çXêV
	void Update() override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitAnimation() override;
	void InitPost() override;
};