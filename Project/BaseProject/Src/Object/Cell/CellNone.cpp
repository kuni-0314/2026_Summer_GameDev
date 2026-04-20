#include "CellNone.h"

CellNone::CellNone(Field* field, Vector2 pos, int size)
	: CellBase(field, CELL_TYPE::NONE, pos, size)
{
}

CellNone::CellNone(Field* field, Vector2 pos, int size, int actionCount)
	: CellBase(field, CELL_TYPE::NONE, pos, size, actionCount)
{
}

CellNone::~CellNone(void)
{
}

void CellNone::Init(void)
{
	CellBase::Init();
	// 通常セル固有の初期化処理をここに追加
}

void CellNone::Update(void)
{
	CellBase::Update();
	// 通常セル固有の更新処理をここに追加
}