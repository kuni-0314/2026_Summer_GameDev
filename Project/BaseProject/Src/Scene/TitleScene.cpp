#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Manager/PostEffectManager.h"
#include "../Sound/AudioManager.h"


TitleScene::TitleScene()
	:
	SceneBase(),
	imgTitle_(-1),
	imgPushSpace_(-1),
	effectTime_(0.0f),
	imgEnd_(0),imgNotEnd_(0),
	imgGameStart_(0),imgNotGameStart_(0),
	imgOption_(0),imgNotOption_(0),
	imgNotTutorial_(0),imgTutorial_(0),
	isFadeIn_(false),
	nextScene_(SceneManager::SCENE_ID::NONE),
	postEffectScreen_(0),
	select_(SELECT::NONE),
	imgPlayer_(0)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	// エフェクト
	PostEffectManager::GetInstance().Init();
	postEffectScreen_ = PostEffectManager::GetInstance().CreatePostEffectScreen();

	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

#pragma region 画像読み込み

	//タイトル画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	//PUSH画像読み込み
	imgPushSpace_ = resMng_.Load(ResourceManager::SRC::TITLE_PUSH).handleId_;

	//タイトル用プレイヤー画像読み込み
	imgPlayer_ = resMng_.Load(ResourceManager::SRC::TITLE_PLAYER).handleId_;

	//選択画像読み込み
	imgGameStart_ = resMng_.Load(ResourceManager::SRC::TITLE_GAMESTART).handleId_;			//選択ゲームスタート
	imgNotGameStart_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_GAMESTART).handleId_;	//非選択ゲームスタート

	imgTutorial_ = resMng_.Load(ResourceManager::SRC::TITLE_TUTORIAL).handleId_;			//選択チュートリアル
	imgNotTutorial_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_TUTORIAL).handleId_;		//非選択チュートリアル

	imgOption_ = resMng_.Load(ResourceManager::SRC::TITLE_OPTION).handleId_;				//選択オプション
	imgNotOption_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_OPTION).handleId_;			//非選択オプション

	imgEnd_ = resMng_.Load(ResourceManager::SRC::TITLE_END).handleId_;						//選択エンド
	imgNotEnd_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_END).handleId_;				//非選択エンド

#pragma endregion

	//初期選択コマンド
	select_ = SELECT::GAME;

	//読み込みサウンド
	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_TITLE);
}

void TitleScene::Update()
{
	auto const ins = InputManager::GetInstance();

	if (!isFadeIn_) SelectUpdate();
	
	//フェード時間加算
	else effectTime_ += FADE_SPEED;

	if (effectTime_ >= 1.0f)
	{
		effectTime_ = 1.0f;
		switch (nextScene_)
		{
		case SceneManager::SCENE_ID::GAME:
			sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
			break;
		case SceneManager::SCENE_ID::TUTORIAL:
			sceMng_.ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
			break;
		case SceneManager::SCENE_ID::OPTION:
			sceMng_.ChangeScene(SceneManager::SCENE_ID::OPTION);
			break;
		}
	}
}


void TitleScene::Draw()
{
	//選択コマンド点滅用カウンター
	static int blinkCounter_ = 0;
	blinkCounter_++;

	//プレイヤー画像描画
	DrawGraph(Application::SCREEN_SIZE_X / 2 + IMG_PLAYER_REG_POS_X, IMG_PLAYER_REG_POS_Y, imgPlayer_, true);
	//タイトル描画
	DrawGraph(IMG_TITLE_REG_POS_X, IMG_TITLE_REG_POS_Y, imgTitle_, true);

	//ゲームスタートコマンド
	if (selectCount_ == static_cast <int>(SELECT::GAME))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgGameStart_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgNotGameStart_, true);
	}
	//チュートリアルコマンド
	if (selectCount_ == static_cast <int>(SELECT::TUTORIAL))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET, imgTutorial_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET, imgNotTutorial_, true);
	}
	//オプションコマンド
	if (selectCount_ == static_cast <int>(SELECT::OPTION))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2, imgOption_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2, imgNotOption_, true);
	}
	//終了コマンド
	if (selectCount_ == static_cast <int>(SELECT::EXIT))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3, imgEnd_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3, imgNotEnd_, true);
	}

#ifdef _DEBUG

	int imgWidth_, imgHeight_, img, posX;
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case static_cast<int>(SELECT::GAME):
			img = imgGameStart_;
			break;
		case static_cast<int>(SELECT::TUTORIAL):
			img = imgTutorial_;
			break;
		case static_cast<int>(SELECT::OPTION):
			img = imgOption_;
			break;
		case static_cast<int>(SELECT::EXIT):
			img = imgEnd_;
			break;
		default:
			break;
		}
		posX = i == selectCount_ ? IMG_CHOICE_POS_X : IMG_NOT_CHOICE_POS_X;
		GetGraphSize(img, &imgWidth_, &imgHeight_);
	;
		static float alpha = 0.0f, speed = 1.0f;


		if (i != selectCount_) continue;
		alpha = (sin(speed++ / 10.0f) * 255.0f + 255.0f) / 4.0f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
		DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i + imgHeight_, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#endif 

	SelectDraw((SELECT)selectCount_);

	// 一時スクリーンにメイン画面をコピー
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);

	//エフェクト加算・減算
	static float effectTime = 0.0f;

	if (InputManager::GetInstance()->IsNew(KEY_INPUT_W))
	{
		effectTime += ADD_EFFECT_POW;
	}
	else if (InputManager::GetInstance()->IsNew(KEY_INPUT_S))
	{
		effectTime -= ADD_EFFECT_POW;
	}
	
	PostEffectManager::EffectParams params = { effectTime_, effectTime_, effectTime_, 0.0f };
	auto& pstEfcMngIns = PostEffectManager::GetInstance();
	//設定エフェクト
	pstEfcMngIns.SetCustomParams(PostEffectManager::EFFECT_TYPE::FH_GAME_START, params);

	// 単一エフェクトモード
	pstEfcMngIns.ApplyEffect(
		PostEffectManager::EFFECT_TYPE::FH_GAME_START,
		tempScreen,
		postEffectScreen_,
		effectTime_
	);
	
	// 最終結果をメイン画面に描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	// 一時スクリーンを削除
	DeleteGraph(tempScreen);
}

