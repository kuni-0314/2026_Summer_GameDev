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
	categoryFontHandle_ = CreateFontToHandle("MS PMincho", 48, -1, DX_FONTTYPE_ANTIALIASING);
	itemFontHandle_ = CreateFontToHandle("MS PMincho", 32, -1, DX_FONTTYPE_ANTIALIASING);

	LoadOptionValues();	

	CalculateItemDisplayInfo();

	backgroundHandle_ = resMng_.Load(ResourceManager::SRC::OPTION_BACKGROUND).handleId_;
	
	// スライダー用画像の読み込み
	sliderFrameHandle_ = resMng_.Load(ResourceManager::SRC::SLIDER_FRAME).handleId_;
	sliderKnobHandle_ = resMng_.Load(ResourceManager::SRC::SLIDER_KNOB).handleId_;
	
	// チェックボックス用画像の読み込み
	 checkboxOnHandle_ = resMng_.Load(ResourceManager::SRC::CHECKBOX_ON).handleId_;
	 checkboxOffHandle_ = resMng_.Load(ResourceManager::SRC::CHECKBOX_OFF).handleId_;

	 // オプション枠
	 optionFrameHandle_ = resMng_.Load(ResourceManager::SRC::OPTION_FRAME).handleId_;

	// ボタン用画像の読み込み
	// TODO: ResourceManager::SRC に追加する必要があります
	 //buttonApplyHandle_ = resMng_.Load(ResourceManager::SRC::BUTTON_APPLY).handleId_;
	 //buttonExitHandle_ = resMng_.Load(ResourceManager::SRC::BUTTON_EXIT).handleId_;
}

void OptionScene::Update()
{
	auto ins = InputManager::GetInstance();

	// スクロール処理
	const float scrollSpeed = 10.0f; // スクロール速度
	int wheelDelta = ins->GetMouseWheel();
	scrollOffset_ += wheelDelta * scrollSpeed;

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

	// ホバー判定（currentItemNum_はホバー中の項目を示す）
	currentItemNum_ = -1;

	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		const auto& info = itemDisplayInfos_[i];

		if (!info.isCategory && 
			cursorX >= info.left && cursorX <= info.right && 
			cursorY >= info.top && cursorY <= info.bottom)
		{
			currentItemNum_ = i;
			
			// クリックされた場合のみ選択状態を更新
			if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
			{
				lastItemNum_ = i;
			}
			break;
		}
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
				UpdateCheckbox(info.itemIndex, controlDisplayX, controlDisplayY, cursorX, cursorY);
				break;
			}
		}
	}

	// 適用・終了ボタンの更新
	UpdateButtons(cursorX, cursorY);
}

void OptionScene::UpdateCheckbox(int itemIndex, int x, int y, int cursorX, int cursorY)
{
	auto ins = InputManager::GetInstance();

	if (ins->IsMouseTrgDown(MOUSE_INPUT_LEFT))
	{
		int checkboxX = x;
		int checkboxY = y;
		int checkboxSize = 48;

		if (cursorX >= checkboxX - checkboxSize / 2 && cursorX <= checkboxX + checkboxSize / 2 &&
			cursorY >= checkboxY - checkboxSize / 2 && cursorY <= checkboxY + checkboxSize / 2)
		{
			bool value = GetItemValueBool(itemIndex);
			SetItemValueBool(itemIndex, !value);
		}
	}
}

