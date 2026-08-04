#include "ClearScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Sound/AudioManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "../Application.h"


ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
	SetMouseDispFlag(true);

	//画像ハンドル読み込み
	playerHandle_ = resMng_.Load(ResourceManager::SRC::PLAYER_GAMEOVER).handleId_;
	imgOnTitleHandle_ = resMng_.Load(ResourceManager::SRC::IMG_ON_TITLE).handleId_;
	imgOffTitleHandle_ = resMng_.Load(ResourceManager::SRC::IMG_OFF_TITLE).handleId_;
	imgGameClear_ = resMng_.Load(ResourceManager::SRC::IMG_GAMECLEAR).handleId_;


	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_CLEAR);
	AudioManager::GetInstance()->SetBgmVolume(200);

	VECTOR playerPos_;
	playerPos_ = { 0,150,0 };

	MV1SetPosition(playerHandle_, playerPos_);
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	
}

void ClearScene::Update()
{
	auto const ins = InputManager::GetInstance();

	if (ins->IsTrgDown(KEY_INPUT_SPACE) || ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))
	{
		AudioManager::GetInstance()->StopBGM();
		AudioManager::GetInstance()->SetSeVolume(150);
		
		
		(SoundID::SE_TITLE_DECISION);
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (isMouseOver_)
	{
		if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
		{
			AudioManager::GetInstance()->StopBGM();
			AudioManager::GetInstance()->SetSeVolume(150);
			AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
			sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	
	// マウスカーソルが選択肢の上にある場合、選択肢を変更する
	int mouseX_, mouseY_;
	ins->GetMousePos(mouseX_, mouseY_);
	int imgWidth_, imgHeight_;
	GetGraphSize(imgOnTitleHandle_, &imgWidth_, &imgHeight_);
	const int IMG_POS_X = Application::SCREEN_SIZE_X / 2 - imgWidth_ / 2;
	const int IMG_POS_Y = (Application::SCREEN_SIZE_Y / 2 + 400) - imgHeight_ / 2;
	// ゲームスタート
	if (mouseX_ >= IMG_POS_X && mouseX_ <= IMG_POS_X + imgWidth_ &&
		mouseY_ >= IMG_POS_Y && mouseY_ <= IMG_POS_Y + imgHeight_)
	{
		isMouseOver_ = true;
	}
	else
	{
		isMouseOver_ = false;
	}
}

void ClearScene::Draw()
{
	int imgWidth_, imgHeight_;
	GetGraphSize(imgOnTitleHandle_, &imgWidth_, &imgHeight_);
	const int IMG_POS_X = Application::SCREEN_SIZE_X / 2 - imgWidth_ / 2;
	const int IMG_POS_Y = (Application::SCREEN_SIZE_Y / 2 + 400) - imgHeight_ / 2;

	DrawGraph(250, 200, imgGameClear_, true);

	if (isMouseOver_)
	{
		DrawGraph(IMG_POS_X, IMG_POS_Y, imgOnTitleHandle_, true);
	}
	else
	{
		DrawGraph(IMG_POS_X, IMG_POS_Y, imgOffTitleHandle_, true);
	}

}

void ClearScene::Release()
{
	
}
