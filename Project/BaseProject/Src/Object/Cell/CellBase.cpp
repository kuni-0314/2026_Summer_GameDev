#include <DxLib.h>
#include "CellBase.h"

CellBase::CellBase(CELL_TYPE type, Vector2 pos, int size)
	:
	type_(type),
	pos_(pos),
	size_(size),
	actionCount_(0),
	color_(0),
	isActive_(false)
{
}

CellBase::CellBase(CELL_TYPE type, Vector2 pos, int size, int actionCount)
	:
	type_(type),
	pos_(pos),
	size_(size),
	actionCount_(actionCount),
	color_(0),
	isActive_(false)
{
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
	// セル内にマウスカーソルがあるかどうか
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	if (mouseX >= pos_.x && mouseX < pos_.x + size_ &&
		mouseY >= pos_.y && mouseY < pos_.y + size_)
	{
		SetActive(true);
	}
	else
	{
		SetActive(false);
	}
}

void CellBase::Draw(void)
{
	DrawBox(
		pos_.x,
		pos_.y,
		pos_.x + size_,
		pos_.y + size_,
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