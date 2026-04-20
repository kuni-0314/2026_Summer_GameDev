#pragma once
#include "CellBase.h"

class CellDefense : public CellBase
{
public:
	CellDefense(Field* field, Vector2 pos, int size);
	CellDefense(Field* field, Vector2 pos, int size, int actionCount);
	~CellDefense(void);

	void Init(void) override;
	void Update(void) override;

private:
};