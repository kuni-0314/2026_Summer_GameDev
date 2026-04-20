#pragma once
#include "CellBase.h"

class CellAttack : public CellBase
{
public:
	CellAttack(Field* field, Vector2 pos, int size);
	CellAttack(Field* field, Vector2 pos, int size, int actionCount);
	~CellAttack(void);

	void Init(void) override;
	void Update(void) override;

private:
};

