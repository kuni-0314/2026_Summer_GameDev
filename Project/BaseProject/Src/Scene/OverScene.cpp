#include "OverScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Sound/AudioManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "../Application.h"

OverScene::OverScene()
	:SceneBase()
{
}

OverScene::~OverScene()
{
}

void OverScene::Init()
{
	selectCount_ = 0;
	maxIndex = static_cast<int>(SELECT::TITLE);
	minIndex = static_cast<int>(SELECT::CONTINUE);

	select_ = SELECT::CONTINUE;

	playerHandle_ = resMng_.Load(ResourceManager::SRC::PLAYER_GAMEOVER).handleId_;

	imgOffTitleHandle_ = resMng_.Load(ResourceManager::SRC::IMG_OFF_TITLE).handleId_;
	imgOffContinueHandle_ = resMng_.Load(ResourceManager::SRC::IMG_OFF_CONTINUE).handleId_;
	imgOffRetryHandle_ = resMng_.Load(ResourceManager::SRC::IMG_OFF_RETRY).handleId_;

	imgOnTitleHandle_ = resMng_.Load(ResourceManager::SRC::IMG_ON_TITLE).handleId_;
	imgOnContinueHandle_ = resMng_.Load(ResourceManager::SRC::IMG_ON_CONTINUE).handleId_;
	imgOnRetryHandle_ = resMng_.Load(ResourceManager::SRC::IMG_ON_RETRY).handleId_;

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_OVER);
	AudioManager::GetInstance()->SetBgmVolume(120);

	VECTOR playerPos_;
	playerPos_ = { 0,150,0 };

	SetMouseDispFlag(true);

	MV1SetPosition(playerHandle_, playerPos_);
	
}

void OverScene::Update()
{
	auto const ins = InputManager::GetInstance();

	SelectUpdate();

}

void OverScene::Draw()
{
	MV1DrawModel(playerHandle_);

	if (selectCount_ == static_cast<int>(SELECT::CONTINUE))
	{ DrawGraph(500, 450, imgOnContinueHandle_, true);}
	else
	{ DrawGraph(500, 450, imgOffContinueHandle_, true);}

	if (selectCount_ == static_cast<int>(SELECT::RETRY))
	{ DrawGraph(500, 650, imgOnRetryHandle_, true);}
	else
	{ DrawGraph(500, 650, imgOffRetryHandle_, true);}

	if (selectCount_ == static_cast<int>(SELECT::TITLE))
	{ DrawGraph(500, 850, imgOnTitleHandle_, true);}
	else
	{ DrawGraph(500, 850, imgOffTitleHandle_, true);}


}

void OverScene::Release()
{
	DeleteGraph(imgOnTitleHandle_);
	DeleteGraph(imgOffTitleHandle_);
	DeleteGraph(imgOnContinueHandle_);
	DeleteGraph(imgOffContinueHandle_);
	DeleteGraph(imgOnRetryHandle_);
	DeleteGraph(imgOffRetryHandle_);
}

void OverScene::SelectChange(SELECT next)
{
	select_ = next;


	switch (next)
	{
	case SELECT::CONTINUE:
		AudioManager::GetInstance()->StopBGM();
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case SELECT::RETRY:
		AudioManager::GetInstance()->StopBGM();
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case SELECT::TITLE:
		AudioManager::GetInstance()->StopBGM();
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		break;
	}
}


void OverScene::SelectUpdate()
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
		AudioManager::GetInstance()->SetSeVolume(150);
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		SelectChange((SELECT)selectCount_);
	}


}