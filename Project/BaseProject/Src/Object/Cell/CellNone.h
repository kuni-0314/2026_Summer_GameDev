#pragma once
#include "CellBase.h"

class CellNone : public CellBase
{
public:
	CellNone(Vector2 pos, int size);
	CellNone(Vector2 pos, int size, int actionCount);
	~CellNone(void);

	void Init(void) override;
	void Update(void) override;

private:
};