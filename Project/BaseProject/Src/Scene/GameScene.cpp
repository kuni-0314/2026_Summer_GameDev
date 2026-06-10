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

GameScene::GameScene(void)
	:
	SceneBase(),
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	enemyManager_(nullptr),
	stageWall_(nullptr),
	itemManger_(nullptr),
	currentEffect_(0),
	targetEnemyId_(0),
	postEffectScreen_(-1)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
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

	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// マテリアルとレンダラーを作成
		// フルスクリーン用の頂点を生成
		// 通常
	materials_[NORMAL] = std::make_unique<PixelMaterial>("Normal.cso", 1);
	renderers_[NORMAL] = std::make_unique<PixelRenderer>(*materials_[NORMAL]);
	renderers_[NORMAL]->MakeSquareVertex();

	// モノクロ
	materials_[MONO] = std::make_unique<PixelMaterial>("Monotone.cso", 1);
	renderers_[MONO] = std::make_unique<PixelRenderer>(*materials_[MONO]);
	renderers_[MONO]->MakeSquareVertex();

	// セピア
	materials_[SEPIA] = std::make_unique<PixelMaterial>("Sepia.cso", 1);
	renderers_[SEPIA] = std::make_unique<PixelRenderer>(*materials_[SEPIA]);
	renderers_[SEPIA]->MakeSquareVertex();

	// ネガポジ反転
	materials_[INVERT] = std::make_unique<PixelMaterial>("Invert.cso", 1);
	renderers_[INVERT] = std::make_unique<PixelRenderer>(*materials_[INVERT]);
	renderers_[INVERT]->MakeSquareVertex();

	// モザイク
	materials_[MOSAIC] = std::make_unique<PixelMaterial>("Mosaic.cso", 1);
	renderers_[MOSAIC] = std::make_unique<PixelRenderer>(*materials_[MOSAIC]);
	renderers_[MOSAIC]->MakeSquareVertex();

	// 色収差
	materials_[CHROM_ABR] = std::make_unique<PixelMaterial>("ChromaticAberration.cso", 1);
	renderers_[CHROM_ABR] = std::make_unique<PixelRenderer>(*materials_[CHROM_ABR]);
	renderers_[CHROM_ABR]->MakeSquareVertex();

	// ビネット
	materials_[VIGNETTE] = std::make_unique<PixelMaterial>("Vignette.cso", 1);
	renderers_[VIGNETTE] = std::make_unique<PixelRenderer>(*materials_[VIGNETTE]);
	renderers_[VIGNETTE]->MakeSquareVertex();

	// 走査線
	materials_[SCANLINE] = std::make_unique<PixelMaterial>("Scanline.cso", 1);
	renderers_[SCANLINE] = std::make_unique<PixelRenderer>(*materials_[SCANLINE]);
	renderers_[SCANLINE]->MakeSquareVertex();

	// ポスタライズ
	materials_[POSTERIZE] = std::make_unique<PixelMaterial>("Posterize.cso", 1);
	renderers_[POSTERIZE] = std::make_unique<PixelRenderer>(*materials_[POSTERIZE]);
	renderers_[POSTERIZE]->MakeSquareVertex();

	// グリッチ
	materials_[GLITCH] = std::make_unique<PixelMaterial>("Glitch.cso", 1);
	renderers_[GLITCH] = std::make_unique<PixelRenderer>(*materials_[GLITCH]);
	renderers_[GLITCH]->MakeSquareVertex();

	// エンボス
	materials_[EMBOSS] = std::make_unique<PixelMaterial>("Emboss.cso", 1);
	renderers_[EMBOSS] = std::make_unique<PixelRenderer>(*materials_[EMBOSS]);
	renderers_[EMBOSS]->MakeSquareVertex();

	// レトロウェーブ
	materials_[RETROWAVE] = std::make_unique<PixelMaterial>("RetroWave.cso", 1);
	renderers_[RETROWAVE] = std::make_unique<PixelRenderer>(*materials_[RETROWAVE]);
	renderers_[RETROWAVE]->MakeSquareVertex();

	// ブルーム
	materials_[BLOOM] = std::make_unique<PixelMaterial>("Bloom.cso", 1);
	renderers_[BLOOM] = std::make_unique<PixelRenderer>(*materials_[BLOOM]);
	renderers_[BLOOM]->MakeSquareVertex();

	// リップル
	materials_[RIPPLE] = std::make_unique<PixelMaterial>("Ripple.cso", 1);
	renderers_[RIPPLE] = std::make_unique<PixelRenderer>(*materials_[RIPPLE]);
	renderers_[RIPPLE]->MakeSquareVertex();

	// RGB分解
	materials_[RGB_SPLIT] = std::make_unique<PixelMaterial>("RGBSplit.cso", 1);
	renderers_[RGB_SPLIT] = std::make_unique<PixelRenderer>(*materials_[RGB_SPLIT]);
	renderers_[RGB_SPLIT]->MakeSquareVertex();

	// ピクセル化
	materials_[PIXELATE] = std::make_unique<PixelMaterial>("Pixelate.cso", 1);
	renderers_[PIXELATE] = std::make_unique<PixelRenderer>(*materials_[PIXELATE]);
	renderers_[PIXELATE]->MakeSquareVertex();

	// 渦巻き
	materials_[SWIRL] = std::make_unique<PixelMaterial>("Swirl.cso", 1);
	renderers_[SWIRL] = std::make_unique<PixelRenderer>(*materials_[SWIRL]);
	renderers_[SWIRL]->MakeSquareVertex();

	// 放射状ブラー
	materials_[RADIAL_BLUR] = std::make_unique<PixelMaterial>("RadialBlur.cso", 1);
	renderers_[RADIAL_BLUR] = std::make_unique<PixelRenderer>(*materials_[RADIAL_BLUR]);
	renderers_[RADIAL_BLUR]->MakeSquareVertex();

	// 色相シフト
	materials_[HUE_SHIFT] = std::make_unique<PixelMaterial>("HueShift.cso", 1);
	renderers_[HUE_SHIFT] = std::make_unique<PixelRenderer>(*materials_[HUE_SHIFT]);
	renderers_[HUE_SHIFT]->MakeSquareVertex();

	// 波
	materials_[WAVE] = std::make_unique<PixelMaterial>("Wave.cso", 1);
	renderers_[WAVE] = std::make_unique<PixelRenderer>(*materials_[WAVE]);
	renderers_[WAVE]->MakeSquareVertex();

	// エッジ検出
	materials_[EDGE_DETECT] = std::make_unique<PixelMaterial>("EdgeDetection.cso", 1);
	renderers_[EDGE_DETECT] = std::make_unique<PixelRenderer>(*materials_[EDGE_DETECT]);
	renderers_[EDGE_DETECT]->MakeSquareVertex();

	// オールドフィルム
	materials_[OLD_FILM] = std::make_unique<PixelMaterial>("OldFilm.cso", 1);
	renderers_[OLD_FILM] = std::make_unique<PixelRenderer>(*materials_[OLD_FILM]);
	renderers_[OLD_FILM]->MakeSquareVertex();

	// ナイトビジョン
	materials_[NIGHT_VISION] = std::make_unique<PixelMaterial>("NightVision.cso", 1);
	renderers_[NIGHT_VISION] = std::make_unique<PixelRenderer>(*materials_[NIGHT_VISION]);
	renderers_[NIGHT_VISION]->MakeSquareVertex();

	// 液体歪み
	materials_[LIQUID_DIST] = std::make_unique<PixelMaterial>("LiquidDistortion.cso", 1);
	renderers_[LIQUID_DIST] = std::make_unique<PixelRenderer>(*materials_[LIQUID_DIST]);
	renderers_[LIQUID_DIST]->MakeSquareVertex();

	// ピンホール
	materials_[PINHOLE] = std::make_unique<PixelMaterial>("Pinhole.cso", 1);
	renderers_[PINHOLE] = std::make_unique<PixelRenderer>(*materials_[PINHOLE]);
	renderers_[PINHOLE]->MakeSquareVertex();

	// スピードライン
	materials_[SPEED_LINES] = std::make_unique<PixelMaterial>("SpeedLines.cso", 1);
	renderers_[SPEED_LINES] = std::make_unique<PixelRenderer>(*materials_[SPEED_LINES]);
	renderers_[SPEED_LINES]->MakeSquareVertex();

	// フロストガラス
	materials_[FROSTED_GLASS] = std::make_unique<PixelMaterial>("FrostedGlass.cso", 1);
	renderers_[FROSTED_GLASS] = std::make_unique<PixelRenderer>(*materials_[FROSTED_GLASS]);
	renderers_[FROSTED_GLASS]->MakeSquareVertex();

	// ドットマトリクス
	materials_[DOT_MATRIX] = std::make_unique<PixelMaterial>("DotMatrix.cso", 1);
	renderers_[DOT_MATRIX] = std::make_unique<PixelRenderer>(*materials_[DOT_MATRIX]);
	renderers_[DOT_MATRIX]->MakeSquareVertex();

	// 深度フォグ
	materials_[DEPTH_FOG] = std::make_unique<PixelMaterial>("DepthFog.cso", 1);
	renderers_[DEPTH_FOG] = std::make_unique<PixelRenderer>(*materials_[DEPTH_FOG]);
	renderers_[DEPTH_FOG]->MakeSquareVertex();

	// デジタル雨
	materials_[DIGITAL_RAIN] = std::make_unique<PixelMaterial>("DigitalRain.cso", 1);
	renderers_[DIGITAL_RAIN] = std::make_unique<PixelRenderer>(*materials_[DIGITAL_RAIN]);
	renderers_[DIGITAL_RAIN]->MakeSquareVertex();

	// ストロボ
	materials_[STROBE] = std::make_unique<PixelMaterial>("Strobe.cso", 1);
	renderers_[STROBE] = std::make_unique<PixelRenderer>(*materials_[STROBE]);
	renderers_[STROBE]->MakeSquareVertex();

	// 雪嵐
	materials_[SNOW_STORM] = std::make_unique<PixelMaterial>("SnowStorm.cso", 1);
	renderers_[SNOW_STORM] = std::make_unique<PixelRenderer>(*materials_[SNOW_STORM]);
	renderers_[SNOW_STORM]->MakeSquareVertex();

	// 画面揺れ
	materials_[SCREEN_SHAKE] = std::make_unique<PixelMaterial>("ScreenShake.cso", 1);
	renderers_[SCREEN_SHAKE] = std::make_unique<PixelRenderer>(*materials_[SCREEN_SHAKE]);
	renderers_[SCREEN_SHAKE]->MakeSquareVertex();

	// CRT
	materials_[CRT] = std::make_unique<PixelMaterial>("CRT.cso", 1);
	renderers_[CRT] = std::make_unique<PixelRenderer>(*materials_[CRT]);
	renderers_[CRT]->MakeSquareVertex();
	
	currentEffect_ = MONO;
}

