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
#include "../Effect/EffectManager.h"
#include "../Effect/LoadEffekseer/EffekseerEffect.h"
#include "../Sound/AudioManager.h"
#include "GameScene.h"
#include <EffekseerForDXLib.h>

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
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::MANUAL);

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::GAME);
	//AudioManager::GetInstance()->PlayBGM(SoundID::BGM_GAME);

	audioHandle_ = LoadSoundMem("Data/Sound/BGM/GameBGM.wav");
	wargnigHandle_ = LoadSoundMem("Data/Sound/BGM/WarnigBgm.wav");
	//ChangeVolumeSoundMem(120, audioHandle_);
	//PlaySoundMem(audioHandle_, DX_PLAYTYPE_LOOP);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_GAME);
	// 音量

	hpHandles_.resize(11);

	hpHandles_[10] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_10).handleId_;
	hpHandles_[9] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_9).handleId_;
	hpHandles_[8] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_8).handleId_;
	hpHandles_[7] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_7).handleId_;
	hpHandles_[6] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_6).handleId_;
	hpHandles_[5] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_5).handleId_;
	hpHandles_[4] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_4).handleId_;
	hpHandles_[3] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_3).handleId_;
	hpHandles_[2] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_2).handleId_;
	hpHandles_[1] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_1).handleId_;
	hpHandles_[0] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_HP_0).handleId_;

	commandHandles_.resize(4);

	commandHandles_[static_cast <int>(COMMAND::THUNDER)] = resMng_.Load(ResourceManager::SRC::IMG_SELECT_SANDER).handleId_;
	commandHandles_[static_cast <int>(COMMAND::FIRE)] = resMng_.Load(ResourceManager::SRC::IMG_SELECT_FIRE).handleId_;
	commandHandles_[static_cast <int>(COMMAND::HEAL)] = resMng_.Load(ResourceManager::SRC::IMG_SELECT_RECOVERY).handleId_;
	commandHandles_[static_cast <int>(COMMAND::ALL)] = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALL).handleId_;

	selectCommand_ = static_cast<int>(COMMAND::THUNDER);

	commandHandles_.resize(6);

	fontCommandHandles_[static_cast <int>(COMMAND::THUNDER)][(int)COMMAND_STATE::NOT_USE] = resMng_.Load(ResourceManager::SRC::IMG_NOTUSE_SANDER).handleId_;
	fontCommandHandles_[static_cast <int>(COMMAND::THUNDER)][(int)COMMAND_STATE::USE] = resMng_.Load(ResourceManager::SRC::IMG_USE_SANDER).handleId_;
	fontCommandHandles_[static_cast <int>(COMMAND::FIRE)][(int)COMMAND_STATE::NOT_USE] = resMng_.Load(ResourceManager::SRC::IMG_NOTUSE_FIRE).handleId_;
	fontCommandHandles_[static_cast <int>(COMMAND::FIRE)][(int)COMMAND_STATE::USE] = resMng_.Load(ResourceManager::SRC::IMG_USE_FIRE).handleId_;
	fontCommandHandles_[static_cast <int>(COMMAND::HEAL)][(int)COMMAND_STATE::NOT_USE] = resMng_.Load(ResourceManager::SRC::IMG_NOTUSE_RECOVERY).handleId_;
	fontCommandHandles_[static_cast <int>(COMMAND::HEAL)][(int)COMMAND_STATE::USE] = resMng_.Load(ResourceManager::SRC::IMG_USE_RECOVERY).handleId_;

	playerUiHandles_.resize(static_cast<int>(PLAYRE_HP_STATE::STATE_MAX));

	playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DEF)] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_UI_DEF).handleId_;
	playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DAMEGE)] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_UI_DAMEGE).handleId_;
	playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::WARNIG)] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_UI_WARNIG).handleId_;


	lockOnImageHandle_ = resMng_.Load(ResourceManager::SRC::TARGET_CURSOR_ORANGE).handleId_;

	lockOnFontHandle_ = resMng_.Load(ResourceManager::SRC::IMG_LOCKON_FONT_UI).handleId_;
}

