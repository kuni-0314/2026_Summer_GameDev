#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/SkyDome/SkyDome.h"
#include "../Object/Charactor/Player/Player.h"
#include "../Object/Charactor/Enemy/EnemyManger.h"
#include "../Object/CellBase.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	enemyManager_(nullptr)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//ステージ
	stage_ = new Stage();
	stage_->Init();



	//プレイヤー
	player_ = new Player();
	player_->Init();

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);
	

	//エネミーー
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();
	enemyManager_->AddHitCollider(stageCollider);
	
	//プレイヤーのカプセルコライダ―をエネミーに登録
	enemyManager_->AddHitCollider(
		player_->GetOwnCollider(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)));
	

	//スカイドーム
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	//追従カメラ
	Camera* camera = sceMng_.GetCamera();
	camera->SetFollow(&player_->GetTransform());//参照型でプレイヤ-のポインタを持ってくる
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	camera->AddHitCollider(stageCollider);

	// セル
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

void GameScene::Update(void)
{

	// シーン遷移
	auto const& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	stage_->Update();//ステージ更新
	skyDome_->Update();//スカイドーム更新
	player_->Update();
	enemyManager_->Update();

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
	actionTimer_ += sceMng_.GetDeltaTime();
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

void GameScene::Draw(void)
{
	skyDome_->Draw();	//スカイドーム描画

	stage_->Draw();		//ステージ描画

	player_->Draw();	//プレイヤー描画

	enemyManager_->Draw();

	// 黒を描画（少し透過）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


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

void GameScene::Release(void)
{
	//解放処理
	stage_->Release();
	delete stage_;
	//スカイドーム解放
	skyDome_->Release();
	delete skyDome_;
	//プレイヤー解放
	player_->Release();
	delete player_;

	enemyManager_->Release();
	delete enemyManager_;
}
