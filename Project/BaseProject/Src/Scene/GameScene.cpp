#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/SkyDome/SkyDome.h"
#include "../Object/Charactor/Player/Player.h"
#include "../Object/Charactor/Enemy/EnemyManger.h"
#include "../Object/FieldManager.h"
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

	// フィールド
	fieldManager_ = new FieldManager(this);
	fieldManager_->Init();

	//追従カメラ
	Camera* camera = sceMng_.GetCamera();
	camera->SetFollow(&player_->GetTransform());
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
	fieldManager_->Update();
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

	fieldManager_->Draw();
}

void GameScene::Release(void)
{
	// フィールド解放
	fieldManager_->Release();
	delete fieldManager_;
	// ステージ解放
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