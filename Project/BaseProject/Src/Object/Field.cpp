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


	/*for (int y = 0; y < PLAYER_FIELD_CELL_Y; y++)
	{
		for (int x = 0; x < PLAYER_FIELD_CELL_X; x++)
		{
			std::shared_ptr<CellBase> cell = std::make_shared<CellBase>(
				static_cast<CellBase::CELL_TYPE>(playerField_[y][x]),
				Vector2(x * PLAYER_FIELD_CELL_SIZE, y * PLAYER_FIELD_CELL_SIZE),
				PLAYER_FIELD_CELL_SIZE
			);
			cell->Init();

			playerCells_.push_back(cell);
		}
	}
	for (int y = 0; y < ENEMY_FIELD_CELL_Y; y++)
	{
		for (int x = 0; x < ENEMY_FIELD_CELL_X; x++)
		{
			std::shared_ptr<CellBase> cell = std::make_shared<CellBase>(
				static_cast<CellBase::CELL_TYPE>(enemyField_[y][x]),
				Vector2(x * ENEMY_FIELD_CELL_SIZE + Application::SCREEN_SIZE_X / 2, y * ENEMY_FIELD_CELL_SIZE),
				ENEMY_FIELD_CELL_SIZE
			);
			cell->Init();
			enemyCells_.push_back(cell);
		}
	}
	for (int y = 0; y < SELECT_FIELD_CELL_Y; y++)
	{
		for (int x = 0; x < SELECT_FIELD_CELL_X; x++)
		{
			std::shared_ptr<CellBase> cell = std::make_shared<CellBase>(
				static_cast<CellBase::CELL_TYPE>(selectField_[y][x]),
				Vector2(x * SELECT_FIELD_CELL_SIZE, y * SELECT_FIELD_CELL_SIZE + Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE),
				SELECT_FIELD_CELL_SIZE
			);
			cell->Init();
			selectCells_.push_back(cell);
		}
	}*/
}

void Field::Update(void)
{
	for (auto& cell : cells_)
	{
		cell->SetActive(cell->GetActionCount() == fieldManager_->GetCurrentActionCount());
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

	//for (int y = 0; y < SELECT_FIELD_CELL_Y; y++)
	//{
	//	for (int x = 0; x < SELECT_FIELD_CELL_X; x++)
	//	{
	//		// 仮でフィールドを正方形で前面に枠のみ描画（下側に配置）
	//		selectCells_[y * SELECT_FIELD_CELL_X + x]->Draw();
	//	}
	//}
}

void Field::Release(void)
{
}

void Field::SetActionCount(const int index, const int actionCount)
{
	cells_[index]->SetActionCount(actionCount);
}