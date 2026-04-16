#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "FieldBase.h"

FieldBase::FieldBase(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	:
	fieldManager_(fieldManager),
	CELL_NUM_X(CELL_NUM_X),
	CELL_NUM_Y(CELL_NUM_Y),
	CELL_TOTAL(CELL_NUM_X * CELL_NUM_Y),
	CELL_SIZE(CELL_SIZE),
	pos_(pos)
{
}

FieldBase::~FieldBase(void)
{
}

void FieldBase::Init(void)
{
	for (int y = 0; y < CELL_NUM_Y; y++)
	{
		for (int x = 0; x < CELL_NUM_X; x++)
		{
			std::shared_ptr<CellBase> cell = std::make_shared<CellBase>(
				static_cast<CellBase::CELL_TYPE>(0),
				Vector2(x * CELL_SIZE + pos_.x, y * CELL_SIZE + pos_.y),
				CELL_SIZE
			);
			cell->Init();
			cells_.push_back(cell);
		}
	}
}

void FieldBase::Update(void)
{
	for (auto& cell : cells_)
	{
		cell->Update();
		//cell->SetActive(cell->GetActionCount() == fieldManager_->GetCurrentActionCount());
	}
}

void FieldBase::Draw(void)
{
	for (int i = 0; i < CELL_TOTAL; i++)
	{
		cells_[i]->Draw();
	}
}

void FieldBase::Release(void)
{
	cells_.clear();
}

void FieldBase::SetActionCount(const int index, const int actionCount)
{
	cells_[index]->SetActionCount(actionCount);
}