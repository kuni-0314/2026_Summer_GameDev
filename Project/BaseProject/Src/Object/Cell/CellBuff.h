#pragma once
#include "CellBase.h"

class CellBuff : public CellBase
{
public:
	CellBuff(Vector2 pos, int size);
	CellBuff(Vector2 pos, int size, int actionCount);
	~CellBuff(void);

	void Init(void) override;
	void Update(void) override;

private:
};