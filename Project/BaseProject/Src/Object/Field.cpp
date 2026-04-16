#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "Field.h"

Field::Field(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	:
	fieldManager_(fieldManager),
	CELL_NUM_X(CELL_NUM_X),
	CELL_NUM_Y(CELL_NUM_Y),
	CELL_TOTAL(CELL_NUM_X * CELL_NUM_Y),
	CELL_SIZE(CELL_SIZE),
	pos_(pos)
{
}

Field::~Field(void)
{
}

void Field::Init(void)
{
	for (int y = 0;y < CELL_SIZE; y++)
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

void Field::Update(void)
{
	for (auto& cell : cells_)
	{
		cell->Update();
		//cell->SetActive(cell->GetActionCount() == fieldManager_->GetCurrentActionCount());
	}
}

void Field::Draw(void)
{
	unsigned int color = 0;
	short cnt = 0;

	for (int i = 0; i < CELL_TOTAL; i++)
	{
		cells_[i]->Draw();
	}
}

void Field::Release(void)
{
	cells_.clear();
}

void Field::SetActionCount(const int index, const int actionCount)
{
	cells_[index]->SetActionCount(actionCount);
}