#include <algorithm>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Sound/AudioManager.h"
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
	// カテゴリーはメイン、項目はサブを使用する
	mainFontHandle_ = CreateFontToHandle("MS Mincho", 48, -1, DX_FONTTYPE_ANTIALIASING);
	subFontHandle_ = CreateFontToHandle("MS Mincho", 32, -1, DX_FONTTYPE_ANTIALIASING);

	LoadOptionValues();	

	CalculateItemDisplayInfo();
		
	backgroundHandle_ = resMng_.Load(ResourceManager::SRC::OPTION_BACKGROUND).handleId_;
	
	// スライダー用画像の読み込み
	sliderFrameHandle_ = resMng_.Load(ResourceManager::SRC::SLIDER_FRAME).handleId_;
	sliderKnobHandle_ = resMng_.Load(ResourceManager::SRC::SLIDER_KNOB).handleId_;
	
	// チェックボックス用画像の読み込み
	// TODO: ResourceManager::SRC に追加する必要があります
	// checkboxOnHandle_ = resMng_.Load(ResourceManager::SRC::CHECKBOX_ON).handleId_;
	// checkboxOffHandle_ = resMng_.Load(ResourceManager::SRC::CHECKBOX_OFF).handleId_;
	
	// ボタン用画像の読み込み
	// TODO: ResourceManager::SRC に追加する必要があります
	// buttonApplyHandle_ = resMng_.Load(ResourceManager::SRC::BUTTON_APPLY).handleId_;
	// buttonExitHandle_ = resMng_.Load(ResourceManager::SRC::BUTTON_EXIT).handleId_;
}

