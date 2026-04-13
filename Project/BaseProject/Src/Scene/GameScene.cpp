#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/SkyDome/SkyDome.h"
#include "../Object/Charactor/Player/Player.h"
#include "../Object/Charactor/Enemy/EnemyManger.h"
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

	static int currentActionCount = 0;
	static int time = 0;
	if (time % 30 == 0) currentActionCount++;
	time++;

	unsigned int color = 0;
	short cnt = 0;
	
	// 総行動回数
	int playerTotalActionCount = PLAYER_FIELD_CELL_X * PLAYER_FIELD_CELL_Y;
	int enemyTotalActionCount = ENEMY_FIELD_CELL_X * ENEMY_FIELD_CELL_Y;
	int totalActionCount = playerTotalActionCount + enemyTotalActionCount;

	bool isFill = false;

	// プレイヤーと敵のセルを交互に描画
	int maxCells = (playerTotalActionCount > enemyTotalActionCount) ? playerTotalActionCount : enemyTotalActionCount;

	for (int i = 0; i < maxCells; i++)
	{
		if (currentActionCount >= totalActionCount)
		{
			currentActionCount = 0; // 行動回数が総行動回数を超えたらリセット
		}
		else if (currentActionCount == cnt)
		{
			isFill = true;
		}
		else
		{
			isFill = false;
		}

		// プレイヤーのセル
		if (i < playerTotalActionCount)
		{
			int y = i / PLAYER_FIELD_CELL_X;
			int x = i % PLAYER_FIELD_CELL_X;

			switch (playerField_[y][x])
			{
			case CELL_TYPE::NONE:
				color = GetColor(255, 255, 255);
				break;
			case CELL_TYPE::ATTACK:
				color = GetColor(255, 0, 0);
				break;
			case CELL_TYPE::DEFENSE:
				color = GetColor(0, 0, 255);
				break;
			case CELL_TYPE::BUFF:
				color = GetColor(0, 255, 0);
				break;
			case CELL_TYPE::DEBUFF:
				color = GetColor(255, 255, 0);
				break;
			}
			cnt++;
			// 仮でフィールドを正方形で前面に枠のみ描画
			DrawBox(
				x * PLAYER_FIELD_CELL_SIZE,
				y * PLAYER_FIELD_CELL_SIZE,
				(x + 1) * PLAYER_FIELD_CELL_SIZE,
				(y + 1) * PLAYER_FIELD_CELL_SIZE,
				color,
				isFill
			);
			DrawFormatString(
				x * PLAYER_FIELD_CELL_SIZE + 10,
				y * PLAYER_FIELD_CELL_SIZE + 10,
				color,
				"%d",
				cnt
			);
		}

		if (currentActionCount >= totalActionCount)
		{
			currentActionCount = 0; // 行動回数が総行動回数を超えたらリセット
		}
		else if (currentActionCount == cnt)
		{
			isFill = true;
		}
		else
		{
			isFill = false;
		}

		// 敵のセル
		if (i < enemyTotalActionCount)
		{
			int y = i / ENEMY_FIELD_CELL_X;
			int x = i % ENEMY_FIELD_CELL_X;

			switch (enemyField_[y][x])
			{
			case CELL_TYPE::NONE:
				color = GetColor(255, 255, 255);
				break;
			case CELL_TYPE::ATTACK:
				color = GetColor(255, 0, 0);
				break;
			case CELL_TYPE::DEFENSE:
				color = GetColor(0, 0, 255);
				break;
			case CELL_TYPE::BUFF:
				color = GetColor(0, 255, 0);
				break;
			case CELL_TYPE::DEBUFF:
				color = GetColor(255, 255, 0);
				break;
			}
			cnt++;
			// 仮でフィールドを正方形で前面に枠のみ描画
			DrawBox(
				x * ENEMY_FIELD_CELL_SIZE + Application::SCREEN_SIZE_X / 2,
				y * ENEMY_FIELD_CELL_SIZE,
				(x + 1) * ENEMY_FIELD_CELL_SIZE + Application::SCREEN_SIZE_X / 2,
				(y + 1) * ENEMY_FIELD_CELL_SIZE,
				color,
				isFill
			);
			DrawFormatString(
				x * ENEMY_FIELD_CELL_SIZE + Application::SCREEN_SIZE_X / 2 + 10,
				y * ENEMY_FIELD_CELL_SIZE + 10,
				color,
				"%d",
				cnt
			);
		}
	}

	cnt = 0;
	for (int y = 0; y < SELECT_FIELD_CELL_Y; y++)
	{
		for (int x = 0; x < SELECT_FIELD_CELL_X; x++)
		{
			switch (selectField_[y][x])
			{
			case CELL_TYPE::NONE:
				color = GetColor(255, 255, 255);
				break;
			case CELL_TYPE::ATTACK:
				color = GetColor(255, 0, 0);
				break;
			case CELL_TYPE::DEFENSE:
				color = GetColor(0, 0, 255);
				break;
			case CELL_TYPE::BUFF:
				color = GetColor(0, 255, 0);
				break;
			case CELL_TYPE::DEBUFF:
				color = GetColor(255, 255, 0);
				break;
			}
			// 仮でフィールドを正方形で前面に枠のみ描画（下側に配置）
			cnt++;
			DrawBox(
				x * SELECT_FIELD_CELL_SIZE,
				y * SELECT_FIELD_CELL_SIZE + Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE,
				(x + 1) * SELECT_FIELD_CELL_SIZE,
				(y + 1) * SELECT_FIELD_CELL_SIZE + Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE,
				color,
				false
			);
			DrawFormatString(
				x * SELECT_FIELD_CELL_SIZE + 10,
				y * SELECT_FIELD_CELL_SIZE + 10 + Application::SCREEN_SIZE_Y - SELECT_FIELD_CELL_Y * SELECT_FIELD_CELL_SIZE,
				color,
				"%d",
				cnt
			);
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
