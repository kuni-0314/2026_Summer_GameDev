#include "CellAttack.h"

CellAttack::CellAttack(Vector2 pos, int size)
	: CellBase(CELL_TYPE::ATTACK, pos, size)
{
}

CellAttack::CellAttack(Vector2 pos, int size, int actionCount)
	: CellBase(CELL_TYPE::ATTACK, pos, size, actionCount)
{
}

CellAttack::~CellAttack(void)
{
}

void CellAttack::Init(void)
{
	CellBase::Init();
	// 攻撃セル固有の初期化処理をここに追加
}

void CellAttack::Update(void)
{
	CellBase::Update();
	// 攻撃セル固有の更新処理をここに追加
}