void GameScene::Update(void)
{
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

	if (ins->IsTrgDown(KEY_INPUT_NUMPAD4))
	{
		currentEffect_ = (currentEffect_ - 1 + EFFECT::MAX) % EFFECT::MAX;
	}
	if (ins->IsTrgDown(KEY_INPUT_NUMPAD6))
	{
		currentEffect_ = (currentEffect_ + 1) % EFFECT::MAX;
	}
}

void GameScene::Draw(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	
	// ステップ1: 3D描画（mainScreenに描画される）
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	itemManger_->Draw();
	enemyManager_->Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// この時点でmainScreenには3D描画が完了している
	// （ただし、camera_->DrawDebug()やfader_->Draw()はまだ描画されていない）
	
	// ステップ2: mainScreenの内容を一時的に保存
	// 新しいスクリーンを作成して、mainScreenの内容をコピー
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	
	// mainScreenの内容をtempScreenにコピー
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);
	
	// ステップ3: ポストエフェクト処理
	SetDrawScreen(postEffectScreen_);
	ClearDrawScreen();

	materials_[currentEffect_]->Begin();
	materials_[currentEffect_]->SetTexture(0, tempScreen);

	FLOAT4* constBufsPtr = materials_[currentEffect_]->GetConstantBuffer();
	static float time = 0.0f;
	time += SceneManager::GetInstance().GetDeltaTime();

	ApplyEffect(currentEffect_, mainScreen, postEffectScreen_, time);
	
	// ステップ4: ポストエフェクト結果をmainScreenに描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);
	
	DrawFormatString(10, 10, 0xFFFF00, "=== SHADER DEBUG INFO ===");
	DrawFormatString(10, 30, 0xFF0000, "currentEffect: %d", currentEffect_);
	DrawFormatString(10, 50, 0xFF0000, "postEffectScreen: %d", postEffectScreen_);
	DrawFormatString(10, 70, 0xFF0000, "shader handle: %d", materials_[currentEffect_]->shader_);
	DrawFormatString(10, 90, 0xFF0000, "constBuf: %d", materials_[currentEffect_]->constBuf_);
	DrawFormatString(10, 110, 0xFFFFFF, "mainScreen: %d", mainScreen);
	DrawFormatString(10, 130, 0xFFFFFF, "tempScreen: %d", tempScreen);


	// ステップ5: 一時スクリーンを削除
	DeleteGraph(tempScreen);
	
	// この後、SceneManager::Draw()に戻り、
	// camera_->DrawDebug()とfader_->Draw()がmainScreenに描画される
}

void GameScene::Release(void)
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

void GameScene::ApplyEffect(int effectType, int srcScreen, int dstScreen, float time)
{
	SetDrawScreen(dstScreen);
	ClearDrawScreen();

	materials_[effectType]->Begin();
	materials_[effectType]->SetTexture(0, srcScreen);

	FLOAT4* constBufsPtr = materials_[effectType]->GetConstantBuffer();
	FLOAT4 bufs = {};
	SetEffectParameters(effectType, bufs, time);

	constBufsPtr->x = bufs.x;
	constBufsPtr->y = bufs.y;
	constBufsPtr->z = bufs.z;
	constBufsPtr->w = bufs.w;

	materials_[effectType]->UpdateConstantBuffer(CONSTANT_BUF_SLOT_BEGIN_PS);
	renderers_[effectType]->Draw();
	materials_[effectType]->SetTexture(0, -1);
	materials_[effectType]->End();
}