OptionScene::SliderConfig OptionScene::GetSliderConfig(int itemIndex) const
{
    switch (itemIndex)
    {
    case IDX_MASTER_VOLUME:
    case IDX_BGM_VOLUME:
    case IDX_SE_VOLUME:
        return { SliderConfig::ValueType::INT, 0.0f, 255.0f, 1.0f };
        
    case IDX_BRIGHTNESS:
        return { SliderConfig::ValueType::INT, 0.0f, 100.0f, 1.0f };
        
    case IDX_MOUSE_SENSITIVITY:
        return { SliderConfig::ValueType::INT, 1.0f, 20.0f, 1.0f };
        
    case IDX_WHEEL_SENSITIVITY:
        return { SliderConfig::ValueType::FLOAT, 1.0f, 100.0f, 1.0f };
        
    case IDX_LEFT_STICK_SENSITIVITY:
    case IDX_RIGHT_STICK_SENSITIVITY:
        return { SliderConfig::ValueType::FLOAT, 0.1f, 5.0f, 0.1f };
        
    case IDX_LEFT_STICK_DEAD_ZONE:
    case IDX_RIGHT_STICK_DEAD_ZONE:
        return { SliderConfig::ValueType::FLOAT, 0.0f, 1.0f, 0.05f };
        
    case IDX_VIBRATION_STRENGTH:
        return { SliderConfig::ValueType::FLOAT, 0.0f, 100.0f, 1.0f };
        
    case IDX_HOLD_THRESHOLD:
        return { SliderConfig::ValueType::INT, 1.0f, 120.0f, 1.0f };
        
    default:
        return { SliderConfig::ValueType::FLOAT, 0.0f, 100.0f, 1.0f };
    }
}

