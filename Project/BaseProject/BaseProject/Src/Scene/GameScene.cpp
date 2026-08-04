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
#include "../Object/Common/AnimationController.h"
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
	ChangeVolumeSoundMem(100, wargnigHandle_);

	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_GAME);
	AudioManager::GetInstance()->SetBgmVolume(100);
	// 音量

	hpHandles_.resize(11);

	const int MAX_HP = 10;
	for (int i = 0; i <= MAX_HP; ++i)
	{
		using SRC = ResourceManager::SRC;
		hpHandles_[i] = resMng_.Load(static_cast<SRC>(static_cast<int>(SRC::IMG_PLAYER_HP_0) + i)).handleId_;
		GraphFilter(hpHandles_[i], DX_GRAPH_FILTER_HSB, 0, (MAX_HP - i) * -12, 0, 0);
	}

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
	playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DAMAGE)] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_UI_DAMEGE).handleId_;
	playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::WARNING)] = resMng_.Load(ResourceManager::SRC::IMG_PLAYER_UI_WARNIG).handleId_;


	lockOnImageHandle_ = resMng_.Load(ResourceManager::SRC::TARGET_CURSOR_ORANGE).handleId_;

	lockOnFontHandle_ = resMng_.Load(ResourceManager::SRC::IMG_LOCKON_FONT_UI).handleId_;
}

void GameScene::Update()
{
	auto const ins = InputManager::GetInstance();

	UpdateEffekseer3D();
	EffectManager::GetInstance().Update();

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


	if (player_->GetHp() <= 6)
	{
		if (!isWarning_)
		{
			isWarning_ = true;

	
			PlaySoundMem(wargnigHandle_, DX_PLAYTYPE_LOOP);
		}
	}
	else
	{
		if (isWarning_)
		{
			isWarning_ = false;

			StopSoundMem(wargnigHandle_);
		}
	}

	if (!enemyManager_->GetEnemyDead())
	{
		if (ins->IsGamepadTrgDown(InputManager::PadInput::RB, 0) || InputManager::GetInstance()->IsTrgDown(KEY_INPUT_R))
		{
			if (camMode_ == CAM_MODE::MANUAL)
			{
				camMode_ = CAM_MODE::TARGETING;
				VECTOR pPos = player_->GetTransform().pos;
				VECTOR ePos = enemyManager_->GetEnemyPos(targetEnemyId_);
				VECTOR dir = VNorm(VSub(ePos, pPos));
				player_->SetMoveDir(dir);
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::TARGETING);
				AudioManager::GetInstance()->SetSeVolume(150);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_LOCKON);
			}
			else
			{
				camMode_ = CAM_MODE::MANUAL;
				sceMng_.GetCamera()->ChangeMode(Camera::MODE::MANUAL);
				AudioManager::GetInstance()->SetSeVolume(150);
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

	//プレイヤーダメージUI処理
	if (damageflag_)
	{
		PlayerDamegeVoice();
		damegeTimeCount_++;

		//表示時間
		if (damegeTimeCount_ > 60)
		{
			damageflag_ = false;
			damegeTimeCount_ = 0;
		}
	}

	// コマンドUI処
	CommandUpdate();
	UpdateHpUI();

	// エフェクト時間更新
	effectTime_ += sceMng_.GetDeltaTime();

	auto& pstEfcMngIns = PostEffectManager::GetInstance();
	static float radialBlurParam = 0.0f;
	const float MAX_RADIAL_BLUR_PARAM = 0.05f;
	const float RADIAL_BLUR_STEP = 0.0025f;
	if (player_->IsRolling())
	{
		if (radialBlurParam == 0.0f)
		{
			ToggleEffect(PostEffectManager::EFFECT_TYPE::RADIAL_BLUR);
		}

		if (radialBlurParam < MAX_RADIAL_BLUR_PARAM)
		{
			radialBlurParam += RADIAL_BLUR_STEP;
		}
		else
		{
			radialBlurParam = MAX_RADIAL_BLUR_PARAM;
		}
	}
	else
	{
		if (radialBlurParam > 0.0f)
		{
			radialBlurParam -= RADIAL_BLUR_STEP;
		}
		else
		{
			radialBlurParam = 0.0f;
			ToggleEffect(PostEffectManager::EFFECT_TYPE::RADIAL_BLUR);
		}
	}

	PostEffectManager::EffectParams params = { radialBlurParam, 0.0f, 0.0f, 0.0f };
	pstEfcMngIns.SetCustomParams(PostEffectManager::EFFECT_TYPE::RADIAL_BLUR, params);

	// x: 強度（減光の急峻さ）, y: 範囲（減光が始まる距離）
	const float STANDARD_VIGNETTE_INTENSITY = 0.5f;
	const float STANDARD_VIGNETTE_RANGE = 0.85f;
	// 0から1までの値をsinで変化させる
	static float time = 0.0f;
	time += SceneManager::GetInstance().GetDeltaTime();
	float sinWaveValue = sin(time * 2.0f * DX_PI_F) * 0.5f + 0.5f;
	float vignetteIntensity = STANDARD_VIGNETTE_INTENSITY + 1.0f * sinWaveValue;
	float vignetteRange = STANDARD_VIGNETTE_RANGE + 1.0f * sinWaveValue;
	params = { vignetteIntensity, vignetteRange, 0.0f, 0.0f };
	pstEfcMngIns.SetCustomParams(PostEffectManager::EFFECT_TYPE::FH_LOW_HP, params);

	// モード切り替え (テンキー5)
	if (ins->IsTrgDown(KEY_INPUT_NUMPAD5))
	{
		multiEffectMode_ = !multiEffectMode_;
	}

	// 単一エフェクトモード
	//if (!multiEffectMode_)
	//{
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
	//}
	// 複数エフェクトモード
	//else
	//{
	//	// Enterキーで現在のエフェクトをトグル
	//	if (ins->IsTrgDown(KEY_INPUT_RETURN))
	//	{
	//		ToggleEffect(currentEffect_);
	//	}

	//	// エフェクト選択 (テンキー4/6)
	//	if (ins->IsTrgDown(KEY_INPUT_NUMPAD4))
	//	{
	//		int current = static_cast<int>(currentEffect_);
	//		current = (current - 1 + static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX)) %
	//			static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
	//		currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
	//	}
	//	if (ins->IsTrgDown(KEY_INPUT_NUMPAD6))
	//	{
	//		int current = static_cast<int>(currentEffect_);
	//		current = (current + 1) % static_cast<int>(PostEffectManager::EFFECT_TYPE::MAX);
	//		currentEffect_ = static_cast<PostEffectManager::EFFECT_TYPE>(current);
	//	}

	//	 全クリア (テンキー0)
	//	if (ins->IsTrgDown(KEY_INPUT_NUMPAD0))
	//	{
	//		activeEffects_.clear();
	//	}
	//}


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

		if (clearTimer_ > 100)   // Deathエフェクト終了後
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
		AudioManager::GetInstance()->SetSeVolume(200);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_COMMAND_SELECT);

		selectCommand_--;
		if (selectCommand_ < static_cast <int>(COMMAND::THUNDER))
		{
			selectCommand_ = static_cast <int>(COMMAND::HEAL);
		}
	}

	if (ins->IsTrgDown(KEY_INPUT_DOWN) || ins->IsTrgDown(KEY_INPUT_Q) || ins->IsGamepadTrgDown(InputManager::PadInput::Down, 0))
	{
		AudioManager::GetInstance()->SetSeVolume(200);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_COMMAND_SELECT);

		selectCommand_++;
		if (selectCommand_ > static_cast <int>(COMMAND::HEAL))
		{
			selectCommand_ = static_cast <int>(COMMAND::THUNDER);
		}
	}

	//#endif

	if (player_->IsShortCut())
	{
		selectCommand_ = static_cast<int>(COMMAND::ALL);
	}
	else if (selectCommand_ == static_cast<int>(COMMAND::ALL))
	{
		selectCommand_ = static_cast<int>(useCommand_);
	}

}