void GameScene::Update()
{
	auto const ins = InputManager::GetInstance();

	UpdateEffekseer3D();

	// 各オブジェクトの更新
	stage_->Update();
	skyDome_->Update();
	player_->Update();
	//weapon_->Update();
	enemyManager_->Update();
	itemManger_->Update();

	if (player_->IsShortCut())
	{
		selectCommand_ = static_cast<int>(COMMAND::ALL);
	}
	else if (selectCommand_ == static_cast<int>(COMMAND::ALL))
	{
		//selectCommand_ = static_cast<int>(usecommand_);
	}

	if (!enemyManager_->GetEnemyDead())
	{
		if (ins->IsGamepadTrgDown(InputManager::PadInput::RB, 0) || InputManager::GetInstance()->IsTrgDown(KEY_INPUT_R))
		{
			if (camMode_ == CAM_MODE::MANUAL)
			{
				camMode_ = CAM_MODE::TARGETING;
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::TARGETING);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_LOCKON);
			}
			else
			{
				camMode_ = CAM_MODE::MANUAL;
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::MANUAL);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_LOCKON_CHANGE);
			}
		}

		if (camMode_ == CAM_MODE::TARGETING && ins->IsGamepadTriggerTrgDown(true, 0) || InputManager::GetInstance()->IsTrgDown(KEY_INPUT_F))
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

	targetPos_ = enemyManager_->GetEnemyPos(targetEnemyId_);
	SceneManager::GetInstance().GetCamera()->SetTargetPos(targetPos_);

	// デバッグ用即死攻撃
	if (ins->IsTrgDown(MOUSE_INPUT_LEFT))
	{
		auto enemies = enemyManager_->GetEnemies();
		for (auto enemy : enemies)
		{
			if (!enemy->IsAlive()) continue;
			VECTOR playerPos = player_->GetTransform().pos;
			VECTOR enemyPos = enemy->GetTransform().pos;
			float dist = VSize(VSub(playerPos, enemyPos));
			if (dist < 300.0f)
			{
				enemy->Damege(99999);
			}
		}
	}

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

	//プレイヤーダメージUI処理
	if (damegeflag_)
	{
		damegeTimeCount_++;

		//表示時間
		if (damegeTimeCount_ > 60)
		{
			damegeflag_ = false;
			damegeTimeCount_ = 0;
		}
	}

	// コマンドUI処
	CommandUpdate();


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
		StopSoundMem(wargnigHandle_);
		AudioManager::GetInstance()->StopBGM();
		sceMng_.ChangeScene(SceneManager::SCENE_ID::OVER);
		return;
	}


	// ゲームクリア判定
	if (enemyManager_->GetEnemyDead())
	{
		clearTimer_++;

		if (clearTimer_ > 60)   // Deathエフェクト終了後
		{
			StopMusic();
			StopSoundMem(audioHandle_);
			AudioManager::GetInstance()->StopBGM();
			sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
		}

		return;
	}

	//#ifdef _DEBUG

	if (ins->IsTrgDown(KEY_INPUT_UP) /*|| ins->IsTrgDown(KEY_INPUT_E)*/ || ins->IsGamepadTrgDown(InputManager::PadInput::Up, 0))
	{
		AudioManager::GetInstance()->PlaySE(SoundID::SE_COMMAND_SELECT);

		selectCommand_--;
		if (selectCommand_ < static_cast <int>(COMMAND::THUNDER))
		{
			selectCommand_ = static_cast <int>(COMMAND::HEAL);
		}
	}

	if (ins->IsTrgDown(KEY_INPUT_DOWN) || ins->IsTrgDown(KEY_INPUT_Q) || ins->IsGamepadTrgDown(InputManager::PadInput::Down, 0))
	{
		AudioManager::GetInstance()->PlaySE(SoundID::SE_COMMAND_SELECT);

		selectCommand_++;
		if (selectCommand_ > static_cast <int>(COMMAND::HEAL))
		{
			selectCommand_ = static_cast <int>(COMMAND::THUNDER);
		}
	}

	//#endif

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


	PlayerHpDraw();
	CommandDraw();

	if (camMode_ == TARGETING)
	{
		static float angle = 0.0f;
		angle -= 0.025f;
		auto pos = targetPos_;
		pos.y += 80.0f;
		DrawBillboard3D(pos, 0.5f, 0.5f, 100.0f, angle, lockOnImageHandle_, true);
		DrawGraph(30, 100, lockOnFontHandle_, true);
	}


	// エフェクトを一番手前に描画
	EffectManager::GetInstance().Draw();

	DrawEffekseer3D();


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


	//DrawFormatString(500, 0, 0xffffff, "HP:%d", player_->GetHp());

	// 最終結果をメイン画面に描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	// デバッグ表示
	int y = 10;
	DrawFormatString(10, y, 0xFFFF00, "Mode: %s (NumPad5 to toggle)",
		multiEffectMode_ ? "Multi" : "Single");
	y += 20;
	// トリガー値を表示
	const auto& ins = InputManager::GetInstance();
	int leftTrigger = ins->GetGamepadTriggerValue(true, 0);
	int rightTrigger = ins->GetGamepadTriggerValue(false, 0);
	DrawFormatString(10, y, 0xFFFF00, "Left Trigger: %d, Right Trigger: %d", leftTrigger, rightTrigger);
	y += 20;
	if (!multiEffectMode_)
	{
		DrawFormatString(10, y, 0xFFFF00, "Current Effect: %s (NumPad4/6)",
			GetEffectName(currentEffect_));
	}
	else
	{
		DrawFormatString(10, y, 0xFFFF00, "Select: %s (Enter to toggle)",
			GetEffectName(currentEffect_));
		y += 20;
		DrawFormatString(10, y, 0xFFFF00, "Active Effects: %d (NumPad0 to clear)",
			static_cast<int>(activeEffects_.size()));
		y += 20;

		for (const auto& effect : activeEffects_)
		{
			DrawFormatString(10, y, 0x00FF00, "  - %s", GetEffectName(effect));
			y += 18;
		}
	}

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

	// エフェクトマネージャー解放
	EffectManager::GetInstance().Clear();

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

