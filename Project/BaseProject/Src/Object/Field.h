#pragma once
#include <vector>
#include <memory>
#include "../Common/Vector2.h"
class FieldManager;
class CellBase;

class Field
{
public:
	Field(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	~Field(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetActionCount(const int index, const int actionCount);	// 

protected:
	std::vector<std::shared_ptr<CellBase>> cells_;
	const int CELL_NUM_X;	// セル幅
	const int CELL_NUM_Y;	// セル高さ
	const int CELL_TOTAL;	// セル総数
	const int CELL_SIZE;	// セルサイズ
	Vector2 pos_;			// 描画位置

	FieldManager* fieldManager_;

};

