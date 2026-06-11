#include "CellDefense.h"

CellDefense::CellDefense(Field* field, Vector2 pos, int size)
	: CellBase(field, CELL_TYPE::DEFENSE, pos, size)
{
}

CellDefense::CellDefense(Field* field, Vector2 pos, int size, int actionCount)
	: CellBase(field, CELL_TYPE::DEFENSE, pos, size, actionCount)
{
}

CellDefense::~CellDefense()
{
}

void CellDefense::Init()
{
	CellBase::Init();
	// 防御セル固有の初期化処理をここに追加
}

void CellDefense::Update()
{
	CellBase::Update();
	// 防御セル固有の更新処理をここに追加
}