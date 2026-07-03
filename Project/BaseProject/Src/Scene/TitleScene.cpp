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
	imgTitle_(-1),
	imgPushSpace_(-1),
	SceneBase()
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

	//タイトル画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPushSpace_ = resMng_.Load(ResourceManager::SRC::TITLE_PUSH).handleId_;

	imgPlayer_ = resMng_.Load(ResourceManager::SRC::TITLE_PLAYER).handleId_;

	//選択画像読み込み
	imgGameStart_ = resMng_.Load(ResourceManager::SRC::TITLE_GAMESTART).handleId_;
	imgNotGameStart_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_GAMESTART).handleId_;
	imgTutorial_ = resMng_.Load(ResourceManager::SRC::TITLE_TUTORIAL).handleId_;
	imgNotTutorial_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_TUTORIAL).handleId_;
	imgOption_ = resMng_.Load(ResourceManager::SRC::TITLE_OPTION).handleId_;
	imgNotOption_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_OPTION).handleId_;
	imgEnd_ = resMng_.Load(ResourceManager::SRC::TITLE_END).handleId_;
	imgNotEnd_ = resMng_.Load(ResourceManager::SRC::TITLE_NOT_END).handleId_;

	//初期選択コマンド
	select_ = SELECT::GAME;

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_TITLE);
	AudioManager::GetInstance()->SetBgmVolume(120);

}

void TitleScene::Update()
{

	auto const ins = InputManager::GetInstance();

	//IsSelect_ = true;

	// エフェクト時間更新
	

	if (!isFadeIn_) SelectUpdate();
	else effectTime_ += FADE_SPEED;

	if (effectTime_ >= 1.0f)
	{
		effectTime_ = 1.0f;
		switch (nextScene_)
		{
		case SceneManager::SCENE_ID::GAME:
			sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
			break;
		//case SceneManager::SCENE_ID::TUTORIAL:
		//	sceMng_.ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
		//	break;
		case SceneManager::SCENE_ID::OPTION:
			sceMng_.ChangeScene(SceneManager::SCENE_ID::OPTION);
			break;
		default:
			break;
		}
	}

}


void TitleScene::Draw()
{
	static int blinkCounter_ = 0;
	blinkCounter_++;
	const int BLINK_CYCLE = 20;
	//DrawPixel(x++, 10, 0xffffff);

	DrawGraph(Application::SCREEN_SIZE_X / 2 + 100, 100, imgPlayer_, true);

	DrawGraph(50, 20, imgTitle_, true);

	//ゲームスタート
	if (selectCount_ == static_cast <int>(SELECT::GAME))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgGameStart_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgNotGameStart_, true);
	}
	//チュートリアル
	if (selectCount_ == static_cast <int>(SELECT::TUTORIAL))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET, imgTutorial_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET, imgNotTutorial_, true);
	}
	//オプション
	if (selectCount_ == static_cast <int>(SELECT::OPTION))
	{
		if (!isFadeIn_ || blinkCounter_ % BLINK_CYCLE < BLINK_CYCLE / 2)
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2, imgOption_, true);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2, imgNotOption_, true);
	}
	//終了
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
		//DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i + imgHeight_, GetRand(0xffffff), false);
		static float alpha = 0.0f, speed = 1.0f;

		// sin(speed++ / 100.0f) → -1.0f - 1.0f
		// sin(speed++ / 100.0f) * 255.0f → -255.0f - 255.0f
		// sin(speed++ / 100.0f) * 255.0f + 255.0f → 0.0f - 510.0f
		// (sin(speed++ / 100.0f) * 255.0f + 255.0f) / 4.0f → 0.0f - 127.5f
		if (i != selectCount_) continue;
		alpha = (sin(speed++ / 10.0f) * 255.0f + 255.0f) / 4.0f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
		DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * i + imgHeight_, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//img = selectCount_ == static_cast<int>(SELECT::GAME) ? imgGameStart_ : imgNotGameStart_;
	//posX = selectCount_ == static_cast<int>(SELECT::GAME) ? IMG_CHOICE_POS_X : IMG_NOT_CHOICE_POS_X;
	//GetGraphSize(img, &imgWidth_, &imgHeight_);
	//DrawBox(posX, IMG_CHOICE_POS_Y, posX + imgWidth_, IMG_CHOICE_POS_Y + imgHeight_, GetRand(0xffffff), false);
	//img = selectCount_ == static_cast<int>(SELECT::TUTORIAL) ? imgTutorial_ : imgNotTutorial_;
	//posX = selectCount_ == static_cast<int>(SELECT::TUTORIAL) ? IMG_CHOICE_POS_X : IMG_NOT_CHOICE_POS_X;
	//GetGraphSize(img, &imgWidth_, &imgHeight_);
	//DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET + imgHeight_, GetRand(0xffffff), false);
	//img = selectCount_ == static_cast<int>(SELECT::OPTION) ? imgOption_ : imgNotOption_;
	//posX = selectCount_ == static_cast<int>(SELECT::OPTION) ? IMG_CHOICE_POS_X : IMG_NOT_CHOICE_POS_X;
	//GetGraphSize(img, &imgWidth_, &imgHeight_);
	//DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2 + imgHeight_, GetRand(0xffffff), false);
	//img = selectCount_ == static_cast<int>(SELECT::EXIT) ? imgEnd_ : imgNotEnd_;
	//posX = selectCount_ == static_cast<int>(SELECT::EXIT) ? IMG_CHOICE_POS_X : IMG_NOT_CHOICE_POS_X;
	//GetGraphSize(img, &imgWidth_, &imgHeight_);
	//DrawBox(posX, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3, posX + imgWidth_, IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3 + imgHeight_, GetRand(0xffffff), false);
