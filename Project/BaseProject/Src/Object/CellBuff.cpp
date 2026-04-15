#include "CellBuff.h"

CellBuff::CellBuff(Vector2 pos, int size)
	: CellBase(CELL_TYPE::BUFF, pos, size)
{
}

CellBuff::CellBuff(Vector2 pos, int size, int actionCount)
	: CellBase(CELL_TYPE::BUFF, pos, size, actionCount)
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