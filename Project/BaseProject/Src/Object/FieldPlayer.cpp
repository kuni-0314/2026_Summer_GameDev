#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Object/FieldManager.h"
#include "../Object/Cell/CellBase.h"
#include "FieldPlayer.h"

FieldPlayer::FieldPlayer(FieldManager* fieldManager, int CELL_NUM_X, int CELL_NUM_Y, int CELL_SIZE, Vector2& pos)
	: FieldBase(fieldManager, CELL_NUM_X, CELL_NUM_Y, CELL_SIZE, pos)
{
}

FieldPlayer::~FieldPlayer(void)
{
}

void FieldPlayer::Init(void)
{
	// プレイヤー専用の初期化処理
	FieldBase::Init();
	// 追加のプレイヤー固有の初期化があればここに記述
}

void FieldPlayer::Update(void)
{
	// プレイヤー専用の更新処理
	FieldBase::Update();
	// 追加のプレイヤー固有の更新処理があればここに記述
}

void FieldPlayer::Draw(void)
{
	// プレイヤー専用の描画処理
	FieldBase::Draw();
	// 追加のプレイヤー固有の描画処理があればここに記述
}