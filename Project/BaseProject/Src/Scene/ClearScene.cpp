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
	playerHandle_ = resMng_.Load(ResourceManager::SRC::PLAYER_GAMEOVER).handleId_;
	imgOnTitleHandle_ = resMng_.Load(ResourceManager::SRC::IMG_ON_TITLE).handleId_;
	AudioManager::GetInstance()->LoadSceneSound(LoadScene::TITLE);
	AudioManager::GetInstance()->PlayBGM(SoundID::BGM_OVER);
	AudioManager::GetInstance()->SetBgmVolume(120);

	VECTOR playerPos_;
	playerPos_ = { 0,150,0 };

	SetMouseDispFlag(true);

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
		AudioManager::GetInstance()->PlaySE(SoundID::SE_TITLE_DECISION);
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	
}

void ClearScene::Draw()
{
	DrawGraph(500, 450, imgOnTitleHandle_, true);
	MV1DrawModel(playerHandle_);
	DrawString(100, 100, "CLEAR", 0xffffff);
}

void ClearScene::Release()
{
	DeleteGraph(imgOnTitleHandle_);
}
