#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Manager/PostEffectManager.h"
#include "../Manager/ResourceManager.h"
#include "../Sound/AudioManager.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/Stage/StageWall.h"
#include "../Object/Actor/SkyDome/SkyDome.h"
#include "../Object/Charactor/Player/Player.h"
#include "../Object/Charactor/Enemy/EnemyManger.h"
#include "../Object/Item/ItemManger.h"
#include "../Object/Weapon/Sword/KeyBlade1.h"
#include "../Object/Collider/ColliderBase.h"
#include "../Object/Collider/Sphere/ColliderSphere.h"
#include "GameScene.h"
// 別プロジェクト
GameScene::GameScene()
	: SceneBase(),
	stage_(nullptr),
	skyDome_(nullptr),
	player_(nullptr),
	enemyManager_(nullptr),
	stageWall_(nullptr),
	itemManger_(nullptr),
	postEffectScreen_(-1),
	currentEffect_(PostEffectManager::EFFECT_TYPE::NORMAL),
	multiEffectMode_(false),
	effectTime_(0.0f),
	targetEnemyId_(0)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	stageWall_ = std::make_unique<StageWall>();

	// プレイヤー初期化
	player_ = new Player(0);	// 何かしら定数作って
	player_->Init();
	player_->SetGameScene(this);

	const ColliderBase* stageCollider =
		stage_->GetOwnCollider(static_cast<int>(ActorBase::COLLIDER_TYPE::MODEL));
	player_->AddHitCollider(stageCollider);

	// 武器初期化
	//weapon_ = new KeyBlade1(player_->GetTransform());
	//weapon_->Init();

	// 敵マネージャー初期化
	enemyManager_ = new EnemyManager(this, player_);
	enemyManager_->Init();
	enemyManager_->AddHitCollider(stageCollider);
	enemyManager_->AddHitCollider(player_->GetSword()->GetOwnCollider(static_cast<int>(ActorBase::COLLIDER_TYPE::CAPSULE)));


	// スカイドーム初期化
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	// アイテムマネージャー初期化
	itemManger_ = new ItemManger();
	itemManger_->Init();
	itemManger_->AddHitCollider(stageCollider);

	// カメラ設定
	Camera* camera = sceMng_.GetCamera();
	camera->SetFollow(&player_->GetTransform());
	camera->AddHitCollider(stageCollider);
	camera->SetTargetPos(enemyManager_->GetEnemyPos(1));

	// ポストエフェクトマネージャーの初期化
	PostEffectManager::GetInstance().Init();
	postEffectScreen_ = PostEffectManager::GetInstance().CreatePostEffectScreen();

	currentEffect_ = PostEffectManager::EFFECT_TYPE::NORMAL;
	multiEffectMode_ = false;

	camMode_ = CAM_MODE::MANUAL;
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::OPENING);

	//AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);
	//AudioManager::GetInstance()->PlayBGM(SoundID::BGM_GAME);

	audioHandle_ = LoadSoundMem("Data/Sound/BGM/GameBGM.wav");
	ChangeVolumeSoundMem(120, audioHandle_);
	PlaySoundMem(audioHandle_, DX_PLAYTYPE_LOOP);
	// 音量

	hpHandle0_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_0).handleId_;
	hpHandle1_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_1).handleId_;
	hpHandle2_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_2).handleId_;
	hpHandle3_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_3).handleId_;
	hpHandle4_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_4).handleId_;
	hpHandle5_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_5).handleId_;
	hpHandle6_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_6).handleId_;
	hpHandle7_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_7).handleId_;
	hpHandle8_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_8).handleId_;
	hpHandle9_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_9).handleId_;
	hpHandle10_ = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_10).handleId_;
}

