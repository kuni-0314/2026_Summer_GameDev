#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "OptionScene.h"

OptionScene::OptionScene()
{
}

OptionScene::~OptionScene()
{
}

void OptionScene::Init()
{
	ResourceManager& resMng = ResourceManager::GetInstance();
	fontHandle_ = resMng.Load(ResourceManager::SRC::FONT_MSGOTHIC).handleId_;
}

void OptionScene::Update()
{
}

void OptionScene::Draw()
{
	// デバッグ: 画面に何か描画されているか確認
	DrawFormatString(0, 0, GetColor(255, 0, 0), "fontHandle_: %d", fontHandle_);
	DrawFormatString(0, 20, GetColor(255, 0, 0), "ALL_ITEM_NUM_MAX: %d", ALL_ITEM_NUM_MAX);

	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		int y = 100 + i * 30;

		// 背景色と区別するため黒で描画
		if (fontHandle_ > 0)
		{
			DrawFormatStringToHandle(0, y, GetColor(255, 0, 0), fontHandle_, "%s", ALL_ITEM_NAME[i].data());
			//DrawFormatString(0, y, GetColor(255, 0, 0), "%s", ALL_ITEM_NAME[i].data());
		}
		else
		{
			// フォールバック: フォントが無効な場合はデフォルトフォントで描画
			DrawFormatString(0, y, GetColor(255, 0, 0), "%s", ALL_ITEM_NAME[i].data());
		}
	}
}

void OptionScene::Release()
{
}
