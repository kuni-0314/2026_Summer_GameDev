#include <random>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Scene/GameScene.h"
#include "FieldManager.h"
#include "Field.h"

FieldManager::FieldManager(GameScene* gameScene)
	: gameScene_(gameScene)
{
}

FieldManager::~FieldManager(void)
{
}

void FieldManager::Init(void)
{
	Vector2 fieldPos(100, 300);
	playerField_ = std::make_unique<Field>(this, Field::FIELD_TYPE::PLAYER,
		PLAYER_FIELD_CELL_X, PLAYER_FIELD_CELL_Y, PLAYER_FIELD_CELL_SIZE,
		fieldPos);
	playerField_->Init();
	fieldPos = Vector2(Application::SCREEN_SIZE_X / 2, 0);
	enemyField_ = std::make_unique<Field>(this, Field::FIELD_TYPE::ENEMY,
		ENEMY_FIELD_CELL_X, ENEMY_FIELD_CELL_Y, ENEMY_FIELD_CELL_SIZE,
		fieldPos);
	enemyField_->Init();
	fieldPos = Vector2(0, Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE);
	selectField_ = std::make_unique<Field>(this, Field::FIELD_TYPE::SELECT,
		SELECT_FIELD_CELL_X, SELECT_FIELD_CELL_Y, SELECT_FIELD_CELL_SIZE,
		fieldPos);
	selectField_->Init();

	imageCellRed_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CELL_RED).handleId_;
}

void FieldManager::Update(void)
{
	// セルの行動タイミングを設定
	int maxCells = GetMaxFieldCell();
	short cnt = 0;
	for (int i = 0; i < maxCells; i++)
	{
		if (i < PLAYER_FIELD_CELL_TOTAL)
		{
			playerField_->SetActionCount(i, ++cnt);
		}
		if (i < ENEMY_FIELD_CELL_TOTAL)
		{
			enemyField_->SetActionCount(i, ++cnt);
		}
	}

	// 現在の行動回数に応じてセルをアクティブにする
	const float ACTION_INTERVAL_TIME = 0.25f;	// 行動間隔時間(秒)
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

	// マウスを離したか
	if (InputManager::GetInstance()->IsMouseTrgUp(MOUSE_INPUT_LEFT))
	{
		CellBase::CELL_TYPE cellType = CellBase::CELL_TYPE::NONE;
		if (playerField_->GetHoldingCellType() != CellBase::CELL_TYPE::NONE)
		{
			cellType = playerField_->GetHoldingCellType();
		}
		else if (enemyField_->GetHoldingCellType() != CellBase::CELL_TYPE::NONE)
		{
			cellType = enemyField_->GetHoldingCellType();
		}
		else if (selectField_->GetHoldingCellType() != CellBase::CELL_TYPE::NONE)
		{
			cellType = selectField_->GetHoldingCellType();
		}

		if (cellType != CellBase::CELL_TYPE::NONE)
		{
			if (playerField_->IsInFieldMouse())
			{
				playerField_->SetCellType(cellType);
			}
			else if (enemyField_->IsInFieldMouse())
			{
				enemyField_->SetCellType(cellType);
			}
			else if (selectField_->IsInFieldMouse())
			{
				selectField_->SetCellType(cellType);
			}
		}
	}

	playerField_->Update();
	enemyField_->Update();
	selectField_->Update();

	if (currentActionCount_ >= MAX_ACTION_COUNT)
	{
		currentActionCount_ = 0;
	}

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_R))
	{
		// メルセンヌ・ツイスタを使用した高精度乱数生成
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, static_cast<int>(SKILL_LIST::MAX) - 1);

		for (int i = 0; i < GENERATE_SKILL_NUM; i++)
		{
			generatedSkills_[i] = static_cast<SKILL_LIST>(dist(mt));
		}
	}

}

void FieldManager::Draw(void)
{
	playerField_->Draw();
	enemyField_->Draw();
	selectField_->Draw();
	
	//generatedSkills_
	for (int i = 0; i < GENERATE_SKILL_NUM; i++)
	{
		// 背景黒
		//DrawBox(0, i * 20, 100, i * 20 + 20, GetColor(0, 0, 0), true);	
		//DrawFormatString(0, i * 20, GetColor(255, 255, 255), "%d", static_cast<int>(generatedSkills_[i]));
	
		for (int y = 0; y < SKILL_MAX_SIZE; y++)
		{
			for (int x = 0; x < SKILL_MAX_SIZE; x++)
			{
				bool isDraw = false;
				switch (generatedSkills_[i])
				{
				case SKILL_LIST::SLASH:
					isDraw = slashPiece[y][x];
					break;
				case SKILL_LIST::DEFENCE:
					isDraw = defencePiece[y][x];
					break;
				case SKILL_LIST::HEEL:
					isDraw = heelPiece[y][x];
					break;
				case SKILL_LIST::PROTECT:
					isDraw = protectPiece[y][x];
					break;
				case SKILL_LIST::FRAME:
					isDraw = framePiece[y][x];
					break;
				default:
					break;
				}

				const int SKILL_DRAW_POS_Y = Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE - 100;

				if (isDraw)
				{
					//DrawGraph(x * BASE_CELL_SIZE + i * SKILL_MAX_SIZE * BASE_CELL_SIZE, y * BASE_CELL_SIZE + SKILL_DRAW_POS_Y, imageCellRed_, true);
					const int DRAW_CELL_SIZE = 72;
					DrawExtendGraph(
						x * DRAW_CELL_SIZE + i * SKILL_MAX_SIZE * DRAW_CELL_SIZE, y * DRAW_CELL_SIZE + SKILL_DRAW_POS_Y,
						(x + 1) * DRAW_CELL_SIZE + i * SKILL_MAX_SIZE * DRAW_CELL_SIZE, (y + 1) * DRAW_CELL_SIZE + SKILL_DRAW_POS_Y, imageCellRed_, false);
				}
			}
		}
	}

}

void FieldManager::Release(void)
{
	selectField_->Release();
	selectField_.reset();
	
	enemyField_->Release();
	enemyField_.reset();
	
	playerField_->Release();
	playerField_.reset();
}

int FieldManager::GetMaxFieldCell(void) const
{
	return max(PLAYER_FIELD_CELL_TOTAL, ENEMY_FIELD_CELL_TOTAL);
}

int FieldManager::GetCurrentActionCount(void) const
{
	return currentActionCount_;
}

int FieldManager::GetTotalActionCount(void) const
{
	return PLAYER_FIELD_CELL_TOTAL + ENEMY_FIELD_CELL_TOTAL;
}