void GameScene::Update()
{
	auto const ins = InputManager::GetInstance();


	// 各オブジェクトの更新
	stage_->Update();
	skyDome_->Update();
	player_->Update();
	//weapon_->Update();
	enemyManager_->Update();
	itemManger_->Update();
	

	goto tmp;
	if (!enemyManager_->GetEnemyDead())
	{
		if (ins->IsGamepadTrgDown(InputManager::PadInput::RB, 0))
		{
			if (camMode_ == CAM_MODE::MANUAL)
			{
				camMode_ = CAM_MODE::TARGETING;
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::TARGETING);
			}
			else
			{
				camMode_ = CAM_MODE::MANUAL;
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::MANUAL);
			}
		}

		if (camMode_ == CAM_MODE::TARGETING && ins->IsGamepadTriggerTrgDown(true, 0))
		{
			//最大まで言ったら0に
			if (targetEnemyId_ > enemyManager_->GetEnemies().size() - 1)
			{
				targetEnemyId_ = 0;
			}
			else
			{
				targetEnemyId_++;
			}
		}
	}

	// ターゲット切り替え
	//if (ins->IsTrgDown(KEY_INPUT_LEFT) && targetEnemyId_ > 0) targetEnemyId_--;
	//if (ins->IsTrgDown(KEY_INPUT_RIGHT)) targetEnemyId_++;

//#ifdef _tmp
	targetPos_ = enemyManager_->GetEnemyPos(targetEnemyId_);
	SceneManager::GetInstance().GetCamera()->SetTargetPos(targetPos_);
//#endif // !_tmp

	tmp:

	// 攻撃コライダーの更新
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
				delete data->collider->GetFollow();
				delete data->collider;
				delete data;
				attackColliders_.erase(attackColliders_.begin() + i);
				i--;
			}
			else
			{
				if (data->collider != nullptr) 
				{
					Transform* transform = const_cast<Transform*>(data->collider->GetFollow());
					transform->pos = player_->GetTransform().pos;
				}
			}
		}
	}

	// エフェクト時間更新
	effectTime_ += sceMng_.GetDeltaTime();

	// モード切り替え (テンキー5)
	if (ins->IsTrgDown(KEY_INPUT_NUMPAD5))
	{
		multiEffectMode_ = !multiEffectMode_;
	}

	// 単一エフェクトモード
	if (!multiEffectMode_)
	{
		// エフェクト切り替え (テンキー4/6)
		if (ins->IsTrgDown(KEY_INPUT_NUMPAD4))
		{
			int current = static_cast<int>(currentEffect_);
			current = (current - 1 + static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX)) %
				static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
			currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
		}
		if (ins->IsTrgDown(KEY_INPUT_NUMPAD6))
		{
			int current = static_cast<int>(currentEffect_);
			current = (current + 1) % static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
			currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
		}
	}
	// 複数エフェクトモード
	else
	{
		// Enterキーで現在のエフェクトをトグル
		if (ins->IsTrgDown(KEY_INPUT_RETURN))
		{
			ToggleEffect(currentEffect_);
		}

		// エフェクト選択 (テンキー4/6)
		if (ins->IsTrgDown(KEY_INPUT_NUMPAD4))
		{
			int current = static_cast<int>(currentEffect_);
			current = (current - 1 + static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX)) %
				static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
			currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
		}
		if (ins->IsTrgDown(KEY_INPUT_NUMPAD6))
		{
			int current = static_cast<int>(currentEffect_);
			current = (current + 1) % static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
			currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
		}

		// 全クリア (テンキー0)
		if (ins->IsTrgDown(KEY_INPUT_NUMPAD0))
		{
			activeEffects_.clear();
		}
	}


	// ゲームオーバー判定
	if (player_->GetHp() <= 0)
	{
		// 強制的に全サウンド停止
		StopMusic();
		StopSoundMem(audioHandle_);
		AudioManager::GetInstance()->StopBGM();
		sceMng_.ChangeScene(SceneManager::SCENE_ID::OVER);
		return;
	}

	//// ゲームクリア判定
	//if (enemyManager_->GetEnemyDead())
	//{
	//	// 強制的に全サウンド停止
	//	StopMusic();
	//	StopSoundMem(audioHandle_);
	//	AudioManager::GetInstance()->StopBGM();
	//	sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	//	return;
	//}
}

