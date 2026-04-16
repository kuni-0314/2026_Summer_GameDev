#include "CellDefense.h"

CellDefense::CellDefense(Vector2 pos, int size)
	: CellBase(CELL_TYPE::DEFENSE, pos, size)
{
}

CellDefense::CellDefense(Vector2 pos, int size, int actionCount)
	: CellBase(CELL_TYPE::DEFENSE, pos, size, actionCount)
{
}

CellDefense::~CellDefense(void)
{
}

void CellDefense::Init(void)
{
	CellBase::Init();
	// 防御セル固有の初期化処理をここに追加
}

void CellDefense::Update(void)
{
	CellBase::Update();
	// 防御セル固有の更新処理をここに追加
}