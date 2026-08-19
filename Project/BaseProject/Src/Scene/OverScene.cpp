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

	// RETRY Å` TITLE
	minIndex = static_cast<int>(SELECT::RETRY);
	maxIndex = static_cast<int>(SELECT::TITLE);

	// èâä˙ëIëÇRETRYÇ…Ç∑ÇÈ
	select_ = SELECT::RETRY;

	playerHandle_ = resMng_.Load(
		ResourceManager::SRC::PLAYER_GAMEOVER).handleId_;

	imgOffTitleHandle_ = resMng_.Load(
		ResourceManager::SRC::IMG_OFF_TITLE).handleId_;

	imgOffRetryHandle_ = resMng_.Load(
		ResourceManager::SRC::IMG_OFF_RETRY).handleId_;

	imgOnTitleHandle_ = resMng_.Load(
		ResourceManager::SRC::IMG_ON_TITLE).handleId_;

	imgOnRetryHandle_ = resMng_.Load(
		ResourceManager::SRC::IMG_ON_RETRY).handleId_;

	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_OVER);
	AudioManager::GetInstance()->SetBgmVolume(120);

	VECTOR playerPos_;
	playerPos_ = { 0, 150, 0 };

	SetMouseDispFlag(true);

	MV1SetPosition(playerHandle_, playerPos_);

	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
}

void OverScene::Update()
{
	SelectUpdate();
}

void OverScene::Draw()
{
	MV1DrawModel(playerHandle_);

	// RETRY
	if (selectCount_ == static_cast<int>(SELECT::RETRY))
	{
		DrawGraph(500, 550, imgOnRetryHandle_, true);
	}
	else
	{
		DrawGraph(500, 550, imgOffRetryHandle_, true);
	}

	// TITLE
	if (selectCount_ == static_cast<int>(SELECT::TITLE))
	{
		DrawGraph(500, 750, imgOnTitleHandle_, true);
	}
	else
	{
		DrawGraph(500, 750, imgOffTitleHandle_, true);
	}
}

void OverScene::Release()
{
	DeleteGraph(imgOnTitleHandle_);
	DeleteGraph(imgOffTitleHandle_);

	DeleteGraph(imgOnRetryHandle_);
	DeleteGraph(imgOffRetryHandle_);
}

void OverScene::SelectChange(SELECT next)
{
	select_ = next;

	switch (next)
	{
	case SELECT::RETRY:

		AudioManager::GetInstance()->StopBGM();

		// ÉQÅ[ÉÄÇç≈èâÇ©ÇÁÇ‚ÇËíºÇ∑
		sceMng_.ResetContinue();
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

	// è„
	if (ins->IsTrgDown(KEY_INPUT_UP) ||
		ins->IsGamepadTrgUp(InputManager::PadInput::Up, 0))
	{
		AudioManager::GetInstance()->SetSeVolume(150);
		AudioManager::GetInstance()->PlaySE(
			SoundID::SE_TITLE_SELECT);

		selectCount_--;

		if (selectCount_ < minIndex)
		{
			selectCount_ = maxIndex;
		}
	}

	// â∫
	if (ins->IsTrgDown(KEY_INPUT_DOWN) ||
		ins->IsGamepadTrgDown(InputManager::PadInput::Down, 0))
	{
		AudioManager::GetInstance()->SetSeVolume(150);
		AudioManager::GetInstance()->PlaySE(
			SoundID::SE_TITLE_SELECT);

		selectCount_++;

		if (selectCount_ > maxIndex)
		{
			selectCount_ = minIndex;
		}
	}

	// åàíË
	if (ins->IsTrgDown(KEY_INPUT_SPACE) ||
		ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))
	{
		AudioManager::GetInstance()->SetSeVolume(150);
		AudioManager::GetInstance()->PlaySE(
			SoundID::SE_TITLE_DECISION);

		SelectChange(static_cast<SELECT>(selectCount_));
	}
}