void OptionScene::UpdateSlider(int itemIndex, int x, int y, int cursorX, int cursorY)
{
	auto ins = InputManager::GetInstance();
	
	const int sliderWidth = 600;
	const int sliderHeight = 15;
	const int knobSize = 40;
	
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
			// スライダー設定を取得
			SliderConfig config = GetSliderConfig(itemIndex);
			
			// スライダーの値を更新
			float ratio = static_cast<float>(cursorX - sliderLeft) / sliderWidth;
			ratio = std::clamp(ratio, 0.0f, 1.0f);
			
			// 最小値～最大値の範囲で値を計算
			float rawValue = config.minValue + (config.maxValue - config.minValue) * ratio;
			
			// 刻み幅に合わせて丸める
			if (config.step > 0.0f)
			{
				rawValue = std::round(rawValue / config.step) * config.step;
			}
			
			// 型に応じて設定
			if (config.type == SliderConfig::ValueType::INT)
			{
				SetItemValueInt(itemIndex, static_cast<int>(rawValue));
			}
			else
			{
				SetItemValueFloat(itemIndex, rawValue);
			}
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
			//ApplyOptionValues();
			SaveOptionValues();
		}
		else if (buttonState_.isExitHovered)
		{
			// 終了ボタン
			//SaveOptionValues();
			// シーン遷移処理（必要に応じて実装）
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void OptionScene::Draw()
{
	// 一時スクリーンにメイン画面をコピー
	int tempScreen = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, false);
	SetDrawScreen(tempScreen);
	ClearDrawScreen();

	// 背景描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	int backgroundWidth_, backgroundHeight_;
	GetGraphSize(backgroundHandle_, &backgroundWidth_, &backgroundHeight_);
	DrawGraph(Application::SCREEN_SIZE_X / 2 - backgroundWidth_ / 2, Application::SCREEN_SIZE_Y / 2 - backgroundHeight_ / 2, backgroundHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 各項目を描画
	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		const auto& info = itemDisplayInfos_[i];

		// 選択中かホバー中かを判定
		bool isSelected = (lastItemNum_ == i);
		bool isHovered = (currentItemNum_ == i);
		// クリックの瞬間かどうか
		bool isClickMoment = isHovered && isSelected;
		
		unsigned int boxColor = GetColor(127, 127, 127);
		unsigned int textColor;

		if (info.isCategory)
		{
			textColor = GetColor(90, 90, 90);
		}
		else
		{
			// クリックの瞬間だけ黄色、それ以外は黒色
			textColor = isClickMoment ? GetColor(255, 255, 0) : GetColor(0, 0, 0);
		}

		// 背景の描画（文字の前に描画）
		if (!info.isCategory)
		{
			if (isClickMoment)
			{
				// クリックの瞬間：塗りつぶし
				DrawBox(info.left, info.top, info.right, info.bottom, boxColor, true);
			}
			else if (isHovered)
			{
				// ホバー中：枠線のみ
				DrawBox(info.left, info.top, info.right, info.bottom, boxColor, false);
			}
		}

		// 文字を描画
		DrawFormatStringToHandle(info.textX, info.y, textColor, info.fontHandle, "%s", ITEM_NAMES[i].data());

		// フレーム画像を描画（文字の後に描画）
		if (!info.isCategory && isSelected)
		{
			int frameWidth, frameHeight;
			GetGraphSize(optionFrameHandle_, &frameWidth, &frameHeight);
			int leftTopX = info.left - FRAME_OFFSET - frameWidth * FRAME_SCALE / 2;
			int leftTopY = info.top - FRAME_OFFSET - frameHeight * FRAME_SCALE / 2;
			int rightTopX = info.right + FRAME_OFFSET + frameWidth * FRAME_SCALE / 2;
			int rightTopY = info.top - FRAME_OFFSET - frameHeight * FRAME_SCALE / 2;
			int leftBottomX = info.left - FRAME_OFFSET - frameWidth * FRAME_SCALE / 2;
			int leftBottomY = info.bottom + FRAME_OFFSET + frameHeight * FRAME_SCALE / 2;
			int rightBottomX = info.right + FRAME_OFFSET + frameWidth * FRAME_SCALE / 2;
			int rightBottomY = info.bottom + FRAME_OFFSET + frameHeight * FRAME_SCALE / 2;

			DrawRotaGraph(leftTopX, leftTopY, FRAME_SCALE, 0.0f, optionFrameHandle_, true);
			DrawRotaGraph(rightTopX, rightTopY, FRAME_SCALE, DX_PI_F / 2.0f, optionFrameHandle_, true);
			DrawRotaGraph(leftBottomX, leftBottomY, FRAME_SCALE, -DX_PI_F / 2.0f, optionFrameHandle_, true);
			DrawRotaGraph(rightBottomX, rightBottomY, FRAME_SCALE, DX_PI_F, optionFrameHandle_, true);
		}
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
				DrawCheckbox(info.itemIndex, controlDisplayX, controlDisplayY);
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

void OptionScene::DrawCheckbox(int itemIndex, int x, int y)
{
	bool value = GetItemValueBool(itemIndex);
	int checkboxSize = 48;

	if (value && checkboxOnHandle_ > 0)
	{
		// ON画像を描画
		DrawRotaGraph(x, y, 1.0, 0.0, checkboxOnHandle_, true);
	}
	else if (!value && checkboxOffHandle_ > 0)
	{
		// OFF画像を描画
		DrawRotaGraph(x, y, 1.0, 0.0, checkboxOffHandle_, true);
	}
	else
	{
		// 画像がない場合は矩形で描画
		unsigned int color = value ? GetColor(0, 200, 0) : GetColor(200, 200, 200);
		DrawBox(x - checkboxSize / 2, y - checkboxSize / 2,
			x + checkboxSize / 2, y + checkboxSize / 2,
			color, true);
		DrawBox(x - checkboxSize / 2, y - checkboxSize / 2,
			x + checkboxSize / 2, y + checkboxSize / 2,
			GetColor(0, 0, 0), false);

		if (value)
		{
			// チェックマークを描画
			DrawLine(x - 15, y, x - 5, y + 10, GetColor(255, 255, 255), 3);
			DrawLine(x - 5, y + 10, x + 15, y - 10, GetColor(255, 255, 255), 3);
		}
	}
}

void OptionScene::DrawSlider(int itemIndex, int x, int y)
{
    const int sliderWidth = 600;
    const int sliderHeight = 15;
    const int knobSize = 40;
    
    int sliderLeft = x - sliderWidth / 2;
    int sliderTop = y - sliderHeight / 2;

    // スライダーフレーム描画
    if (sliderFrameHandle_ > 0)
    {
        DrawExtendGraph(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, sliderFrameHandle_, true);
    }
    else
    {
        DrawBox(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, GetColor(100, 100, 100), true);
        DrawBox(sliderLeft, sliderTop, sliderLeft + sliderWidth, sliderTop + sliderHeight, GetColor(200, 200, 200), false);
    }

    // スライダー設定を取得
    SliderConfig config = GetSliderConfig(itemIndex);
    
    // 値を取得
    float value;
    if (config.type == SliderConfig::ValueType::INT)
    {
        value = static_cast<float>(GetItemValueInt(itemIndex));
    }
    else
    {
        value = GetItemValueFloat(itemIndex);
    }
    
    // 位置を計算(最小値～最大値の範囲で正規化)
    float ratio = (value - config.minValue) / (config.maxValue - config.minValue);
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    
    int knobX = sliderLeft + static_cast<int>(sliderWidth * ratio);
    int knobY = y;

    // つまみ描画
    if (sliderKnobHandle_ > 0)
    {
        DrawRotaGraph(knobX, knobY, 1.0, 0.0, sliderKnobHandle_, true);
    }
    else
    {
        DrawCircle(knobX, knobY, knobSize / 2, GetColor(255, 255, 255), true);
        DrawCircle(knobX, knobY, knobSize / 2, GetColor(0, 0, 0), false);
    }

    // 値を右側に表示
    if (config.type == SliderConfig::ValueType::INT)
    {
        DrawFormatStringToHandle(x + sliderWidth / 2 + 20, y - 16, GetColor(0, 0, 0), itemFontHandle_, "%d", static_cast<int>(value));
    }
    else
    {
        DrawFormatStringToHandle(x + sliderWidth / 2 + 20, y - 16, GetColor(0, 0, 0), itemFontHandle_, "%.1f", value);
    }
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
	DrawBox(buttonLeft, buttonTop, buttonLeft + dropdownWidth, buttonTop + dropdownHeight, GetColor(200, 200, 200), true);
	DrawBox(buttonLeft, buttonTop, buttonLeft + dropdownWidth, buttonTop + dropdownHeight, GetColor(0, 0, 0), false);
	
	DrawFormatStringToHandle(buttonLeft + 10, buttonTop + 5, GetColor(0, 0, 0), itemFontHandle_, "%s", displayText.c_str());
	DrawFormatStringToHandle(buttonLeft + dropdownWidth - 35, buttonTop + 5, GetColor(0, 0, 0), itemFontHandle_, "▼");

	// 展開時の選択肢描画
	if (dropdownState_.isOpen && dropdownState_.openItemIndex == itemIndex)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			int optionTop = buttonTop + dropdownHeight + static_cast<int>(i) * optionHeight;
			bool isHovered = (dropdownState_.hoveredOption == static_cast<int>(i));
			
			unsigned int bgColor = isHovered ? GetColor(150, 150, 255) : GetColor(220, 220, 220);
			DrawBox(buttonLeft, optionTop, buttonLeft + dropdownWidth, optionTop + optionHeight, bgColor, true);
			DrawBox(buttonLeft, optionTop, buttonLeft + dropdownWidth, optionTop + optionHeight, GetColor(0, 0, 0), false);
			
			DrawFormatStringToHandle(buttonLeft + 10, optionTop + 2, GetColor(0, 0, 0), itemFontHandle_, "%s", options[i].c_str());
		}
	}
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
		DrawRotaGraph(applyButtonX, buttonY, 1.0, 0.0, buttonApplyHandle_, true);
	}
	else
	{
		unsigned int applyColor = buttonState_.isApplyHovered ? GetColor(100, 200, 100) : GetColor(150, 150, 150);
		DrawBox(applyButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				applyButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				applyColor, true);
		DrawBox(applyButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				applyButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				GetColor(0, 0, 0), false);
		
		const char* applyText = "適用";
		int textWidth = GetDrawStringWidthToHandle(applyText, static_cast<int>(strlen(applyText)), itemFontHandle_);
		DrawStringToHandle(applyButtonX - textWidth / 2, buttonY - 16, applyText, GetColor(255, 255, 255), itemFontHandle_);
	}

	// 終了ボタン描画
	if (buttonExitHandle_ > 0)
	{
		DrawRotaGraph(exitButtonX, buttonY, 1.0, 0.0, buttonExitHandle_, true);
	}
	else
	{
		unsigned int exitColor = buttonState_.isExitHovered ? GetColor(200, 100, 100) : GetColor(150, 150, 150);
		DrawBox(exitButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				exitButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				exitColor, true);
		DrawBox(exitButtonX - buttonWidth / 2, buttonY - buttonHeight / 2,
				exitButtonX + buttonWidth / 2, buttonY + buttonHeight / 2,
				GetColor(0, 0, 0), false);
		
		const char* exitText = "終了";
		int textWidth = GetDrawStringWidthToHandle(exitText, static_cast<int>(strlen(exitText)), itemFontHandle_);
		DrawStringToHandle(exitButtonX - textWidth / 2, buttonY - 16, exitText, GetColor(255, 255, 255), itemFontHandle_);
	}
}

