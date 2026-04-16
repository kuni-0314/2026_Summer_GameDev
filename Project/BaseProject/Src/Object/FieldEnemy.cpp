#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "FieldEnemy.h"

FieldEnemy::FieldEnemy(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	: FieldBase(fieldManager, CELL_NUM_X, CELL_NUM_Y, CELL_SIZE, pos)
{
}

FieldEnemy::~FieldEnemy(void)
{
}

void FieldEnemy::Init(void)
{
	// エネミー専用の初期化処理
	FieldBase::Init();
	// 追加のエネミー固有の初期化があればここに記述
}

void FieldEnemy::Update(void)
{
	// エネミー専用の更新処理
	FieldBase::Update();
	// 追加のエネミー固有の更新処理があればここに記述
}

void FieldEnemy::Draw(void)
{
	// エネミー専用の描画処理
	FieldBase::Draw();
	// 追加のエネミー固有の描画処理があればここに記述
}