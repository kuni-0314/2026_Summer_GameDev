#pragma once
#include "CellBase.h"

class CellNone : public CellBase
{
public:
	CellNone(Field* field, Vector2 pos, int size);
	CellNone(Field* field, Vector2 pos, int size, int actionCount);
	~CellNone(void);

	void Init(void) override;
	void Update(void) override;

private:
};