void OptionScene::Release()
{
	// 設定を保存
	SaveOptionValues();

	// フォントハンドルを削除
	DeleteFontToHandle(categoryFontHandle_);
	DeleteFontToHandle(itemFontHandle_);
}

void OptionScene::LoadOptionValues()
{
	auto& appIns = Application::GetInstance();
	auto inputIns = InputManager::GetInstance();
	auto audioIns = AudioManager::GetInstance();

	// コメントアウトの分は未実装
	//currentAudioDeviceNum_ = audioIns->GetCurrentDeviceId();
	mVolume_ = audioIns->GetMasterVolume();
	bVolume_ = audioIns->GetBgmVolume();
	sVolume_ = audioIns->GetSeVolume();
	isMute_ = audioIns->IsMute();
	brightness_ = appIns.GetBrightness();
	//enableShader_ = appIns.IsShaderEnabled();
	
	// FPSリミット: 実際のFPS値をドロップダウンのインデックスに変換
	int actualFPS = appIns.GetFPSLimit();
	if (actualFPS <= 30) fpsLimit_ = 0;
	else if (actualFPS <= 60) fpsLimit_ = 1;
	else if (actualFPS <= 120) fpsLimit_ = 2;
	else fpsLimit_ = 3; // 無制限
	
	//isInvertXAxis_ = appIns.IsInvertXAxis();
	//isInvertYAxis_ = appIns.IsInvertYAxis();
	//holdThreshold_ = inputIns->GetHoldThreshold();
	isAcceptKeyboardInput_ = inputIns->IsEnableKeyAndMouse();
	mouseSensitivity_ = static_cast<float>(inputIns->GetMouseSensitivity());
	wheelSensitivity_ = inputIns->GetMouseWheelSensitivity();
	//leftStickSensitivity_ = inputIns->GetLeftStickSensitivity();
	//leftStickDeadZone_ = inputIns->GetLeftStickDeadZone();
	//rightStickSensitivity_ = inputIns->GetRightStickSensitivity();
	//rightStickDeadZone_ = inputIns->GetRightStickDeadZone();
	enableVibration_ = inputIns->IsVibrationEnabled();
	//vibrationStrength_ = inputIns->GetVibrationStrength();
	//currentColorAccessibilityNum_ = inputIns->GetColorAccessibilityNum();

	Application::ShowInfos& showInfos = appIns.GetShowInfos();
	isShowFPS_ = showInfos.fps;
	isShowMemoryUsage_ = showInfos.memoryUsage;
	isShowBatteryStatus_ = showInfos.batteryStatus;
	isShowPlayerPosition_ = showInfos.playerPosition;
	isShowCollider_ = showInfos.collider;
	isShowXYZAxis_ = showInfos.xyzAxis;
}

