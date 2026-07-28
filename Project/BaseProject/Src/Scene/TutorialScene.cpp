#include "TutorialScene.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"

TutorialScene::TutorialScene():
	imgTutorialMouse_(-1),
	imgTutorialPad_(-1),
	page_(0)
{
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Init()
{
	page_ = 0;

	// チュートリアル画像読み込み
	imgTutorialMouse_ = resMng_.Load(ResourceManager::SRC::IMG_TUTORIAL_KEYBOARD).handleId_;
	imgTutorialPad_ = resMng_.Load(ResourceManager::SRC::IMG_TUTORIAL_GAMEPAD).handleId_;
}

void TutorialScene::Update()
{
	auto const ins = InputManager::GetInstance();

	if (ins->IsTrgDown(KEY_INPUT_SPACE) || ins->IsGamepadTrgDown(InputManager::PadInput::A, 0))
	{
		page_++;
		if (page_ > 1)
		{
			sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void TutorialScene::Draw()
{
	if (page_ == 0)
	{
		DrawGraph(0, 0, imgTutorialMouse_, true);
	}
	else if (page_ == 1)
	{
		DrawGraph(0, 0, imgTutorialPad_, true);
	}
}

void TutorialScene::Release()
{
	DeleteGraph(imgTutorialMouse_);
	DeleteGraph(imgTutorialPad_);
}
