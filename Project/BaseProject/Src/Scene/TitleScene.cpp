#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "TitleScene.h"
#include "../Application.h"
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

	IsSelect_ = true;

	SelectUpdate();

}


void TitleScene::Draw()
{
	//mainScreen_ = SceneManager::GetInstance().GetMainScreen();
	//SetDrawScreen(mainScreen_);
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	static int x = 10;
	DrawPixel(x++, 10, 0xffffff);


	DrawGraph(Application::SCREEN_SIZE_X / 2 + 100, 100, imgPlayer_, true);

	DrawGraph(50, 20, imgTitle_, TRUE);

	//ゲームスタート
	if (selectCount_ == static_cast <int>(SELECT::GAME))
	{
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgGameStart_, TRUE);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y, imgNotGameStart_, TRUE);
	}
	//チュートリアル
	if (selectCount_ == static_cast <int>(SELECT::TUTORIAL))
	{
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + 100, imgTutorial_, TRUE);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + 100, imgNotTutorial_, TRUE);
	}
	//オプション
	if (selectCount_ == static_cast <int>(SELECT::OPTION))
	{
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + 200, imgOption_, TRUE);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + 200, imgNotOption_, TRUE);
	}
	//終了
	if (selectCount_ == static_cast <int>(SELECT::EXIT))
	{
		DrawGraph(IMG_CHOICE_POS_X, IMG_CHOICE_POS_Y + 300, imgEnd_, TRUE);
	}
	else
	{
		DrawGraph(IMG_NOT_CHOICE_POS_X, IMG_CHOICE_POS_Y + 300, imgNotEnd_, TRUE);
	}




	SelectDraw((SELECT)selectCount_);
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
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case SELECT::TUTORIAL:

		break;
	case SELECT::OPTION:

		break;
	case SELECT::EXIT:

		Application::GetInstance().End();

		break;
	}
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
	if (ins->IsTrgDown(KEY_INPUT_SPACE) || ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))//決定
	{
		//決定SE
		AudioManager::GetInstance()->SetSeVolume(80);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}


}
