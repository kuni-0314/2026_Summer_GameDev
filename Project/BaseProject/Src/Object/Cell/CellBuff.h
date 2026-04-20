#pragma once
#include "CellBase.h"

class CellBuff : public CellBase
{
public:
	CellBuff(Field* field, Vector2 pos, int size);
	CellBuff(Field* field, Vector2 pos, int size, int actionCount);
	~CellBuff(void);

	void Init(void) override;
	void Update(void) override;

private:
};