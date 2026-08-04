#pragma once
#include <vector>
#include <memory>
#include "Cell/CellBase.h"
#include "../Common/Vector2.h"
class FieldManager;
//class CellBase;

class Field
{
public:
	enum class FIELD_TYPE
	{
		NONE = 0,
		PLAYER,
		ENEMY,
		SELECT,
	};

	Field(FieldManager* fieldManager, FIELD_TYPE fieldType, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos);
	virtual ~Field();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Release();

	void SetActionCount(const int index, const int actionCount);	// セルの行動回数を設定

	void SetHoldingCellType(CellBase::CELL_TYPE cellType) { holdingCellType_ = cellType; }
	CellBase::CELL_TYPE GetHoldingCellType() const { return holdingCellType_; }

	bool IsInFieldMouse() const;	// マウスカーソルがフィールド内にあるかチェック
	void SetCellType(CellBase::CELL_TYPE cellType);	// フィールド内のセルのタイプを設定

protected:

	std::vector<std::unique_ptr<CellBase>> cells_;	// セルの配列
	FIELD_TYPE fieldType_;	// フィールドタイプ
	const int CELL_NUM_X;	// セル幅
	const int CELL_NUM_Y;	// セル高さ
	const int CELL_TOTAL;	// セル総数
	const int CELL_SIZE;	// セルサイズ
	Vector2 pos_;			// 描画位置

	CellBase::CELL_TYPE holdingCellType_;	// 現在保持しているセルのタイプ

	FieldManager* fieldManager_;

	// 完成時には使わなくなる予定
	void LoadFieldData();	// フィールドデータを読み込む
};