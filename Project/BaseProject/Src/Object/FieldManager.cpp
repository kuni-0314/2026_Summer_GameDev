#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "FieldManager.h"
#include "FieldPlayer.h"
#include "FieldEnemy.h"
#include "FieldSelect.h"

FieldManager::FieldManager(GameScene* gameScene)
	: gameScene_(gameScene)
{
}

FieldManager::~FieldManager(void)
{
}

void FieldManager::Init(void)
{
	Vector2 fieldPos(0, 0);
	playerField_ = std::make_shared<FieldPlayer>(this,
		PLAYER_FIELD_CELL_X, PLAYER_FIELD_CELL_Y, PLAYER_FIELD_CELL_SIZE,
		fieldPos);
	playerField_->Init();
	fieldPos = Vector2(Application::SCREEN_SIZE_X / 2, 0);
	enemyField_ = std::make_shared<FieldEnemy>(this,
		ENEMY_FIELD_CELL_X, ENEMY_FIELD_CELL_Y, ENEMY_FIELD_CELL_SIZE,
		fieldPos);
	enemyField_->Init();
	fieldPos = Vector2(0, Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE);
	selectField_ = std::make_shared<FieldSelect>(this,
		SELECT_FIELD_CELL_X, SELECT_FIELD_CELL_Y, SELECT_FIELD_CELL_SIZE,
		fieldPos);
	selectField_->Init();
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

	playerField_->Update();
	enemyField_->Update();
	selectField_->Update();

	if (currentActionCount_ >= MAX_ACTION_COUNT)
	{
		currentActionCount_ = 0;
	}
}

void FieldManager::Draw(void)
{
	playerField_->Draw();
	enemyField_->Draw();
	selectField_->Draw();
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

FieldManager::CELL_TYPE* FieldManager::GetFieldCellTypes(const int fieldIndex) const
{
	switch (fieldIndex)
	{
	case 1:
		return (CELL_TYPE*)playerFieldCellTypes_;
	case 2:
		return (CELL_TYPE*)enemyFieldCellTypes_;
	case 3:
		return (CELL_TYPE*)selectFieldCellTypes_;
	default:
		return (CELL_TYPE*)nullptr;
	}
}
