#pragma once
#include "CellBase.h"

class CellDefense : public CellBase
{
public:
	CellDefense(Field* field, Vector2 pos, int size);
	CellDefense(Field* field, Vector2 pos, int size, int actionCount);
	~CellDefense();

	void Init() override;
	void Update() override;

private:
};