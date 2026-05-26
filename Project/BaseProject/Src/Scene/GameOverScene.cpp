#include "GameOverScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"


GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	//’Ç]ƒJƒƒ‰
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
}

void GameOverScene::Update(void)
{
	auto const ins = InputManager::GetInstance();
	if (ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::Draw(void)
{
}

void GameOverScene::Release(void)
{
}
