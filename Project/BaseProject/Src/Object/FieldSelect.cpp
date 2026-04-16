#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "FieldSelect.h"

FieldSelect::FieldSelect(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	: FieldBase(fieldManager, CELL_NUM_X, CELL_NUM_Y, CELL_SIZE, pos)
{
}

FieldSelect::~FieldSelect(void)
{
}

void FieldSelect::Init(void)
{
	// セレクトフィールド専用の初期化処理
	FieldBase::Init();
	// 追加のセレクトフィールド固有の初期化があればここに記述
}

void FieldSelect::Update(void)
{
	// セレクトフィールド専用の更新処理
	FieldBase::Update();
	// 追加のセレクトフィールド固有の更新処理があればここに記述
}

void FieldSelect::Draw(void)
{
	// セレクトフィールド専用の描画処理
	FieldBase::Draw();
	// 追加のセレクトフィールド固有の描画処理があればここに記述
}