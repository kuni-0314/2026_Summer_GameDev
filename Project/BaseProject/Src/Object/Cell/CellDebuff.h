#pragma once
#include "CellBase.h"

class CellDebuff : public CellBase
{
public:
	CellDebuff(Field* field, Vector2 pos, int size);
	CellDebuff(Field* field, Vector2 pos, int size, int actionCount);
	~CellDebuff(void);

	void Init(void) override;
	void Update(void) override;

private:
};