void OptionScene::SaveOptionValues()
{
	auto& appIns = Application::GetInstance();
	auto inputIns = InputManager::GetInstance();
	auto audioIns = AudioManager::GetInstance();

	// 設定を各マネージャーに反映
	// コメントアウトの分は未実装
	//audioIns->SetCurrentDeviceId(currentAudioDeviceNum_);
	audioIns->SetMasterVolume(mVolume_);
	audioIns->SetBgmVolume(bVolume_);
	audioIns->SetSeVolume(sVolume_);
	audioIns->SetMute(isMute_);
	appIns.SetBrightness(brightness_);
	//appIns.SetShaderEnabled(enableShader);
	
	// FPSリミット: ドロップダウンのインデックスを実際のFPS値に変換
	int actualFPS = 60; // デフォルト値
	switch (fpsLimit_)
	{
	case 0: actualFPS = 30; break;
	case 1: actualFPS = 60; break;
	case 2: actualFPS = 120; break;
	case 3: actualFPS = 0; break; // 無制限
	default: actualFPS = 60; break;
	}
	appIns.SetFPSLimit(actualFPS);
	
	//appIns.SetInvertXAxis(isInvertXAxis_);
	//appIns.SetInvertYAxis(isInvertYAxis_);
	//inputIns->SetHoldThreshold(holdThreshold_);
	inputIns->SetKeyAndMouseEnabled(isAcceptKeyboardInput_);
	inputIns->SetMouseSensitivity(mouseSensitivity_);
	inputIns->SetMouseWheelSensitivity(wheelSensitivity_);
	//inputIns->SetLeftStickSensitivity(leftStickSensitivity_);
	//inputIns->SetLeftStickDeadZone(leftStickDeadZone_);
	//inputIns->SetRightStickSensitivity(rightStickSensitivity_);
	//inputIns->SetRightStickDeadZone(rightStickDeadZone_);
	inputIns->SetVibrationEnabled(enableVibration_);
	//inputIns->SetVibrationStrength(vibrationStrength_);
	//inputIns->SetColorAccessibilityNum(currentColorAccessibilityNum_);

	Application::ShowInfos& showInfos = appIns.GetShowInfos();
	showInfos.fps = isShowFPS_;
	showInfos.memoryUsage = isShowMemoryUsage_;
	showInfos.batteryStatus = isShowBatteryStatus_;
	showInfos.playerPosition = isShowPlayerPosition_;
	showInfos.collider = isShowCollider_;
	showInfos.xyzAxis = isShowXYZAxis_;

	// TODO: 設定ファイルへの保存処理を実装
	// 例: SaveToFile("Config/Options.ini");
}

	
	