void TitleScene::Release()
{
}

void TitleScene::SelectChange(SELECT next)
{
	select_ = next;

	switch (next)
	{
	case SELECT::GAME:
		AudioManager::GetInstance()->StopBGM();
		nextScene_ = SceneManager::SCENE_ID::GAME;
		break;
	case SELECT::TUTORIAL:
		AudioManager::GetInstance()->StopBGM();
		nextScene_ = SceneManager::SCENE_ID::TUTORIAL;
		break;
	case SELECT::OPTION:
		nextScene_ = SceneManager::SCENE_ID::OPTION;
		break;
	case SELECT::EXIT:
		Application::GetInstance().End();
		break;
	}

	effectTime_ = 0.0f;
	isFadeIn_ = true;
}

void TitleScene::SelectDraw(SELECT next)
{
	const char* name = "";

	if (next == SELECT::GAME) name = "ゲームスタート";
	else if (next == SELECT::TUTORIAL) name = "チュートリアル";
	else if (next == SELECT::OPTION) name = "設定";
	else if (next == SELECT::EXIT) name = "終了";

	DrawFormatString(SELECT_POS_X, SELECT_POS_Y, GetColor(COLOR_R, COLOR_G, COLOR_B), "選択中: %s", name);
}

void TitleScene::SelectUpdate()
{
	auto const ins = InputManager::GetInstance();

	//選択コマンド変更
	pSelectCount_ = selectCount_;
	if (ins->IsTrgDown(KEY_INPUT_UP) || ins->IsGamepadTrgUp(InputManager::PadInput::Up, 0))
	{
		//選択SE
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
		selectCount_--;
		if (selectCount_ < minIndex)
		{
			selectCount_ = maxIndex; // 一番下へ
		}
	}

	if (ins->IsTrgDown(KEY_INPUT_DOWN) || ins->IsGamepadTrgDown(InputManager::PadInput::Down, 0))
	{
		//選択SE
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
		selectCount_++;
		if (selectCount_ > maxIndex)
		{
			selectCount_ = minIndex; // 一番上へ
		}
	}

	// マウス操作があった場合、マウスカーソルの位置から選択肢を変更する
	if (ins->IsMouseMoved())
	{
		// マウスカーソルが選択肢の上にある場合、選択肢を変更する
		int mouseX_, mouseY_;
		ins->GetMousePos(mouseX_, mouseY_);

		//画像の横幅、立幅
		int imgWidth_, imgHeight_;

		//マウスが画像上にあるかのフラグ
		bool isMouseOver = false;

		// ゲームスタート
		GetGraphSize(selectCount_ == static_cast<int>(SELECT::GAME) ? imgGameStart_ : imgNotGameStart_, &imgWidth_, &imgHeight_);
		if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
			mouseY_ >= IMG_CHOICE_POS_Y && mouseY_ <= IMG_CHOICE_POS_Y + imgHeight_)
		{
			selectCount_ = static_cast<int>(SELECT::GAME);
			AudioManager::GetInstance()->SetSeVolume(SE_SET_VOLUME);
			isMouseOver = true;
		}

		// チュートリアル
		GetGraphSize(selectCount_ == static_cast<int>(SELECT::TUTORIAL) ? imgTutorial_ : imgNotTutorial_, &imgWidth_, &imgHeight_);
		if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
			mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET + imgHeight_)
		{
			selectCount_ = static_cast<int>(SELECT::TUTORIAL);
			AudioManager::GetInstance()->SetSeVolume(SE_SET_VOLUME);
			isMouseOver = true;
		}

		// オプション
		GetGraphSize(selectCount_ == static_cast<int>(SELECT::OPTION) ? imgOption_ : imgNotOption_, &imgWidth_, &imgHeight_);
		if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
			mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2 && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2 + imgHeight_)
		{
			selectCount_ = static_cast<int>(SELECT::OPTION);
			AudioManager::GetInstance()->SetSeVolume(SE_SET_VOLUME);
			isMouseOver = true;
		}

		// 終了
		GetGraphSize(selectCount_ == static_cast<int>(SELECT::EXIT) ? imgEnd_ : imgNotEnd_, &imgWidth_, &imgHeight_);
		if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
			mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3 && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3 + imgHeight_)
		{
			selectCount_ = static_cast<int>(SELECT::EXIT);
		
			isMouseOver = true;
		}
	}

	// SpaceキーまたはゲームパッドのAボタンで決定
	if (ins->IsTrgDown(KEY_INPUT_SPACE) || ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))//決定
	{
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}
	// マウスクリックで決定
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}

	//選択中のコマンド
	if (selectCount_ != pSelectCount_)
	{
		InputManager::GetInstance()->VibrateGamepad(VIBRATION_INDEX, VIBRATION_POWER, VIBRATION_TIME);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
	}
}
