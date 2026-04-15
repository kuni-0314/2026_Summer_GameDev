#pragma once
#include "CellBase.h"

class CellAttack : public CellBase
{
public:
	CellAttack(Vector2 pos, int size);
	CellAttack(Vector2 pos, int size, int actionCount);
	~CellAttack(void);

	void Init(void) override;
	void Update(void) override;

private:
};