#endif // _DEBUG

	SelectDraw((SELECT)selectCount_);
	

	// 一時スクリーンにメイン画面をコピー
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, false);
	static float effectTime = 0.0f;
	if (InputManager::GetInstance()->IsNew(KEY_INPUT_W))
	{
		effectTime += 0.01f;
	}
	else if (InputManager::GetInstance()->IsNew(KEY_INPUT_S))
	{
		effectTime -= 0.01f;
	}
	
	PostEffectManager::EffectParams params = { effectTime_, effectTime_, effectTime_, 0.0f };
	auto& pstEfcMngIns = PostEffectManager::GetInstance();
	pstEfcMngIns.SetCustomParams(PostEffectManager::EFFECT_TYPE::FH_GAME_START, params);
	// エフェクト適用
	//if (!multiEffectMode_)
	//{
	// 単一エフェクトモード
	pstEfcMngIns.ApplyEffect(
		PostEffectManager::EFFECT_TYPE::FH_GAME_START,
		tempScreen,
		postEffectScreen_,
		effectTime_
	);
	//}
	//else
	//{
	//	// 複数エフェクトモード
	//	PostEffectManager::GetInstance().ApplyEffects(
	//		activeEffects_,
	//		tempScreen,
	//		postEffectScreen_,
	//		effectTime_
	//	);
	//}


	// 最終結果をメイン画面に描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	// 一時スクリーンを削除
	DeleteGraph(tempScreen);
}

void TitleScene::Release()
{

	DeleteGraph(imgTitle_);
	DeleteGraph(imgGameStart_);
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
		//AudioManager::GetInstance()->StopBGM();
		//nextScene_ = SceneManager::SCENE_ID::TUTORIAL;
		break;
	case SELECT::OPTION:
		AudioManager::GetInstance()->StopBGM();
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

	DrawFormatString(100, 100, GetColor(255, 255, 255), "選択中: %s", name);
}

void TitleScene::SelectUpdate()
{
	auto const ins = InputManager::GetInstance();

	//選択コマンド変更

	if (ins->IsTrgDown(KEY_INPUT_UP) || ins->IsGamepadTrgUp(InputManager::PadInput::Up, 0))
	{
		//選択SE
		AudioManager::GetInstance()->SetSeVolume(150);
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
		AudioManager::GetInstance()->SetSeVolume(150);
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
		int imgWidth_, imgHeight_;

		// ゲームスタート
		if (selectCount_ != static_cast<int>(SELECT::GAME))
		{
			GetGraphSize(selectCount_ == static_cast<int>(SELECT::GAME) ? imgGameStart_ : imgNotGameStart_, &imgWidth_, &imgHeight_);
			if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
				mouseY_ >= IMG_CHOICE_POS_Y && mouseY_ <= IMG_CHOICE_POS_Y + imgHeight_)
			{
				selectCount_ = static_cast<int>(SELECT::GAME);
				AudioManager::GetInstance()->SetSeVolume(150);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
			}
		}

		// チュートリアル
		if (selectCount_ != static_cast<int>(SELECT::TUTORIAL))
		{
			GetGraphSize(selectCount_ == static_cast<int>(SELECT::TUTORIAL) ? imgTutorial_ : imgNotTutorial_, &imgWidth_, &imgHeight_);
			if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
				mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET + imgHeight_)
			{
				selectCount_ = static_cast<int>(SELECT::TUTORIAL);
				AudioManager::GetInstance()->SetSeVolume(150);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
			}
		}

		// オプション
		if (selectCount_ != static_cast<int>(SELECT::OPTION))
		{
			GetGraphSize(selectCount_ == static_cast<int>(SELECT::OPTION) ? imgOption_ : imgNotOption_, &imgWidth_, &imgHeight_);
			if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
				mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2 && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 2 + imgHeight_)
			{
				selectCount_ = static_cast<int>(SELECT::OPTION);
				AudioManager::GetInstance()->SetSeVolume(150);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
			}
		}

		// 終了
		if (selectCount_ != static_cast<int>(SELECT::EXIT))
		{
			GetGraphSize(selectCount_ == static_cast<int>(SELECT::EXIT) ? imgEnd_ : imgNotEnd_, &imgWidth_, &imgHeight_);
			if (mouseX_ >= IMG_CHOICE_POS_X && mouseX_ <= IMG_CHOICE_POS_X + imgWidth_ &&
				mouseY_ >= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3 && mouseY_ <= IMG_CHOICE_POS_Y + IMG_CHOICE_POS_Y_OFFSET * 3 + imgHeight_)
			{
				selectCount_ = static_cast<int>(SELECT::EXIT);
				AudioManager::GetInstance()->SetSeVolume(150);
				AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_SELECT);
			}
		}
	}

	// SpaceキーまたはゲームパッドのAボタンで決定
	if (ins->IsTrgDown(KEY_INPUT_SPACE) || ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))//決定
	{
		AudioManager::GetInstance()->SetSeVolume(80);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}
	// マウスクリックで決定
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		AudioManager::GetInstance()->SetSeVolume(80);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}
}
