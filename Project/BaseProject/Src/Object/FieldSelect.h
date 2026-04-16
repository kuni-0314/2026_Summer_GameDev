#pragma once
#include "FieldBase.h"

class FieldSelect : public FieldBase
{
public:
	FieldSelect(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	~FieldSelect(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
};