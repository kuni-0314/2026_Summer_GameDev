#pragma once
#include "FieldBase.h"

class FieldEnemy : public FieldBase
{
public:
	FieldEnemy(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	~FieldEnemy(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
};