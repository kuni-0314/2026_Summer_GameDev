#pragma once
#include <vector>
#include <memory>
#include "../Common/Vector2.h"
class FieldManager;
class CellBase;

class FieldBase
{
public:
	FieldBase(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	virtual ~FieldBase(void);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	void SetActionCount(const int index, const int actionCount);	// セルの行動回数を設定

protected:
	std::vector<std::shared_ptr<CellBase>> cells_;
	const int CELL_NUM_X;	// セル幅
	const int CELL_NUM_Y;	// セル高さ
	const int CELL_TOTAL;	// セル総数
	const int CELL_SIZE;	// セルサイズ
	Vector2 pos_;			// 描画位置

	FieldManager* fieldManager_;
};