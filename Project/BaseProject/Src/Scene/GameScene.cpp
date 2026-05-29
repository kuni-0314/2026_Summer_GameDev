#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/Stage/StageWall.h"
#include "../Object/Actor/SkyDome/SkyDome.h"
#include "../Object/Charactor/Player/Player.h"
#include "../Object/Charactor/Enemy/EnemyManger.h"
#include "../Object/Item/ItemManger.h"
#include "../Object/FieldManager.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Object/Collider/Sphere/ColliderSphere.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	enemyManager_(nullptr),
	stageWall_(nullptr),
	itemManger_(nullptr)
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

	stageWall_ = std::make_unique<StageWall>();
	//stageWall_->Init();

	//プレイヤー
	player_ = new Player();
	player_->Init();
	player_->SetGameScene(this);

	// ステージモデルのコライダーをプレイヤーに登録
	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);
	

	//エネミーー
	enemyManager_ = new EnemyManager(this,player_);
	enemyManager_->Init();
	enemyManager_->AddHitCollider(stageCollider);

	
	//プレイヤーのカプセルコライダ―をエネミーに登録
	//enemyManager_->AddHitCollider(
	//	player_->GetOwnCollider(static_cast<int>(CharactorBase::COLLIDER_TYPE::CAPSULE)));
	
	//スカイドーム
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	// フィールド
	//fieldManager_ = new FieldManager(this);
	//fieldManager_->Init();

	//アイテムマネージャー
	itemManger_ = new ItemManger();
	itemManger_->Init();
	itemManger_->AddHitCollider(stageCollider);

	//追従カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	Camera* camera = sceMng_.GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->SetTargetPos(enemyManager_->GetEnemyPos(1));
}

void GameScene::Update(void)
{
	// シーン遷移
	auto const ins = InputManager::GetInstance();
	if (player_->GetHp() <= 0)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::OVER);
	}
	bool end = enemyManager_->GetEnemyDead();
	if (end)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}

	stage_->Update();//ステージ更新
	//stageWall_->Update();//ステージ壁更新
	skyDome_->Update();//スカイドーム更新
	player_->Update();
	enemyManager_->Update();
	//fieldManager_->Update();
	itemManger_->Update();

	if (ins->IsTrgDown(KEY_INPUT_LEFT) && targetEnemyId_ > 0) targetEnemyId_--;
	if (ins->IsTrgDown(KEY_INPUT_RIGHT)) targetEnemyId_++;

	targetPos_ = enemyManager_->GetEnemyPos(targetEnemyId_);
	SceneManager::GetInstance().GetCamera()->SetTargetPos(targetPos_);

	// 攻撃コライダの更新と寿命管理
	for (int i = 0; i < attackColliders_.size(); i++)
	{
		auto data = attackColliders_[i];
		if (data->collider != nullptr)
		{
			data->lifeTime--;
			
			// 寿命が尽きたらコライダを削除
			if (data->lifeTime <= 0)
			{
				// エネミーマネージャーから攻撃コライダを削除
				enemyManager_->ClearAttackColliders();
				
				delete data->collider->GetFollow();
				delete data->collider;
				delete data;
				attackColliders_.erase(attackColliders_.begin() + i);
				i--;
			}
			else
			{
				// プレイヤーの位置に追従
				Transform* transform = const_cast<Transform*>(data->collider->GetFollow());
				transform->pos = player_->GetTransform().pos;
			}
		}
	}
}

void GameScene::Draw(void)
{
	skyDome_->Draw();	//スカイドーム描画

	stage_->Draw();		//ステージ描画
	//stageWall_->Draw();	//ステージ壁描画
	player_->Draw();	//プレイヤー描画
	itemManger_->Draw();	//アイテム描画

	enemyManager_->Draw();
	//VECTOR enemyPos = enemyManager_->GetNearEnemyPos(player_->GetTransform().pos);
	//VECTOR targetPos = enemyManager_->GetEnemyPos(targetEnemyId_);
	//DrawSphere3D(targetPos, 40.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	// 黒を描画（少し透過）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//fieldManager_->Draw();

	// 攻撃コライダのデバッグ表示
	for (auto data : attackColliders_)
	{
		if (data->collider != nullptr)
		{
			VECTOR pos = data->collider->GetFollow()->pos;
			ColliderSphere* sphere = dynamic_cast<ColliderSphere*>(data->collider);
			if (sphere != nullptr)
			{
				float radius = sphere->GetRadius();
				DrawSphere3D(pos, radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
			}
		}
	}
}

void GameScene::Release(void)
{
	// 攻撃用コライダの解放
	for (auto data : attackColliders_)
	{
		if (data->collider != nullptr)
		{
			delete data->collider->GetFollow();
			delete data->collider;
			data->collider = nullptr;
		}
		delete data;
	}
	attackColliders_.clear();

	// アイテムマネージャー解放
	if (itemManger_ != nullptr)
	{
		itemManger_->Release();
		delete itemManger_;
		itemManger_ = nullptr;
	}

	// エネミーマネージャー解放
	if (enemyManager_ != nullptr)
	{
		enemyManager_->Release();
		delete enemyManager_;
		enemyManager_ = nullptr;
	}

	// フィールド解放
	//fieldManager_->Release();
	//delete fieldManager_;
	// ステージ解放
	stage_->Release();
	delete stage_;

	//ステージ壁解放
	stageWall_->Release();

	//スカイドーム解放
	skyDome_->Release();
	delete skyDome_;
	//プレイヤー解放
	player_->Release();
	delete player_;

	
}

void GameScene::CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float Damage, int lifeTime)
{
	// 新しいTransformを作成
	Transform* transform = new Transform();
	transform->pos = pos;
	
	// 球体コライダを作成
	ColliderBase* collider = new ColliderSphere(tag, transform, pos, radius);
	
	// 攻撃データを作成
	AttackColliderData* data = new AttackColliderData();
	data->collider = collider;
	data->damage = Damage;
	data->lifeTime = lifeTime;
	
	// リストに追加
	attackColliders_.push_back(data);
	
	// エネミーマネージャーに攻撃コライダを登録
	enemyManager_->AddAttackCollider(collider);
}
