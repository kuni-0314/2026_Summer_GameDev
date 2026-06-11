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
#include "../Shader/PixelMaterial.h"
#include "../Shader/PixelRenderer.h"
#include "GameScene.h"
#include "../Manager/PostEffectManager.h"

GameScene::GameScene()
	: SceneBase(),
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	enemyManager_(nullptr),
	stageWall_(nullptr),
	itemManger_(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	stage_ = new Stage();
	stage_->Init();

	stageWall_ = std::make_unique<StageWall>();

	player_ = new Player();
	player_->Init();
	player_->SetGameScene(this);

	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(Stage::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	enemyManager_ = new EnemyManager(this,player_);
	enemyManager_->Init();
	enemyManager_->AddHitCollider(stageCollider);

	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	itemManger_ = new ItemManger();
	itemManger_->Init();
	itemManger_->AddHitCollider(stageCollider);

	sceMng_.GetCamera()->ChangeMode(Camera::MODE::MOUSE);
	Camera* camera = sceMng_.GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->SetTargetPos(enemyManager_->GetEnemyPos(1));

	// ポストエフェクトマネージャーの初期化
	PostEffectManager::GetInstance().Init();
	postEffectScreen_ = PostEffectManager::GetInstance().CreatePostEffectScreen();
	
	currentEffect_ = PostEffectManager::EffectType::VIGNETTE;
}

void GameScene::Update()
{
	auto const ins = InputManager::GetInstance();
	if (player_->GetHp() <= 0)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::OVER);
	}
	//bool end = enemyManager_->GetEnemyDead();
	//if (end)
	//{
	//	sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	//}

	stage_->Update();
	skyDome_->Update();
	player_->Update();
	enemyManager_->Update();
	itemManger_->Update();

	if (ins->IsTrgDown(KEY_INPUT_LEFT) && targetEnemyId_ > 0) targetEnemyId_--;
	if (ins->IsTrgDown(KEY_INPUT_RIGHT)) targetEnemyId_++;

	targetPos_ = enemyManager_->GetEnemyPos(targetEnemyId_);
	SceneManager::GetInstance().GetCamera()->SetTargetPos(targetPos_);

	if (ins->IsTrgDown(MOUSE_INPUT_LEFT)) {
		auto enemies = enemyManager_->GetEnemies();
		for (auto enemy : enemies) {
			if (!enemy->IsAlive()) continue;
			VECTOR playerPos = player_->GetTransform().pos;
			VECTOR enemyPos = enemy->GetTransform().pos;
			float dist = VSize(VSub(playerPos, enemyPos));
			if (dist < 300.0f) {
				enemy->Damege(99999);
			}
		}
	}

	for (int i = 0; i < attackColliders_.size(); i++)
	{
		auto data = attackColliders_[i];
		if (data->collider != nullptr)
		{
			data->lifeTime--;
			
			ColliderSphere* sphere = dynamic_cast<ColliderSphere*>(data->collider);
			if (sphere != nullptr)
			{
				Transform* transform = const_cast<Transform*>(data->collider->GetFollow());
				transform->pos = player_->GetTransform().pos;
				
				auto enemies = enemyManager_->GetEnemies();
				for (auto enemy : enemies)
				{
					if (!enemy->IsAlive()) continue;
					
					VECTOR enemyPos = enemy->GetTransform().pos;
					VECTOR spherePos = sphere->GetPos();
					float distance = VSize(VSub(enemyPos, spherePos));
					
					if (distance < sphere->GetRadius())
					{
						enemy->Damege(static_cast<int>(data->damage));
					}
				}
			}
			
			if (data->lifeTime <= 0)
			{
				enemyManager_->ClearAttackColliders();
				
				delete data->collider->GetFollow();
				delete data->collider;
				delete data;
				attackColliders_.erase(attackColliders_.begin() + i);
				i--;
			}
			else
			{
				Transform* transform = const_cast<Transform*>(data->collider->GetFollow());
				transform->pos = player_->GetTransform().pos;
			}
		}
	}

	//if (ins->IsTrgDown(KEY_INPUT_NUMPAD4))
	//{
	//	currentEffect_ = (currentEffect_ - 1 + EFFECT::MAX) % EFFECT::MAX;
	//}
	//if (ins->IsTrgDown(KEY_INPUT_NUMPAD6))
	//{
	//	currentEffect_ = (currentEffect_ + 1) % EFFECT::MAX;
	//}
}

