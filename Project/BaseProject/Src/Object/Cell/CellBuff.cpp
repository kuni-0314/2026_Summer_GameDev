#include "CellBuff.h"

CellBuff::CellBuff(Field* field, Vector2 pos, int size)
	: CellBase(field, CELL_TYPE::BUFF, pos, size)
{
}

CellBuff::CellBuff(Field* field, Vector2 pos, int size, int actionCount)
	: CellBase(field, CELL_TYPE::BUFF, pos, size, actionCount)
{
}

CellBuff::~CellBuff(void)
{
}

void CellBuff::Init(void)
{
	CellBase::Init();
	// バフセル固有の初期化処理をここに追加
}

void CellBuff::Update(void)
{
	CellBase::Update();
	// バフセル固有の更新処理をここに追加
}