void OptionScene::Update()
{
	auto ins = InputManager::GetInstance();

	// スクロール処理
	int wheelDelta = ins->GetMouseWheel();
	scrollOffset_ += wheelDelta;

	// キーボードによるスクロール
	if (ins->IsNew(KEY_INPUT_DOWN))
	{
		scrollOffset_ -= 10;
	}
	if (ins->IsNew(KEY_INPUT_UP))
	{
		scrollOffset_ += 10;
	}

	// マウス座標を取得
	int cursorX, cursorY;
	ins->GetMousePos(cursorX, cursorY);

	CalculateItemDisplayInfo();

	// ホバー判定
	currentItemNum_ = -1;

	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		const auto& info = itemDisplayInfos_[i];

		if (!info.isCategory && 
			cursorX >= info.left && cursorX <= info.right && 
			cursorY >= info.top && cursorY <= info.bottom)
		{
			currentItemNum_ = i;
			break;
		}
	}

	if (currentItemNum_ != -1)
	{
		lastItemNum_ = currentItemNum_;
	}

	// 右側のコントロール領域でのインタラクション
	if (lastItemNum_ >= 0 && lastItemNum_ < ALL_ITEM_NUM_MAX)
	{
		const auto& info = itemDisplayInfos_[lastItemNum_];
		const int controlDisplayX = Application::SCREEN_SIZE_X * 3 / 4;
		const int controlDisplayY = Application::SCREEN_SIZE_Y / 4;

		if (!info.isCategory)
		{
			switch (ITEM_TYPES[lastItemNum_])
			{
			case SLIDER:
				UpdateSlider(info.itemIndex, controlDisplayX, controlDisplayY, cursorX, cursorY);
				break;
			case DROPDOWN:
				UpdateDropdown(info.itemIndex, controlDisplayX, controlDisplayY, cursorX, cursorY);
				break;
			case CHECKBOX:
				// チェックボックスのクリック処理
				if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
				{
					int checkboxX = controlDisplayX;
					int checkboxY = controlDisplayY;
					int checkboxSize = 48;
					
					if (cursorX >= checkboxX - checkboxSize / 2 && cursorX <= checkboxX + checkboxSize / 2 &&
						cursorY >= checkboxY - checkboxSize / 2 && cursorY <= checkboxY + checkboxSize / 2)
					{
						bool value = GetItemValueBool(info.itemIndex);
						SetItemValueBool(info.itemIndex, !value);
					}
				}
				break;
			}
		}
	}

	// 適用・終了ボタンの更新
	UpdateButtons(cursorX, cursorY);

	// ESCキーで終了
	if (ins->IsTrgDown(KEY_INPUT_ESCAPE))
	{
		// シーン遷移処理（必要に応じて実装）
		// sceneManager.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void OptionScene::UpdateButtons(int cursorX, int cursorY)
{
	auto ins = InputManager::GetInstance();
	
	const int buttonWidth = 200;
	const int buttonHeight = 60;
	const int buttonY = Application::SCREEN_SIZE_Y - 100;
	const int applyButtonX = Application::SCREEN_SIZE_X / 2 - 120;
	const int exitButtonX = Application::SCREEN_SIZE_X / 2 + 120;

	// 適用ボタンのホバー判定
	buttonState_.isApplyHovered = (cursorX >= applyButtonX - buttonWidth / 2 && 
									cursorX <= applyButtonX + buttonWidth / 2 &&
									cursorY >= buttonY - buttonHeight / 2 && 
									cursorY <= buttonY + buttonHeight / 2);

	// 終了ボタンのホバー判定
	buttonState_.isExitHovered = (cursorX >= exitButtonX - buttonWidth / 2 && 
								   cursorX <= exitButtonX + buttonWidth / 2 &&
								   cursorY >= buttonY - buttonHeight / 2 && 
								   cursorY <= buttonY + buttonHeight / 2);

	// クリック処理
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		if (buttonState_.isApplyHovered)
		{
			// 適用ボタン
			ApplyOptionValues();
		}
		else if (buttonState_.isExitHovered)
		{
			// 終了ボタン
			SaveOptionValues();
			// シーン遷移処理（必要に応じて実装）
			// sceneManager.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void OptionScene::UpdateSlider(int itemIndex, int x, int y, int cursorX, int cursorY)
{
	auto ins = InputManager::GetInstance();
	
	const int sliderWidth = 400;
	const int sliderHeight = 20;
	const int knobSize = 30;
	
	int sliderLeft = x - sliderWidth / 2;
	int sliderRight = x + sliderWidth / 2;
	int sliderTop = y - sliderHeight / 2;
	int sliderBottom = y + sliderHeight / 2;

	// ドラッグ開始
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		if (cursorX >= sliderLeft - knobSize / 2 && cursorX <= sliderRight + knobSize / 2 &&
			cursorY >= sliderTop - knobSize / 2 && cursorY <= sliderBottom + knobSize / 2)
		{
			sliderState_.isDragging = true;
			sliderState_.dragItemIndex = itemIndex;
		}
	}

	// ドラッグ中
	if (sliderState_.isDragging && sliderState_.dragItemIndex == itemIndex)
	{
		if (ins->IsMouseNew(MOUSE_INPUT_LEFT))
		{
			// スライダーの値を更新
			float ratio = static_cast<float>(cursorX - sliderLeft) / sliderWidth;
			ratio = std::clamp(ratio, 0.0f, 1.0f);
			
			// 項目に応じた範囲で値を設定
			// TODO: 各項目の最小値・最大値を定義する必要があります
			float value = ratio * 100.0f; // 仮に0-100の範囲
			SetItemValueFloat(itemIndex, value);
		}
		else
		{
			// ドラッグ終了
			sliderState_.isDragging = false;
			sliderState_.dragItemIndex = -1;
		}
	}
}

void OptionScene::UpdateDropdown(int itemIndex, int x, int y, int cursorX, int cursorY)
{
	auto ins = InputManager::GetInstance();
	
	const int dropdownWidth = 300;
	const int dropdownHeight = 40;
	const int optionHeight = 35;
	
	// ドロップダウンのボタン領域
	int buttonLeft = x - dropdownWidth / 2;
	int buttonRight = x + dropdownWidth / 2;
	int buttonTop = y - dropdownHeight / 2;
	int buttonBottom = y + dropdownHeight / 2;

	// ドロップダウンのクリック判定
	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		if (cursorX >= buttonLeft && cursorX <= buttonRight &&
			cursorY >= buttonTop && cursorY <= buttonBottom)
		{
			// 開閉切り替え
			if (dropdownState_.isOpen && dropdownState_.openItemIndex == itemIndex)
			{
				dropdownState_.isOpen = false;
				dropdownState_.openItemIndex = -1;
			}
			else
			{
				dropdownState_.isOpen = true;
				dropdownState_.openItemIndex = itemIndex;
			}
		}
		else if (dropdownState_.isOpen && dropdownState_.openItemIndex == itemIndex)
		{
			// 選択肢のクリック判定
			const auto& options = GetDropdownOptions(itemIndex);
			for (size_t i = 0; i < options.size(); i++)
			{
				int optionTop = buttonBottom + static_cast<int>(i) * optionHeight;
				int optionBottom = optionTop + optionHeight;
				
				if (cursorX >= buttonLeft && cursorX <= buttonRight &&
					cursorY >= optionTop && cursorY <= optionBottom)
				{
					// 選択肢を設定
					SetItemValueInt(itemIndex, static_cast<int>(i));
					dropdownState_.isOpen = false;
					dropdownState_.openItemIndex = -1;
					break;
				}
			}
			
			// 外側をクリックしたら閉じる
			dropdownState_.isOpen = false;
			dropdownState_.openItemIndex = -1;
		}
	}

	// ホバー判定
	if (dropdownState_.isOpen && dropdownState_.openItemIndex == itemIndex)
	{
		const auto& options = GetDropdownOptions(itemIndex);
		dropdownState_.hoveredOption = -1;
		
		for (size_t i = 0; i < options.size(); i++)
		{
			int optionTop = buttonBottom + static_cast<int>(i) * optionHeight;
			int optionBottom = optionTop + optionHeight;
			
			if (cursorX >= buttonLeft && cursorX <= buttonRight &&
				cursorY >= optionTop && cursorY <= optionBottom)
			{
				dropdownState_.hoveredOption = static_cast<int>(i);
				break;
			}
		}
	}
}

void OptionScene::Draw()
{
	// 一時スクリーンにメイン画面をコピー
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	int backgroundWidth_, backgroundHeight_;
	GetGraphSize(backgroundHandle_, &backgroundWidth_, &backgroundHeight_);
	DrawGraph(Application::SCREEN_SIZE_X / 2 - backgroundWidth_ / 2, Application::SCREEN_SIZE_Y / 2 - backgroundHeight_ / 2, backgroundHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 各項目を描画
	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		const auto& info = itemDisplayInfos_[i];

		bool isFill = (currentItemNum_ == i);
		unsigned int boxColor = GetColor(127, 127, 127);
		unsigned int textColor;

		if (info.isCategory)
		{
			textColor = GetColor(50, 50, 50);
		}
		else
		{
			textColor = isFill ? GetColor(255, 255, 255) : GetColor(0, 0, 0);
		}

		if (!info.isCategory && lastItemNum_ == i)
		{
			DrawBox(info.left, info.top, info.right, info.bottom, boxColor, isFill);
		}

		DrawFormatStringToHandle(info.textX, info.y, textColor, info.fontHandle, "%s", ITEM_NAMES[i].data());
	}

	// 右側のコントロール描画
	if (lastItemNum_ >= 0 && lastItemNum_ < ALL_ITEM_NUM_MAX)
	{
		const auto& info = itemDisplayInfos_[lastItemNum_];
		const int controlDisplayX = Application::SCREEN_SIZE_X * 3 / 4;
		const int controlDisplayY = Application::SCREEN_SIZE_Y / 4;

		if (!info.isCategory)
		{
			switch (ITEM_TYPES[lastItemNum_])
			{
			case CHECKBOX:
				{
					bool value = GetItemValueBool(info.itemIndex);
					int checkboxSize = 48;
					
					if (value && checkboxOnHandle_ > 0)
					{
						// ON画像を描画
						DrawRotaGraph(controlDisplayX, controlDisplayY, 1.0, 0.0, checkboxOnHandle_, TRUE);
					}
					else if (!value && checkboxOffHandle_ > 0)
					{
						// OFF画像を描画
						DrawRotaGraph(controlDisplayX, controlDisplayY, 1.0, 0.0, checkboxOffHandle_, TRUE);
					}
					else
					{
						// 画像がない場合は矩形で描画
						unsigned int color = value ? GetColor(0, 200, 0) : GetColor(200, 200, 200);
						DrawBox(controlDisplayX - checkboxSize / 2, controlDisplayY - checkboxSize / 2,
								controlDisplayX + checkboxSize / 2, controlDisplayY + checkboxSize / 2,
								color, TRUE);
						DrawBox(controlDisplayX - checkboxSize / 2, controlDisplayY - checkboxSize / 2,
								controlDisplayX + checkboxSize / 2, controlDisplayY + checkboxSize / 2,
								GetColor(0, 0, 0), FALSE);
						
						if (value)
						{
							// チェックマークを描画
							DrawLine(controlDisplayX - 15, controlDisplayY, controlDisplayX - 5, controlDisplayY + 10, GetColor(255, 255, 255), 3);
							DrawLine(controlDisplayX - 5, controlDisplayY + 10, controlDisplayX + 15, controlDisplayY - 10, GetColor(255, 255, 255), 3);
						}
					}
				}
				break;
			case SLIDER:
				DrawSlider(info.itemIndex, controlDisplayX, controlDisplayY);
				break;
			case DROPDOWN:
				DrawDropdown(info.itemIndex, controlDisplayX, controlDisplayY);
				break;
			}
		}
	}

	// 適用・終了ボタンを描画
	DrawButtons();

	// 最終結果をメイン画面に描画
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, tempScreen, false);

	// 一時スクリーンを削除
	DeleteGraph(tempScreen);
}