void GameScene::Draw()
{
	a_++;
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	// 3D描画
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	itemManger_->Draw();
	enemyManager_->Draw();

	// ポストエフェクト適用
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);

	// マネージャーを使ってエフェクトを適用
	PostEffectManager::GetInstance().ApplyEffect(currentEffect_, tempScreen, postEffectScreen_, effectTime_);

	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	// デバッグ表示
	DrawFormatString(10, 10, 0xFFFF00, "Effect: %d", static_cast<int>(currentEffect_));
	
	DeleteGraph(tempScreen);
}

void GameScene::Release()
{
	if (postEffectScreen_ != -1)
	{
		DeleteGraph(postEffectScreen_);
		postEffectScreen_ = -1;
	}

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

	if (itemManger_ != nullptr)
	{
		itemManger_->Release();
		delete itemManger_;
		itemManger_ = nullptr;
	}

	if (enemyManager_ != nullptr)
	{
		enemyManager_->Release();
		delete enemyManager_;
		enemyManager_ = nullptr;
	}

	stage_->Release();
	delete stage_;

	stageWall_->Release();

	skyDome_->Release();
	delete skyDome_;

	player_->Release();
	delete player_;
}

void GameScene::CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float Damage, int lifeTime)
{
	Transform* transform = new Transform();
	transform->pos = pos;
	
	ColliderBase* collider = new ColliderSphere(tag, transform, pos, radius);
	
	AttackColliderData* data = new AttackColliderData();
	data->collider = collider;
	data->damage = Damage;
	data->lifeTime = lifeTime;
	
	attackColliders_.push_back(data);
	
	enemyManager_->AddAttackCollider(collider);
}

void GameScene::SetEffectParameters(int effectType, FLOAT4& bufs, float time)
{
	switch (effectType)
	{
	case NORMAL:
		bufs.x = time;
		break;
	case MONO:
		break;
	case SEPIA:
		break;
	case INVERT:
		break;
	case MOSAIC:
		bufs.x = 16.0f;
		bufs.y = 10.0f;
		break;
	case CHROM_ABR:
		bufs.x = 10.0f;
		break;
	case VIGNETTE:
		bufs.x = 0.6f;
		bufs.y = 0.7f;
		break;
	case SCANLINE:
		bufs.x = 100.0f;
		bufs.y = 0.3f;
		break;
	case POSTERIZE:
		bufs.x = 4.0f;
		break;
	case GLITCH:
		bufs.x = time;
		break;
	case EMBOSS:
		bufs.x = 0.001f;
		bufs.y = 0.001f;
		break;
	case RETROWAVE:
		bufs.x = 0.5f;
		break;
	case BLOOM:
		bufs.x = 0.6f;
		bufs.y = 0.8f;
		break;
	case RIPPLE:
		bufs.x = time;
		bufs.y = 2.0f;
		bufs.z = 1.5f;
		break;
	case RGB_SPLIT:
		bufs.x = 0.025f;
		bufs.y = 0.05f;
		bufs.z = 0.025f;
		break;
	case PIXELATE:
		bufs.x = 0.002f;
		break;
	case SWIRL:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case RADIAL_BLUR:
		bufs.x = -0.1f;
		break;
	case HUE_SHIFT:
		bufs.x = time;
		break;
	case WAVE:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 2.0f;
		break;
	case EDGE_DETECT:
		bufs.x = 0.15f;
		break;
	case OLD_FILM:
		bufs.x = time;
		break;
	case NIGHT_VISION:
		bufs.x = time;
		break;
	case LIQUID_DIST:
		bufs.x = time;
		bufs.y = 0.1f;
		bufs.z = 3.0f;
		break;
	case PINHOLE:
		bufs.x = 0.5f;
		bufs.y = 2.0f;
		break;
	case SPEED_LINES:
		bufs.x = time;
		bufs.y = 100.0f;
		bufs.z = 100.0f;
		break;
	case FROSTED_GLASS:
		bufs.x = 1.0f;
		bufs.y = 1.0f;
		break;
	case DOT_MATRIX:
		bufs.x = 100.0f;
		bufs.y = 1.0f;
		break;
	case DEPTH_FOG:
		bufs.x = 0.0f;
		bufs.y = 1.0f;
		bufs.z = 1.0f;
		break;
	case DIGITAL_RAIN:
		bufs.x = time;
		bufs.y = 3.0f;
		bufs.z = 64.0f;
		break;
	case STROBE:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 2.0f;
		break;
	case SNOW_STORM:
		bufs.x = time;
		bufs.y = 40.0f;
		bufs.z = -1.0f;
		break;
	case SCREEN_SHAKE:
		bufs.x = time;
		bufs.y = 0.01f;
		bufs.z = 0.01f;
		bufs.w = 0.01f;
		break;
	case CRT:
		bufs.x = time;
		bufs.y = 1.0f;
		bufs.z = 0.5f;
		bufs.w = 0.25f;
		break;
	default:
		break;
	}
}