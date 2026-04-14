#include <DxLib.h>
#include "CellBase.h"

CellBase::CellBase(const CELL_TYPE type, const Vector2& pos, const int size)
	: type_(type), pos_(pos), CELL_SIZE(size)
{
	isActive_ = false;
	actionCount_ = 0;
	color_ = CELL_NONE_COLOR;
}

CellBase::~CellBase(void)
{
}

void CellBase::Init(void)
{
	switch (type_)
	{
	case CELL_TYPE::NONE:
		color_ = CELL_NONE_COLOR;
		break;
	case CELL_TYPE::ATTACK:
		color_ = CELL_ATTACK_COLOR;
		break;
	case CELL_TYPE::DEFENSE:
		color_ = CELL_DEFENSE_COLOR;
		break;
	case CELL_TYPE::BUFF:
		color_ = CELL_BUFF_COLOR;
		break;
	case CELL_TYPE::DEBUFF:
		color_ = CELL_DEBUFF_COLOR;
		break;
	}

}

void CellBase::Update(void)
{
}

void CellBase::Draw(void)
{
	DrawBox(
		pos_.x,
		pos_.y,
		pos_.x + CELL_SIZE,
		pos_.y + CELL_SIZE,
		color_,
		isActive_
	);

	DrawFormatString(
		pos_.x + 10,
		pos_.y + 10,
		color_,
		"%d",
		actionCount_
	);
}

void CellBase::Release(void)
{
}