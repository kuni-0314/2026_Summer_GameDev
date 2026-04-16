#pragma once
#include "FieldBase.h"

class FieldPlayer : public FieldBase
{
public:
	FieldPlayer(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	~FieldPlayer(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
};