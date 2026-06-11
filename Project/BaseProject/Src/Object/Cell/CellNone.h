#pragma once
#include "CellBase.h"

class CellNone : public CellBase
{
public:
	CellNone(Field* field, Vector2 pos, int size);
	CellNone(Field* field, Vector2 pos, int size, int actionCount);
	~CellNone();

	void Init() override;
	void Update() override;

private:
};