#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Object/CellBase.h"
#include "Field.h"

Field::Field(void)
{
}

Field::~Field(void)
{
}

void Field::Init(void)
{
	for (int y = 0; y < PLAYER_FIELD_CELL_Y; y++)
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
	}
}

void Field::Update(void)
{
	// セルの行動タイミングを設定
	int maxCells = max(PLAYER_FIELD_CELL_TOTAL, ENEMY_FIELD_CELL_TOTAL);
	short cnt = 0;
	for (int i = 0; i < maxCells; i++)
	{
		if (i < PLAYER_FIELD_CELL_TOTAL)
		{
			playerCells_[i]->SetActionCount(++cnt);
		}
		if (i < ENEMY_FIELD_CELL_TOTAL)
		{
			enemyCells_[i]->SetActionCount(++cnt);
		}
	}

	// 現在の行動回数に応じてセルをアクティブにする

	const float ACTION_INTERVAL_TIME = 0.25f;	// 行動間隔時間（秒）
	static float actionTimer_ = 0.0f;	// 行動タイマー
	actionTimer_ += SceneManager::GetInstance().GetDeltaTime();
	if (actionTimer_ >= ACTION_INTERVAL_TIME)
	{
		currentActionCount_++;
		if (currentActionCount_ >= MAX_ACTION_COUNT)
		{
			currentActionCount_ = 0;
		}
		actionTimer_ -= ACTION_INTERVAL_TIME;
	}

	for (auto& cell : playerCells_)
	{
		cell->SetActive(cell->GetActionCount() == currentActionCount_);
	}
	for (auto& cell : enemyCells_)
	{
		cell->SetActive(cell->GetActionCount() == currentActionCount_);
	}
	if (currentActionCount_ >= MAX_ACTION_COUNT)
	{
		currentActionCount_ = 0;
	}
}

void Field::Draw(void)
{
	unsigned int color = 0;
	short cnt = 0;

	// 総行動回数
	int totalActionCount = PLAYER_FIELD_CELL_TOTAL + ENEMY_FIELD_CELL_TOTAL;

	// プレイヤーと敵のセルを交互に描画
	int maxCells = (PLAYER_FIELD_CELL_TOTAL > ENEMY_FIELD_CELL_TOTAL) ? PLAYER_FIELD_CELL_TOTAL : ENEMY_FIELD_CELL_TOTAL;

	for (int i = 0; i < maxCells; i++)
	{
		// プレイヤーのセル
		if (i < PLAYER_FIELD_CELL_TOTAL)
		{
			playerCells_[i]->Draw();
		}
		// 敵のセル
		if (i < ENEMY_FIELD_CELL_TOTAL)
		{
			enemyCells_[i]->Draw();
		}
	}

	for (int y = 0; y < SELECT_FIELD_CELL_Y; y++)
	{
		for (int x = 0; x < SELECT_FIELD_CELL_X; x++)
		{
			// 仮でフィールドを正方形で前面に枠のみ描画（下側に配置）
			selectCells_[y * SELECT_FIELD_CELL_X + x]->Draw();
		}
	}
}

void Field::Release(void)
{
}