void OptionScene::DrawButtons()
{
	const int buttonWidth = 200;
	const int buttonHeight = 60;
	const int buttonY = Application::SCREEN_SIZE_Y - 100;
	const int applyButtonX = Application::SCREEN_SIZE_X / 2 - 120;
	const int exitButtonX = Application::SCREEN_SIZE_X / 2 + 120;

	// 適用ボタン描画
	if (buttonApplyHandle_ > 0)
	{
		DrawRotaGraph(applyButtonX, buttonY, 1.0, 0.0, buttonApplyHandle_, TRUE);
	}
	else
	{
		unsigned int applyColor = buttonState_.isApplyHovered ? GetColor(100, 200, 100) : GetColor(150, 150, 150);
		DrawBox(applyButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				applyButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				applyColor, TRUE);
		DrawBox(applyButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				applyButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				GetColor(0, 0, 0), FALSE);
		
		const char* applyText = "適用";
		int textWidth = GetDrawStringWidthToHandle(applyText, static_cast<int>(strlen(applyText)), subFontHandle_);
		DrawStringToHandle(applyButtonX - textWidth / 2, buttonY - 16, applyText, GetColor(255, 255, 255), subFontHandle_);
	}

	// 終了ボタン描画
	if (buttonExitHandle_ > 0)
	{
		DrawRotaGraph(exitButtonX, buttonY, 1.0, 0.0, buttonExitHandle_, TRUE);
	}
	else
	{
		unsigned int exitColor = buttonState_.isExitHovered ? GetColor(200, 100, 100) : GetColor(150, 150, 150);
		DrawBox(exitButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				exitButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				exitColor, TRUE);
		DrawBox(exitButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				exitButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				GetColor(0, 0, 0), FALSE);
		
		const char* exitText = "終了";
		int textWidth = GetDrawStringWidthToHandle(exitText, static_cast<int>(strlen(exitText)), subFontHandle_);
		DrawStringToHandle(exitButtonX - textWidth / 2, buttonY - 16, exitText, GetColor(255, 255, 255), subFontHandle_);
	}
}