void GameScene::Draw()
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	// 3D描画
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	//weapon_->Draw();
	itemManger_->Draw();
	enemyManager_->Draw();
	PlayerHpUIDraw();

	// 一時スクリーンにメイン画面をコピー
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);

	// エフェクト適用
	if (!multiEffectMode_)
	{
		// 単一エフェクトモード
		PostEffectManager::GetInstance().ApplyEffect(
			currentEffect_,
			tempScreen,
			postEffectScreen_,
			effectTime_
		);
	}
	else
	{
		// 複数エフェクトモード
		PostEffectManager::GetInstance().ApplyEffects(
			activeEffects_,
			tempScreen,
			postEffectScreen_,
			effectTime_
		);
	}

	// 最終結果をメイン画面に描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	//// デバッグ表示
	//int y = 10;
	//DrawFormatString(10, y, 0xFFFF00, "Mode: %s (NumPad5 to toggle)", 
	//	multiEffectMode_ ? "Multi" : "Single");
	//y += 20;
	//// トリガー値を表示
	//const auto& ins = InputManager::GetInstance();
	//int leftTrigger = ins->GetGamepadTriggerValue(true, 0);
	//int rightTrigger = ins->GetGamepadTriggerValue(false, 0);
	//DrawFormatString(10, y, 0xFFFF00, "Left Trigger: %d, Right Trigger: %d", leftTrigger, rightTrigger);
	//y += 20;
	//if (!multiEffectMode_)
	//{
	//	DrawFormatString(10, y, 0xFFFF00, "Current Effect: %s (NumPad4/6)", 
	//		GetEffectName(currentEffect_));
	//}
	//else
	//{
	//	DrawFormatString(10, y, 0xFFFF00, "Select: %s (Enter to toggle)", 
	//		GetEffectName(currentEffect_));
	//	y += 20;
	//	DrawFormatString(10, y, 0xFFFF00, "Active Effects: %d (NumPad0 to clear)", 
	//		static_cast<int>(activeEffects_.size()));
	//	y += 20;
	//	
	//	for (const auto& effect : activeEffects_)
	//	{
	//		DrawFormatString(10, y, 0x00FF00, "  - %s", GetEffectName(effect));
	//		y += 18;
	//	}
	//}

	// 一時スクリーン削除
	DeleteGraph(tempScreen);
}

void GameScene::Release()
{
	// ポストエフェクトスクリーン解放
	if (postEffectScreen_ != -1)
	{
		PostEffectManager::GetInstance().DeletePostEffectScreen(postEffectScreen_);
		postEffectScreen_ = -1;
	}

	// 攻撃コライダー解放
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

	// 敵マネージャー解放
	if (enemyManager_ != nullptr)
	{
		enemyManager_->Release();
		delete enemyManager_;
		enemyManager_ = nullptr;
	}

	// ステージ解放
	if (stage_ != nullptr)
	{
		stage_->Release();
		delete stage_;
		stage_ = nullptr;
	}

	// ステージの壁解放
	if (stageWall_ != nullptr)
	{
		stageWall_->Release();
		stageWall_.reset();
	}

	// スカイドーム解放
	if (skyDome_ != nullptr)
	{
		skyDome_->Release();
		delete skyDome_;
		skyDome_ = nullptr;
	}

	// プレイヤー解放
	if (player_ != nullptr)
	{
		player_->Release();
		delete player_;
		player_ = nullptr;
	}

	// 武器解放
	//if (weapon_ != nullptr)
	//{
	//	weapon_->Release();
	//	delete weapon_;
	//	weapon_ = nullptr;
	//}
}

void GameScene::CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float damage, int lifeTime)
{
	Transform* transform = new Transform();
	transform->pos = pos;

	ColliderBase* collider = new ColliderSphere(tag, transform, pos, radius);

	AttackColliderData* data = new AttackColliderData();
	data->collider = collider;
	data->damage = damage;
	data->lifeTime = lifeTime;

	attackColliders_.push_back(data);
}

void GameScene::ToggleEffect(PostEffectManager::EFFECT_TYPE effectType)
{
	// NORMALは追加しない
	if (effectType == PostEffectManager::EFFECT_TYPE::NORMAL)
	{
		return;
	}

	// 既にリストにあれば削除
	auto it = std::find(activeEffects_.begin(), activeEffects_.end(), effectType);
	if (it != activeEffects_.end())
	{
		activeEffects_.erase(it);
	}
	else
	{
		// なければ追加
		activeEffects_.push_back(effectType);
	}
}

