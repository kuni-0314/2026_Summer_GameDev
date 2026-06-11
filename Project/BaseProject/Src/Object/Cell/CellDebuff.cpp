#include "CellDebuff.h"

CellDebuff::CellDebuff(Field* field, Vector2 pos, int size)
	: CellBase(field, CELL_TYPE::DEBUFF, pos, size)
{
}

CellDebuff::CellDebuff(Field* field, Vector2 pos, int size, int actionCount)
	: CellBase(field, CELL_TYPE::DEBUFF, pos, size, actionCount)
{
}

CellDebuff::~CellDebuff()
{
}

void CellDebuff::Init()
{
	CellBase::Init();
	// デバフセル固有の初期化処理をここに追加
}

void CellDebuff::Update()
{
	CellBase::Update();
	// デバフセル固有の更新処理をここに追加
}