void OptionScene::DrawSlider(int itemIndex, int x, int y)
{
	const int sliderWidth = 400;
	const int sliderHeight = 20;
	const int knobSize = 30;
	
	int sliderLeft = x - sliderWidth / 2;
	int sliderTop = y - sliderHeight / 2;

	// スライダーフレーム描画
	if (sliderFrameHandle_ > 0)
	{
		DrawExtendGraph(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, sliderFrameHandle_, TRUE);
	}
	else
	{
		DrawBox(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, GetColor(100, 100, 100), TRUE);
		DrawBox(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, GetColor(200, 200, 200), FALSE);
	}

	// 値を取得して位置を計算
	float value = GetItemValueFloat(itemIndex);
	float ratio = value / 100.0f;
	ratio = std::clamp(ratio, 0.0f, 1.0f);
	
	int knobX = sliderLeft + static_cast<int>(sliderWidth * ratio);
	int knobY = y;

	// つまみ描画
	if (sliderKnobHandle_ > 0)
	{
		DrawRotaGraph(knobX, knobY, 1.0, 0.0, sliderKnobHandle_, TRUE);
	}
	else
	{
		DrawCircle(knobX, knobY, knobSize / 2, GetColor(255, 255, 255), TRUE);
		DrawCircle(knobX, knobY, knobSize / 2, GetColor(0, 0, 0), FALSE);
	}

	// 値を右側に表示
	DrawFormatStringToHandle(x + sliderWidth / 2 + 20, y - 16, GetColor(0, 0, 0), subFontHandle_, "%.1f", value);
}

