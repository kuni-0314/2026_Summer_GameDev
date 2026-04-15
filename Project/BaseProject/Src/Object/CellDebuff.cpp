#include "CellDebuff.h"

CellDebuff::CellDebuff(Vector2 pos, int size)
	: CellBase(CELL_TYPE::DEBUFF, pos, size)
{
}

CellDebuff::CellDebuff(Vector2 pos, int size, int actionCount)
	: CellBase(CELL_TYPE::DEBUFF, pos, size, actionCount)
{
}

CellDebuff::~CellDebuff(void)
{
}

void CellDebuff::Init(void)
{
	CellBase::Init();
	// デバフセル固有の初期化処理をここに追加
}

void CellDebuff::Update(void)
{
	CellBase::Update();
	// デバフセル固有の更新処理をここに追加
}