const char* GameScene::GetEffectName(PostEffectManager::EFFECT_TYPE effectType)
{
	switch (effectType)
	{
	case PostEffectManager::EFFECT_TYPE::NORMAL: return "Normal";
	case PostEffectManager::EFFECT_TYPE::MONO: return "Mono";
	case PostEffectManager::EFFECT_TYPE::SEPIA: return "Sepia";
	case PostEffectManager::EFFECT_TYPE::INVERT: return "Invert";
	case PostEffectManager::EFFECT_TYPE::MOSAIC: return "Mosaic";
	case PostEffectManager::EFFECT_TYPE::CHROM_ABR: return "ChromaticAberration";
	case PostEffectManager::EFFECT_TYPE::VIGNETTE: return "Vignette";
	case PostEffectManager::EFFECT_TYPE::SCANLINE: return "Scanline";
	case PostEffectManager::EFFECT_TYPE::POSTERIZE: return "Posterize";
	case PostEffectManager::EFFECT_TYPE::GLITCH: return "Glitch";
	case PostEffectManager::EFFECT_TYPE::EMBOSS: return "Emboss";
	case PostEffectManager::EFFECT_TYPE::RETROWAVE: return "RetroWave";
	case PostEffectManager::EFFECT_TYPE::BLOOM: return "Bloom";
	case PostEffectManager::EFFECT_TYPE::RIPPLE: return "Ripple";
	case PostEffectManager::EFFECT_TYPE::RGB_SPLIT: return "RGBSplit";
	case PostEffectManager::EFFECT_TYPE::PIXELATE: return "Pixelate";
	case PostEffectManager::EFFECT_TYPE::SWIRL: return "Swirl";
	case PostEffectManager::EFFECT_TYPE::RADIAL_BLUR: return "RadialBlur";
	case PostEffectManager::EFFECT_TYPE::HUE_SHIFT: return "HueShift";
	case PostEffectManager::EFFECT_TYPE::WAVE: return "Wave";
	case PostEffectManager::EFFECT_TYPE::EDGE_DETECT: return "EdgeDetection";
	case PostEffectManager::EFFECT_TYPE::OLD_FILM: return "OldFilm";
	case PostEffectManager::EFFECT_TYPE::NIGHT_VISION: return "NightVision";
	case PostEffectManager::EFFECT_TYPE::LIQUID_DIST: return "LiquidDistortion";
	case PostEffectManager::EFFECT_TYPE::PINHOLE: return "Pinhole";
	case PostEffectManager::EFFECT_TYPE::SPEED_LINES: return "SpeedLines";
	case PostEffectManager::EFFECT_TYPE::FROSTED_GLASS: return "FrostedGlass";
	case PostEffectManager::EFFECT_TYPE::DOT_MATRIX: return "DotMatrix";
	case PostEffectManager::EFFECT_TYPE::DEPTH_FOG: return "DepthFog";
	case PostEffectManager::EFFECT_TYPE::DIGITAL_RAIN: return "DigitalRain";
	case PostEffectManager::EFFECT_TYPE::STROBE: return "Strobe";
	case PostEffectManager::EFFECT_TYPE::SNOW_STORM: return "SnowStorm";
	case PostEffectManager::EFFECT_TYPE::SCREEN_SHAKE: return "ScreenShake";
	case PostEffectManager::EFFECT_TYPE::CRT: return "CRT";
	default: return "Unknown";
	}
}

void GameScene::PlayerHpUIDraw()
{
	//プレイヤーHP
	int hp = player_->GetHp();

	hpUiCount_ =  (hp + 2 - 1) / 2;

	if(hpUiCount_ >= static_cast<int>(HP_UI::HP0)) 
		DrawGraph(IMG_HP_X,IMG_HP_Y,hpHandle0_,true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP1))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle1_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP2))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle2_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP3))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle3_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP4))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle4_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP5))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle5_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP6))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle6_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP7))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle7_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP8))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle8_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP9))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle9_, true);
	else if (hpUiCount_ == static_cast<int>(HP_UI::HP10))
		DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandle10_, true);
	
}