void OptionScene::DrawDropdown(int itemIndex, int x, int y)
{
	const int dropdownWidth = 300;
	const int dropdownHeight = 40;
	const int optionHeight = 35;
	
	int buttonLeft = x - dropdownWidth / 2;
	int buttonTop = y - dropdownHeight / 2;

	// 現在の選択値を取得
	int currentValue = GetItemValueInt(itemIndex);
	const auto& options = GetDropdownOptions(itemIndex);
	
	std::string displayText = (currentValue >= 0 && currentValue < static_cast<int>(options.size())) 
		? options[currentValue] : "未選択";

	// ドロップダウンボタン描画
	DrawBox(buttonLeft, buttonTop, buttonLeft + dropdownWidth, buttonTop + dropdownHeight, GetColor(200, 200, 200), TRUE);
	DrawBox(buttonLeft, buttonTop, buttonLeft + dropdownWidth, buttonTop + dropdownHeight, GetColor(0, 0, 0), FALSE);
	
	DrawFormatStringToHandle(buttonLeft + 10, buttonTop + 5, GetColor(0, 0, 0), subFontHandle_, "%s", displayText.c_str());
	DrawFormatStringToHandle(buttonLeft + dropdownWidth - 30, buttonTop + 5, GetColor(0, 0, 0), subFontHandle_, "▼");

	// 展開時の選択肢描画
	if (dropdownState_.isOpen && dropdownState_.openItemIndex == itemIndex)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			int optionTop = buttonTop + dropdownHeight + static_cast<int>(i) * optionHeight;
			bool isHovered = (dropdownState_.hoveredOption == static_cast<int>(i));
			
			unsigned int bgColor = isHovered ? GetColor(150, 150, 255) : GetColor(220, 220, 220);
			DrawBox(buttonLeft, optionTop, buttonLeft + dropdownWidth, optionTop + optionHeight, bgColor, TRUE);
			DrawBox(buttonLeft, optionTop, buttonLeft + dropdownWidth, optionTop + optionHeight, GetColor(0, 0, 0), FALSE);
			
			DrawFormatStringToHandle(buttonLeft + 10, optionTop + 2, GetColor(0, 0, 0), subFontHandle_, "%s", options[i].c_str());
		}
	}
}

void OptionScene::Release()
{
	// 設定を保存
	SaveOptionValues();

	// フォントハンドルを削除
	DeleteFontToHandle(mainFontHandle_);
	DeleteFontToHandle(subFontHandle_);
}

void OptionScene::LoadOptionValues()
{
	auto& appIns = Application::GetInstance();
	auto inputIns = InputManager::GetInstance();
	auto audioIns = AudioManager::GetInstance();

	// コメントアウトの分は未実装
	//currentAudioDeviceNum_ = audioIns->GetCurrentDeviceId();
	//mVolume_ = audioIns->GetMasterVolume();
	//bVolume_ = audioIns->GetBGMVolume();
	//sVolume_ = audioIns->GetSEVolume();
	//isMute_ = audioIns->IsMute();
	//brightness_ = appIns.GetBrightness();
	//enableShader_ = appIns.IsShaderEnabled();
	//fpsLimit_ = appIns.GetFPSLimit();
	//isInvertXAxis_ = appIns.IsInvertXAxis();
	//isInvertYAxis_ = appIns.IsInvertYAxis();
	//holdThreshold_ = inputIns->GetHoldThreshold();
	//isAcceptKeyboardInput_ = inputIns->IsAcceptKeyboardInput();
	//mouseSensitivity_ = inputIns->GetMouseSensitivity();
	wheelSensitivity_ = static_cast<float>(inputIns->GetMouseWheelSensitivity());
	//leftStickSensitivity_ = inputIns->GetLeftStickSensitivity();
	//leftStickDeadZone_ = inputIns->GetLeftStickDeadZone();
	//rightStickSensitivity_ = inputIns->GetRightStickSensitivity();
	//rightStickDeadZone_ = inputIns->GetRightStickDeadZone();
	//enableVibration_ = inputIns->IsVibrationEnabled();
	//vibrationStrength_ = inputIns->GetVibrationStrength();
	//currentColorAccessibilityNum_ = inputIns->GetColorAccessibilityNum();
	//isShowFPS_ = inputIns->IsShowFPS();
	//isShowMemoryUsage_ = inputIns->IsShowMemoryUsage();
	//isShowCollider_ = inputIns->IsShowCollider();
	//isShowXYZAxis_ = inputIns->IsShowXYZAxis();
	//isShowPlayerPosition_ = inputIns->IsShowPlayerPosition();
	//isShowBatteryStatus_ = inputIns->IsShowBatteryStatus();
}

