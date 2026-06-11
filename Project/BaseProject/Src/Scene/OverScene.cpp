#include "OverScene.h"
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "../Application.h"

OverScene::OverScene()
{
}

OverScene::~OverScene()
{
}

void OverScene::Init()
{
}

void OverScene::Update()
{
	auto const ins = InputManager::GetInstance();

	if (ins->IsTrgDown(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void OverScene::Draw()
{
	DrawString(100, 100, "Ç∞Ç•Å[ÇﬁÇ®Å[ÇŒÅ[", 0xffffff);
}

void OverScene::Release()
{
}
