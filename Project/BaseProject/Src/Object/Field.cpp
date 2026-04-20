#include <fstream>
#include <sstream>
#include <string>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "Field.h"

Field::Field(FieldManager* fieldManager, FIELD_TYPE fieldType, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	:
	fieldManager_(fieldManager),
	fieldType_(fieldType),
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
	// CSVデータからセルを生成
	LoadFieldData();

	// CSVデータがない場合はデフォルトで初期化
	if (cells_.empty())
	{
		for (int y = 0; y < CELL_NUM_Y; y++)
		{
			for (int x = 0; x < CELL_NUM_X; x++)
			{
				std::unique_ptr<CellBase> cell = std::make_unique<CellBase>(
					this,
					static_cast<CellBase::CELL_TYPE>(0),
					Vector2(x * CELL_SIZE + pos_.x, y * CELL_SIZE + pos_.y),
					CELL_SIZE
				);
				cell->Init();
				cells_.push_back(std::move(cell));
			}
		}
	}
}

void Field::Update(void)
{
	if (InputManager::GetInstance().IsTrgUpMouseLeft()
		&& holdingCellType_ != CellBase::CELL_TYPE::NONE)
	{
		// セルの上か
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);
		for (auto& cell : cells_)
		{
			if (cell->IsMouseOver())
			{
				cell->SetType(holdingCellType_);
				break;
			}
		}
	}
	// 右クリックでセルのタイプをリセット
	if (InputManager::GetInstance().IsTrgUpMouseRight())
	{
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);
		for (auto& cell : cells_)
		{
			if (cell->IsMouseOver())
			{
				cell->SetType(CellBase::CELL_TYPE::NONE);
				break;
			}
		}
	}

	for (auto& cell : cells_)
	{
		cell->Update();
		//cell->SetActive(cell->GetActionCount() == fieldManager_->GetCurrentActionCount());
	}
}

void Field::Draw(void)
{
	for (int i = 0; i < CELL_TOTAL; i++)
	{
		cells_[i]->Draw();
	}

	if (holdingCellType_ != CellBase::CELL_TYPE::NONE)
	{
		// マウスカーソルの位置を取得
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);
		// セルのサイズを取得
		int cellSize = CELL_SIZE;
		// セルの描画位置を計算
		int drawX = mouseX - cellSize / 2;
		int drawY = mouseY - cellSize / 2;
		// セルの色を取得
		unsigned int color = CellBase::CELL_NONE_COLOR;
		switch (holdingCellType_)
		{
		case CellBase::CELL_TYPE::ATTACK:
			color = CellBase::CELL_ATTACK_COLOR;
			break;
		case CellBase::CELL_TYPE::DEFENSE:
			color = CellBase::CELL_DEFENSE_COLOR;
			break;
		case CellBase::CELL_TYPE::BUFF:
			color = CellBase::CELL_BUFF_COLOR;
			break;
		case CellBase::CELL_TYPE::DEBUFF:
			color = CellBase::CELL_DEBUFF_COLOR;
			break;
		default:
			break;
		}
		// セルを描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 半透明に設定
		DrawBox(drawX, drawY, drawX + cellSize, drawY + cellSize, color, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す
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

bool Field::IsInFieldMouse(void) const
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	return (mouseX >= pos_.x && mouseX < pos_.x + CELL_NUM_X * CELL_SIZE &&
		mouseY >= pos_.y && mouseY < pos_.y + CELL_NUM_Y * CELL_SIZE);
}

void Field::SetCellType(CellBase::CELL_TYPE cellType)
{
	// マウスカーソルがある部分のセルのタイプを変更
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	for (auto& cell : cells_)
	{
		if (cell->IsMouseOver())
		{
			cell->SetType(cellType);
			break;
		}
	}
}

void Field::LoadFieldData(void)
{
	std::string filePath;
	
	// フィールドタイプに応じてCSVファイルのパスを設定
	switch (fieldType_)
	{
	case FIELD_TYPE::PLAYER:
		filePath = "Data/Csv/Field/Player.csv";
		break;
	case FIELD_TYPE::ENEMY:
		filePath = "Data/Csv/Field/Enemy.csv";
		break;
	case FIELD_TYPE::SELECT:
		filePath = "Data/Csv/Field/Select.csv";
		break;
	default:
		// フィールドタイプが不明な場合は何もしない
		return;
	}

	// CSVファイルを開く
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		// ファイルが開けない場合は何もしない
		return;
	}

	// CSVファイルを行ごとに読み込む
	std::string line;
	int y = 0;
	while (std::getline(file, line) && y < CELL_NUM_Y)
	{
		// 行をカンマ区切りでセルに分割
		std::stringstream ss(line);
		std::string cell;
		int x = 0;
		while (std::getline(ss, cell, ',') && x < CELL_NUM_X)
		{
			// セルタイプを取得（カンマ区切りの数値）
			int cellTypeValue = 0;
			std::stringstream cellStream(cell);
			cellStream >> cellTypeValue;

			// 数値をセルタイプに変換
			CellBase::CELL_TYPE cellType = static_cast<CellBase::CELL_TYPE>(cellTypeValue);
			
			// セルを生成
			std::unique_ptr<CellBase> cellObj = std::make_unique<CellBase>(
				this,
				cellType,
				Vector2(x * CELL_SIZE + pos_.x, y * CELL_SIZE + pos_.y),
				CELL_SIZE
			);
			cellObj->Init();
			cells_.push_back(std::move(cellObj));

			x++;
		}
		y++;
	}

	file.close();
}