void OptionScene::SaveOptionValues()
{
	auto& appIns = Application::GetInstance();
	auto inputIns = InputManager::GetInstance();
	auto audioIns = AudioManager::GetInstance();

	// 設定を各マネージャーに反映
	// コメントアウトの分は未実装
	//audioIns->SetCurrentDeviceId(currentAudioDeviceNum_);
	//audioIns->SetMasterVolume(mVolume_);
	//audioIns->SetBGMVolume(bVolume_);
	//audioIns->SetSEVolume(sVolume_);
	//audioIns->SetMute(isMute_);
	//appIns.SetBrightness(brightness_);
	//appIns.SetShaderEnabled(enableShader_);
	//appIns.SetFPSLimit(fpsLimit_);
	//appIns.SetInvertXAxis(isInvertXAxis_);
	//appIns.SetInvertYAxis(isInvertYAxis_);
	//inputIns->SetHoldThreshold(holdThreshold_);
	//inputIns->SetAcceptKeyboardInput(isAcceptKeyboardInput_);
	//inputIns->SetMouseSensitivity(mouseSensitivity_);
	inputIns->SetMouseWheelSensitivity(wheelSensitivity_);
	//inputIns->SetLeftStickSensitivity(leftStickSensitivity_);
	//inputIns->SetLeftStickDeadZone(leftStickDeadZone_);
	//inputIns->SetRightStickSensitivity(rightStickSensitivity_);
	//inputIns->SetRightStickDeadZone(rightStickDeadZone_);
	//inputIns->SetVibrationEnabled(enableVibration_);
	//inputIns->SetVibrationStrength(vibrationStrength_);
	//inputIns->SetColorAccessibilityNum(currentColorAccessibilityNum_);
	//inputIns->SetShowFPS(isShowFPS_);
	//inputIns->SetShowMemoryUsage(isShowMemoryUsage_);
	//inputIns->SetShowCollider(isShowCollider_);
	//inputIns->SetShowXYZAxis(isShowXYZAxis_);
	//inputIns->SetShowPlayerPosition(isShowPlayerPosition_);
	//inputIns->SetShowBatteryStatus(isShowBatteryStatus_);

	// TODO: 設定ファイルへの保存処理を実装
	// 例: SaveToFile("Config/Options.ini");
}

void OptionScene::ApplyOptionValues()
{
	// 設定を即座に反映する場合に使用
	SaveOptionValues();
}

void OptionScene::CalculateItemDisplayInfo()
{
	const int mainTextHeight = 48;
	const int subTextHeight = 32;
	const int hitboxMargin = 5;
	const int itemSpacing = 20;
	const int categorySpacing = 50;
	const int leftAreaCenterX = Application::SCREEN_SIZE_X / 4;

	int currentY = scrollOffset_;
	int actualItemIndex = 0;

	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		auto& info = itemDisplayInfos_[i];

		info.isCategory = (ITEM_TYPES[i] == CATEGORY_HEADER);
		info.textHeight = info.isCategory ? mainTextHeight : subTextHeight;
		info.fontHandle = info.isCategory ? mainFontHandle_ : subFontHandle_;

		if (!info.isCategory)
		{
			info.itemIndex = actualItemIndex++;
		}
		else
		{
			info.itemIndex = -1;
		}

		if (info.isCategory && i > 0)
		{
			currentY += categorySpacing;
		}

		info.y = currentY;
		info.textWidth = GetDrawFormatStringWidthToHandle(info.fontHandle, "%s", ITEM_NAMES[i].data());
		info.textX = leftAreaCenterX - info.textWidth / 2;
		info.left = info.textX - hitboxMargin;
		info.right = info.textX + info.textWidth + hitboxMargin;
		info.top = info.y - hitboxMargin;
		info.bottom = info.y + info.textHeight + hitboxMargin;

		currentY += info.textHeight + itemSpacing;
	}
}