void GameScene::AddEnemyHitCollider(const ColliderBase* hitCollider)
{
	enemyManager_->AddHitCollider(hitCollider);
}

void GameScene::RemoveEnemyHitCollider(const ColliderBase* hitCollider)
{
	enemyManager_->RemoveHitCollider(hitCollider);

}

void GameScene::SetDamageFlag(bool flag)
{
	damegeflag_ = flag;
}

bool GameScene::GetFlag()
{
	return damegeflag_;

}

void GameScene::SelectCommand(COMMAND command)
{
	selectCommand_ = command;

	switch (command)
	{
	case GameScene::THUNDER:
		break;
	case GameScene::FIRE:
		break;
	case GameScene::HEAL:
		break;
	default:
		break;
	}
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

void GameScene::PlayerHpDraw()
{
	//HPゲージUI描画
	hpIndex_ = (player_->GetHp() + 2 - 1) / 2;

	if (hpIndex_ < 0) hpIndex_ = 0;
	if (hpIndex_ >= hpHandles_.size()) hpIndex_ = hpHandles_.size() - 1;

	DrawGraph(IMG_HP_X, IMG_HP_Y, hpHandles_[hpIndex_], true);

	if (damegeflag_)
	{

		//ダメージUI
		DrawGraph(IMG_HP_X, IMG_HP_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DAMEGE)], true);
	}
	else
	{
		if (player_->GetHp() <= 6)
		{
			//瀕死状態UI
			DrawGraph(IMG_HP_X, IMG_HP_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::WARNIG)], true);
		}
		else
		{
			//デフォルトUI
			DrawGraph(IMG_HP_X, IMG_HP_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DEF)], true);
		}
	}
}

void GameScene::CommandUpdate()
{
	//サンダー
	if (player_->GetThunderCoolTime() > 0)
	{
		thunderState_ = COMMAND_STATE::USE;
	}
	else
	{
		thunderState_ = COMMAND_STATE::NOT_USE;
	}


	// ファイア
	if (player_->GetFireCoolTime() > 0)
	{
		fireState_ = COMMAND_STATE::USE;
	}
	else
	{
		fireState_ = COMMAND_STATE::NOT_USE;
	}

	// 回復
	if (player_->GetHealCoolTime() > 0)
	{
		healState_ = COMMAND_STATE::USE;
	}
	else
	{
		healState_ = COMMAND_STATE::NOT_USE;
	}
}

void GameScene::CommandDraw()
{

	//選択してるコマンド描画
		DrawGraph(0, 735, commandHandles_[selectCommand_], true);

	const int baseX = 45;
	const int selectOffset = 90;

	//
	for (int i = 0; i < static_cast<int>(COMMAND::MAX); i++)
	{
		bool isSelect =
			(selectCommand_ == static_cast<int>(COMMAND::ALL)) ||
			(i == selectCommand_);

		COMMAND_STATE state = COMMAND_STATE::NOT_USE;

		switch (static_cast<COMMAND>(i))
		{
		case COMMAND::THUNDER:
			state = thunderState_;
			break;

		case COMMAND::FIRE:
			state = fireState_;
			break;

		case COMMAND::HEAL:
			state = healState_;
			break;
		}

		DrawGraph(
			baseX + (isSelect ? selectOffset : 0),
			780 + i * 75,
			fontCommandHandles_[i][static_cast<int>(state)],
			true);
	}
}

void GameScene::PlayerFaceUIDrow()
{

}