//void OptionScene::ApplyOptionValues()
//{
//	// 設定を即座に反映する場合に使用
//	SaveOptionValues();
//}

void OptionScene::CalculateItemDisplayInfo()
{
	int categoryTextHeight = 0;
	GetDrawFormatStringSizeToHandle(nullptr, &categoryTextHeight, nullptr, categoryFontHandle_, "%S", "A");
	int itemTextHeight = 0;
	GetDrawFormatStringSizeToHandle(nullptr, &itemTextHeight, nullptr, itemFontHandle_, "%S", "A");
	const int hitboxMargin = 5;
	const int categorySpacing = categoryTextHeight;
	const int itemSpacing = itemTextHeight;
	const int leftAreaCenterX = Application::SCREEN_SIZE_X / 4;

	int currentY = scrollOffset_;
	int actualItemIndex = 0;

	for (int i = 0; i < ALL_ITEM_NUM_MAX; i++)
	{
		auto& info = itemDisplayInfos_[i];

		info.isCategory = (ITEM_TYPES[i] == CATEGORY_HEADER);
		info.textHeight = info.isCategory ? categoryTextHeight : itemTextHeight;
		info.fontHandle = info.isCategory ? categoryFontHandle_ : itemFontHandle_;

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
	case IDX_BATTERY_STATUS:			return &isShowBatteryStatus_;
	case IDX_PLAYER_POSITION:			return &isShowPlayerPosition_;
	case IDX_COLLIDER:					return &isShowCollider_;
	case IDX_XYZ_AXIS:					return &isShowXYZAxis_;
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
	static std::vector<std::string> dummyOptions = {"選択肢1", "選択肢2", "選択肢3"};
	
	switch (itemIndex)
	{
	case IDX_OUTPUT_DEVICE:
		{
			static std::vector<std::string> devices = {"PC", "Bluetooth", "-"};
			return devices;
		}
	case IDX_FPS_LIMIT:
		{
			static std::vector<std::string> fpsOptions = {"30", "60", "120", "無制限"};
			return fpsOptions;
		}
	case IDX_COLOR_ACCESSIBILITY:
		{
			static std::vector<std::string> colorOptions = {"一般色覚(C型)", "1型2色覚(PA型)", "1型3色覚(P型)", "2型2色覚(DA型)", "2型3色覚(D型)", "3型2色覚(TA型)", "3型3色覚(T型)", "1色覚(A型)"};
			return colorOptions;
		}
	default:
		OutputDebugString("GetDropdownOptions: 未定義の項目インデックスが指定されました\n");
		return dummyOptions;
	}
}