void* OptionScene::GetItemValuePtr(int itemIndex)
{
	switch (itemIndex)
	{
	case IDX_OUTPUT_DEVICE:			return &currentAudioDeviceNum_;
	case IDX_MASTER_VOLUME:			return &mVolume_;
	case IDX_BGM_VOLUME:			return &bVolume_;
	case IDX_SE_VOLUME:				return &sVolume_;
	case IDX_ENABLE_MUTE:			return &isMute_;
	case IDX_BRIGHTNESS:			return &brightness_;
	case IDX_ENABLE_SHADER:			return &enableShader_;
	case IDX_FPS_LIMIT:				return &fpsLimit_;
	case IDX_INVERT_X_AXIS:			return &isInvertXAxis_;
	case IDX_INVERT_Y_AXIS:			return &isInvertYAxis_;
	case IDX_HOLD_THRESHOLD:		return &holdThreshold_;
	case IDX_ACCEPT_KEYBOARD_INPUT:	return &isAcceptKeyboardInput_;
	case IDX_MOUSE_SENSITIVITY:		return &mouseSensitivity_;
	case IDX_WHEEL_SENSITIVITY:		return &wheelSensitivity_;
	case IDX_LEFT_STICK_SENSITIVITY:	return &leftStickSensitivity_;
	case IDX_LEFT_STICK_DEAD_ZONE:		return &leftStickDeadZone_;
	case IDX_RIGHT_STICK_SENSITIVITY:	return &rightStickSensitivity_;
	case IDX_RIGHT_STICK_DEAD_ZONE:		return &rightStickDeadZone_;
	case IDX_ENABLE_VIBRATION:			return &enableVibration_;
	case IDX_VIBRATION_STRENGTH:		return &vibrationStrength_;
	case IDX_COLOR_ACCESSIBILITY:		return &currentColorAccessibilityNum_;
	case IDX_FPS:						return &isShowFPS_;
	case IDX_MEMORY_USAGE:				return &isShowMemoryUsage_;
	case IDX_COLLIDER:					return &isShowCollider_;
	case IDX_XYZ_AXIS:					return &isShowXYZAxis_;
	case IDX_PLAYER_POSITION:			return &isShowPlayerPosition_;
	case IDX_BATTERY_STATUS:			return &isShowBatteryStatus_;
	default: return nullptr;
	}
}

float OptionScene::GetItemValueFloat(int itemIndex)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return 0.0f;
	
	// TODO: 型に応じて適切にキャスト
	return *static_cast<float*>(ptr);
}

void OptionScene::SetItemValueFloat(int itemIndex, float value)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return;
	
	*static_cast<float*>(ptr) = value;
}

int OptionScene::GetItemValueInt(int itemIndex)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return 0;
	
	return *static_cast<int*>(ptr);
}

void OptionScene::SetItemValueInt(int itemIndex, int value)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return;
	
	*static_cast<int*>(ptr) = value;
}

bool OptionScene::GetItemValueBool(int itemIndex)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return false;
	
	return *static_cast<bool*>(ptr);
}

void OptionScene::SetItemValueBool(int itemIndex, bool value)
{
	void* ptr = GetItemValuePtr(itemIndex);
	if (!ptr) return;
	
	*static_cast<bool*>(ptr) = value;
}

const std::vector<std::string>& OptionScene::GetDropdownOptions(int itemIndex)
{
	// TODO: 各項目の選択肢を定義
	static std::vector<std::string> dummyOptions = {"選択肢1", "選択肢2", "選択肢3"};
	
	switch (itemIndex)
	{
	case IDX_OUTPUT_DEVICE:
		{
			static std::vector<std::string> devices = {"デバイス1", "デバイス2", "デバイス3"};
			return devices;
		}
	case IDX_FPS_LIMIT:
		{
			static std::vector<std::string> fpsOptions = {"30", "60", "120", "無制限"};
			return fpsOptions;
		}
	case IDX_COLOR_ACCESSIBILITY:
		{
			static std::vector<std::string> colorOptions = {"通常", "1型色覚", "2型色覚", "3型色覚"};
			return colorOptions;
		}
	default:
		return dummyOptions;
	}
}