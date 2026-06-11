#include "ClearScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
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
}

void ClearScene::Update()
{
	auto const ins = InputManager::GetInstance();

	if (ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	
}

void ClearScene::Draw()
{
	DrawString(100, 100, "CLEAR", 0xffffff);
}

void ClearScene::Release()
{
}