void GameScene::Draw()
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	// エフェクトを一番手前に描画
	EffectManager::GetInstance().Draw();

	DrawEffekseer3D();

	// 3D描画
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	//weapon_->Draw();
	itemManger_->Draw();
	enemyManager_->Draw();



	if (camMode_ == TARGETING)
	{
		static float angle = 0.0f;
		angle -= 0.025f;
		auto pos = targetPos_;
		pos.y += 80.0f;
		DrawBillboard3D(pos, 0.5f, 0.5f, 100.0f, angle, lockOnImageHandle_, true);
		DrawGraph(30, 100, lockOnFontHandle_, true);
	}





	// 一時スクリーンにメイン画面をコピー
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);

	// エフェクト適用
	//if (!multiEffectMode_)
	//{
		// 単一エフェクトモード
		//PostEffectManager::GetInstance().ApplyEffect(
		//	currentEffect_,
		//	tempScreen,
		//	postEffectScreen_,
		//	effectTime_
		//);
	//}
	//else
	//{
		// 複数エフェクトモード
		PostEffectManager::GetInstance().ApplyEffects(
			activeEffects_,
			tempScreen,
			postEffectScreen_,
			effectTime_
		);
	//}

	PlayerHpDraw();
	CommandDraw();

	//
	// (500, 0, 0xffffff, "HP:%d", player_->GetHp());

	// 最終結果をメイン画面に描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	// デバッグ表示
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

	// エフェクトマネージャー解放
	EffectManager::GetInstance().Clear();
	EffectManager::GetInstance().Release();

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

}

void GameScene::CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float damage)
{
	Transform* transform = new Transform();
	transform->pos = pos;

	ColliderBase* collider = new ColliderSphere(tag, transform, pos, radius);

	AttackColliderData* data = new AttackColliderData();
	data->collider = collider;
	data->damage = damage;
	data->isActive = false;

	attackColliders_.push_back(data);
}

