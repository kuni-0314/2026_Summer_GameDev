#include "ClearScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"


ClearScene::ClearScene(void)
{
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Init(void)
{
	//’Ç]ƒJƒƒ‰
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
}

void ClearScene::Update(void)
{
	auto const ins = InputManager::GetInstance();
	if (ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ClearScene::Draw(void)
{
}

void ClearScene::Release(void)
{
}