void GameScene::DeleteAttackCollider()
{
	// 本当は良くない方法
	attackColliders_.clear();
}

void GameScene::SetActiveAttackCollider(bool isActive)
{
	// 面倒だから一括で有効化/無効化
	for (auto data : attackColliders_)
	{
		data->isActive = isActive;
	}
}

void GameScene::AddEnemyHitCollider(const ColliderBase* hitCollider)
{
	enemyManager_->AddHitCollider(hitCollider);
}

void GameScene::RemoveEnemyHitCollider(const ColliderBase* hitCollider)
{
	enemyManager_->RemoveHitCollider(hitCollider);

}

void GameScene::AddPlayerHitCollider(const ColliderBase* hitCollider)
{
	player_->AddHitCollider(hitCollider);
}

void GameScene::RemovePlayerHitCollider(const ColliderBase* hitCollider)
{
	player_->RemoveHitCollider(hitCollider);
}

//void GameScene::SetDamageFlag(bool flag)
//{
//	isDamageFlag_ = flag;
//}

void GameScene::ShakeHpUI()
{
	isHpUIShake_ = true;
	shakePow_ = SHAKE_POW_MAX;
}

void GameScene::CheckHitEnemy(const VECTOR& pos, float radius, int damage)
{
	enemyManager_->CheckHit(pos, radius, damage);
}

void GameScene::SetLowHpEffect()
{
	ToggleEffect(PostEffectManager::EFFECT_TYPE::FH_LOW_HP);
}

void GameScene::PlayerDamegeVoice()
{
	
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
	case PostEffectManager::EFFECT_TYPE::FADE_WHITE: return "CRT";
	case PostEffectManager::EFFECT_TYPE::ZOOM_IN_RADIAL_BLUR: return "ZoomInRadialBlur";
	case PostEffectManager::EFFECT_TYPE::FH_GAME_START: return "FH_GameStart";
	case PostEffectManager::EFFECT_TYPE::FH_LOW_HP: return "FH_Low_HP";
	default: return "Unknown";
	}
}

void GameScene::PlayerHpDraw()
{
	//HPゲージUI描画
	hpIndex_ = (player_->GetHp() + 2 - 1) / 2;

	if (hpIndex_ < 0) hpIndex_ = 0;
	if (hpIndex_ >= hpHandles_.size()) hpIndex_ = hpHandles_.size() - 1;

	const int UI_OFFSET_Y = IMG_HP_Y + hpUIOffsetY_;

	DrawGraph(IMG_HP_X, UI_OFFSET_Y, hpHandles_[hpIndex_], true);

	if (damageflag_)
	{
		//ダメージUI
		DrawGraph(IMG_HP_X, UI_OFFSET_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DAMAGE)], true);
	}
	else
	{
		if (player_->GetHp() <= 6)
		{
			//瀕死状態UI
			DrawGraph(IMG_HP_X, UI_OFFSET_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::WARNING)], true);
		}
		else
		{
			//デフォルトUI
			DrawGraph(IMG_HP_X, UI_OFFSET_Y, playerUiHandles_[static_cast<int>(PLAYRE_HP_STATE::DEF)], true);
		}
	}
}

void GameScene::CommandUpdate()
{
	//サンダー
	if (player_->GetThunderCoolTime() > 0)
	{
		//クールタイムフォント切り替え
		thunderState_ = COMMAND_STATE::USE;
		//使用魔法保存
		useCommand_ = COMMAND::THUNDER;
	}
	else
	{
		thunderState_ = COMMAND_STATE::NOT_USE;
	}


	// ファイア
	if (player_->GetFireCoolTime() > 0)
	{
		//クールタイムフォント切り替え
		fireState_ = COMMAND_STATE::USE;
		//使用魔法保存
		useCommand_ = COMMAND::FIRE;
	}
	else
	{
		fireState_ = COMMAND_STATE::NOT_USE;
	}

	// 回復
	if (player_->GetHealCoolTime() > 0)
	{
		//クールタイムフォント切り替え
		healState_ = COMMAND_STATE::USE;
		//使用魔法保存
		useCommand_ = COMMAND::HEAL;
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

void GameScene::PlayerFaceUIDraw()
{
	// 何のための関数ですか
}

void GameScene::UpdateHpUI()
{
	const float SHAKE_SPEED = 30.0f;
	static float time = 0.0f;

	if (isHpUIShake_)
	{
		time += SHAKE_SPEED * sceMng_.GetDeltaTime();

		hpUIOffsetY_ = sinf(time) * shakePow_;

		shakePow_ *= SHAKE_DECREASE;

		if (shakePow_ < 0.1f)
		{
			isHpUIShake_ = false;
			hpUIOffsetY_ = 0.0f;
			time = 0.0f;
		}
	}
	else
	{
		hpUIOffsetY_ = 0.0f;
	}
}
