#pragma once
#include <string>
#include "SceneBase.h"

class OptionScene : public SceneBase
{
public:

	// コンストラクタ
	OptionScene();

	// デストラクタ
	~OptionScene() override;

	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;

private:
	// 項目の表示情報を格納する構造体
	struct ItemDisplayInfo
	{
		int y;				// Y座標
		int textX;			// テキストのX座標
		int textWidth;		// テキストの幅
		int textHeight;		// テキストの高さ
		int fontHandle;		// フォントハンドル
		int left;			// ヒットボックス左
		int right;			// ヒットボックス右
		int top;			// ヒットボックス上
		int bottom;			// ヒットボックス下
		bool isCategory;	// カテゴリヘッダーか
		int itemIndex;		// 項目のインデックス（カテゴリを除いた実際の設定項目の番号）
	};

	// 項目インデックスから設定値へのマッピング
	enum ACTUAL_ITEM_INDEX
	{
		// 音響
		IDX_OUTPUT_DEVICE = 0,
		IDX_MASTER_VOLUME,
		IDX_BGM_VOLUME,
		IDX_SE_VOLUME,
		IDX_ENABLE_MUTE,
		
		// 画面
		IDX_BRIGHTNESS,
		IDX_ENABLE_SHADER,
		IDX_FPS_LIMIT,
		
		// 汎用操作
		IDX_INVERT_X_AXIS,
		IDX_INVERT_Y_AXIS,
		IDX_HOLD_THRESHOLD,
		IDX_ACCEPT_KEYBOARD_INPUT,
		
		// マウス
		IDX_MOUSE_SENSITIVITY,
		IDX_WHEEL_SENSITIVITY,
		
		// パッド
		IDX_LEFT_STICK_SENSITIVITY,
		IDX_LEFT_STICK_DEAD_ZONE,
		IDX_RIGHT_STICK_SENSITIVITY,
		IDX_RIGHT_STICK_DEAD_ZONE,
		IDX_ENABLE_VIBRATION,
		IDX_VIBRATION_STRENGTH,
		
		// その他
		IDX_COLOR_ACCESSIBILITY,
		
		// デバッグ
		IDX_FPS,
		IDX_MEMORY_USAGE,
		IDX_COLLIDER,
		IDX_XYZ_AXIS,
		IDX_PLAYER_POSITION,
		IDX_BATTERY_STATUS,
		
		IDX_MAX
	};

	// 項目のクリック処理（個別）
	void HandleItemClick(int itemIndex);
	
	// 特定の項目の値を取得
	void* GetItemValuePtr(int itemIndex);
	
	// 特定の項目の値を設定
	void SetItemValue(int itemIndex, void* value);

	// 値の読み込み
	void LoadOptionValues();

	// 値の保存
	void SaveOptionValues();

	// 値の適用
	//void ApplyOptionValues();

	// 項目の表示情報を初期化
	void CalculateItemDisplayInfo();

	// 設定項目
	enum OPTION_ITEM
	{
		VOLUME,				// 音響
		DISPLAY,			// 画面
		GENERAL_CONTROL,	// 汎用操作
		MOUSE,				// マウス
		PAD,				// パッド
		OTHER,				// その他
		DEBUG,				// デバッグ
		APPLY,				// 適用
		EXIT,				// 終了
		OPTION_MAX
	};

	// 音響関連
	enum VOLUME_ITEM
	{
		OUTPUT_DEVICE,	// 出力デバイス
		MASTER_VOLUME,	// 主音量
		BGM_VOLUME,		// BGM音量
		SE_VOLUME,		// 効果音音量
		ENABLE_MUTE,	// ミュートの有無
		VOLUME_MAX
	};
	
	// AudioManager
	int currentAudioDeviceNum_ = 0;		// 出力デバイスの番号
	float mVolume_ = 0.0f;				// 主音量
	float bVolume_ = 0.0f;				// BGM音量
	float sVolume_ = 0.0f;				// 効果音音量
	bool isMute_ = false;				// ミュートの有無

	// 画面関連
	enum DISPLAY_ITEM
	{
		BRIGHTNESS,		// 明るさ
		ENABLE_SHADER,	// シェーダーの有無
		FPS_LIMIT,		// FPS制限
		DISPLAY_MAX
	};

	// Application
	float brightness_ = 0.0f;	// 明るさ
	bool enableShader_ = false;	// シェーダーの有無
	int fpsLimit_ = 0;			// FPS制限

	// 汎用操作関連
	enum GENERAL_CONTROL_ITEM
	{
		INVERT_X_AXIS,			// X軸反転
		INVERT_Y_AXIS,			// Y軸反転
		HOLD_THRESHOLD,			// 長押し判定まで閾値（フレーム数）
		ACCEPT_KEYBOARD_INPUT,	// パッド接続時のキー入力の受付
		GEN_CTRL_MAX
	};

	// Application
	bool isInvertXAxis_ = false;			// X軸反転
	bool isInvertYAxis_ = false;			// Y軸反転
	// InputManager
	int holdThreshold_ = 0;					// 長押し判定まで閾値（フレーム数）
	bool isAcceptKeyboardInput_ = false;	// パッド接続時のキー入力の受付

	// マウス操作関連
	enum MOUSE_ITEM
	{
		MOUSE_SENSITIVITY,	// マウス感度	
		WHEEL_SENSITIVITY,	// ホイール回転感度
		MOUSE_MAX
	};

	// InputManager
	int mouseSensitivity_ = 0;	// マウス感度
	float wheelSensitivity_ = 0.0f;	// ホイール回転感度

	// パッド操作関連
	enum PAD_ITEM
	{
		LEFT_STICK_SENSITIVITY,		// 左スティック感度
		LEFT_STICK_DEAD_ZONE,		// 左スティックのデッドゾーン
		RIGHT_STICK_SENSITIVITY,	// 右スティック感度
		RIGHT_STICK_DEAD_ZONE,		// 右スティックのデッドゾーン
		ENABLE_VIBRATION,			// 振動の有無
		VIBRATION_STRENGTH,			// 振動の強さ
		PAD_MAX
	};

	// InputManager
	float leftStickSensitivity_ = 0.0f;		// 左スティック感度
	float leftStickDeadZone_ = 0.0f;		// 左スティックのデッドゾーン
	float rightStickSensitivity_ = 0.0f;	// 右スティック感度
	float rightStickDeadZone_ = 0.0f;		// 右スティックのデッドゾーン
	bool enableVibration_ = false;			// 振動の有無
	float vibrationStrength_ = 0.0f;		// 振動の強さ

	// その他
	enum OTHER_ITEM
	{
		COLOR_ACCESSIBILITY,	// 色覚
		OTHER_MAX
	};

	// Application
	int currentColorAccessibilityNum_ = 0;	// 色覚の番号

	// デバッグ関連
	enum DEBUG_ITEM
	{
		FPS,				// FPSの表示
		MEMORY_USAGE,		// メモリ使用量の表示
		COLLIDER,			// コライダー表示
		XYZ_AXIS,			// XYZ軸の表示
		PLAYER_POSITION,	// プレイヤー座標の表示
		BATTERY_STATUS,		// バッテリー残量の表示
		DEBUG_MAX
	};

	// Application
	bool isShowFPS_ = false;				// FPSの表示
	bool isShowMemoryUsage_ = false;		// メモリ使用量の表示
	bool isShowCollider_ = false;			// コライダー表示
	bool isShowXYZAxis_ = false;			// XYZ軸の表示
	bool isShowPlayerPosition_ = false;		// プレイヤー座標の表示
	bool isShowBatteryStatus_ = false;		// バッテリー残量の表示

	// カテゴリヘッダー数（APPLY, EXITを除く）
	static constexpr int CATEGORY_COUNT = 7;
	
	// 全項目数（カテゴリヘッダー + 各項目）
	static constexpr int ALL_ITEM_NUM_MAX = CATEGORY_COUNT + VOLUME_MAX + DISPLAY_MAX + GEN_CTRL_MAX + MOUSE_MAX + PAD_MAX + OTHER_MAX + DEBUG_MAX;

	const std::string ITEM_NAMES[ALL_ITEM_NUM_MAX] =
	{
		// 音響カテゴリ
		"【音響】",
		"音声出力デバイス",
		"主音量",
		"BGM音量",
		"効果音音量",
		"ミュート",
		
		// 画面カテゴリ
		"【画面】",
		"明るさ",
		"シェーダーの有無",
		"FPS制限",
		
		// 汎用操作カテゴリ
		"【汎用操作】",
		"X軸の反転",
		"Y軸の反転",
		"長押し判定まで閾値",
		"パッド接続時のキー入力の受付",
		
		// マウスカテゴリ
		"【マウス】",
		"マウスの感度",
		"ホイールの回転感度",
		
		// パッドカテゴリ
		"【パッド】",
		"左スティックの感度",
		"左スティックのデッドゾーン",
		"右スティックの感度",
		"右スティックのデッドゾーン",
		"振動の有無",
		"振動の強さ",
		
		// その他カテゴリ
		"【その他】",
		"色覚",
		
		// デバッグカテゴリ
		"【デバッグ】",
		"FPS",
		"メモリ使用量",
		"コライダー",
		"XYZ軸",
		"プレイヤー座標",
		"バッテリー残量"
	};

	const std::string OPTION_ITEM_NAME[OPTION_MAX] =
	{
		"音響",
		"画面",
		"汎用操作",
		"マウス",
		"パッド",
		"その他",
		"デバッグ",
		"適用",
		"終了"
	};

	enum OPTION_TYPE
	{
		CATEGORY_HEADER,	// カテゴリヘッダー（クリック不可）
		CHECKBOX,
		DROPDOWN,
		SLIDER
	};

	static constexpr OPTION_TYPE ITEM_TYPES[ALL_ITEM_NUM_MAX] =
	{
		// 音響カテゴリ
		CATEGORY_HEADER,	// 【音響】
		DROPDOWN,			// 音声出力デバイス
		SLIDER,				// 主音量
		SLIDER,				// BGM音量
		SLIDER,				// 効果音音量
		CHECKBOX,			// ミュート
		
		// 画面カテゴリ
		CATEGORY_HEADER,	// 【画面】
		SLIDER,				// 明るさ
		CHECKBOX,			// シェーダーの有無
		DROPDOWN,			// FPS制限
		
		// 汎用操作カテゴリ
		CATEGORY_HEADER,	// 【汎用操作】
		CHECKBOX,			// X軸の反転
		CHECKBOX,			// Y軸の反転
		SLIDER,				// 長押し判定まで閾値
		CHECKBOX,			// パッド接続時のキー入力の受付
		
		// マウスカテゴリ
		CATEGORY_HEADER,	// 【マウス】
		SLIDER,				// マウスの感度
		SLIDER,				// ホイールの回転感度
		
		// パッドカテゴリ
		CATEGORY_HEADER,	// 【パッド】
		SLIDER,				// 左スティックの感度
		SLIDER,				// 左スティックのデッドゾーン
		SLIDER,				// 右スティックの感度
		SLIDER,				// 右スティックのデッドゾーン
		CHECKBOX,			// 振動の有無
		SLIDER,				// 振動の強さ
		
		// その他カテゴリ
		CATEGORY_HEADER,	// 【その他】
		DROPDOWN,			// 色覚
		
		// デバッグカテゴリ
		CATEGORY_HEADER,	// 【デバッグ】
		CHECKBOX,			// FPS
		CHECKBOX,			// メモリ使用量
		CHECKBOX,			// コライダー
		CHECKBOX,			// XYZ軸
		CHECKBOX,			// プレイヤー座標
		CHECKBOX			// バッテリー残量
	};

	int categoryFontHandle_ = 0;	// メインフォントハンドル
	int itemFontHandle_ = 0;		// サブフォントハンドル
	int scrollOffset_ = 32;		// スクロールオフセット
	int lastItemNum_ = 0;		// 最後にホバーした項目番号
	int currentItemNum_ = -1;	// 現在ホバーしている項目番号
	int backgroundHandle_ = 0;	// 背景画像ハンドル

	// 項目の表示情報キャッシュ
	ItemDisplayInfo itemDisplayInfos_[ALL_ITEM_NUM_MAX];

	// チェックボックスの状態
	struct CheckboxState
	{
		bool isHovered = false;	// ホバー中か
	};

	// スライダーの状態
	struct SliderState
	{
		bool isDragging = false;	// ドラッグ中か
		int dragItemIndex = -1;		// ドラッグ中の項目インデックス
	};

	// ドロップダウンの状態
	struct DropdownState
	{
		bool isOpen = false;		// 開いているか
		int openItemIndex = -1;		// 開いている項目インデックス
		int hoveredOption = -1;		// ホバー中の選択肢
	};

	// チェックボックスの更新処理
	void UpdateCheckbox(int itemIndex, int x, int y, int cursorX, int cursorY);

	// スライダーの更新処理
	void UpdateSlider(int itemIndex, int x, int y, int cursorX, int cursorY);

	// ドロップダウンの更新処理
	void UpdateDropdown(int itemIndex, int x, int y, int cursorX, int cursorY);

	// チェックボックスの描画
	void DrawCheckbox(int itemIndex, int x, int y);

	// スライダーの描画
	void DrawSlider(int itemIndex, int x, int y);

	// ドロップダウンの描画
	void DrawDropdown(int itemIndex, int x, int y);

	// 項目の値を取得（float型）
	float GetItemValueFloat(int itemIndex);

	// 項目の値を設定（float型）
	void SetItemValueFloat(int itemIndex, float value);

	// 項目の値を取得（int型）
	int GetItemValueInt(int itemIndex);

	// 項目の値を設定（int型）
	void SetItemValueInt(int itemIndex, int value);

	// 項目の値を取得（bool型）
	bool GetItemValueBool(int itemIndex);

	// 項目の値を設定（bool型）
	void SetItemValueBool(int itemIndex, bool value);

	// ドロップダウンの選択肢を取得
	const std::vector<std::string>& GetDropdownOptions(int itemIndex);

	CheckboxState checkboxState_;	// チェックボックスの状態
	SliderState sliderState_;		// スライダーの状態
	DropdownState dropdownState_;	// ドロップダウンの状態

	// UI画像ハンドル
	int sliderFrameHandle_ = 0;		// スライダーフレーム画像
	int sliderKnobHandle_ = 0;		// スライダーつまみ画像
	int checkboxOnHandle_ = 0;		// チェックボックスON画像
	int checkboxOffHandle_ = 0;		// チェックボックスOFF画像
	int buttonApplyHandle_ = 0;		// 適用ボタン画像
	int buttonExitHandle_ = 0;		// 終了ボタン画像

	// 適用・終了ボタンの更新処理
	void UpdateButtons(int cursorX, int cursorY);

	// 適用・終了ボタンの描画
	void DrawButtons();

	// ボタンの状態
	struct ButtonState
	{
		bool isApplyHovered = false;	// 適用ボタンがホバーされているか
		bool isExitHovered = false;		// 終了ボタンがホバーされているか
	};

	ButtonState buttonState_;	// ボタンの状態

	// スライダー設定
	struct SliderConfig
	{
		enum class ValueType
		{
			FLOAT,
			INT
		};
		
		ValueType type;
		float minValue;
		float maxValue;
		float step; // 刻み幅(オプション)
	};

	// スライダー設定を取得
	SliderConfig GetSliderConfig(int itemIndex) const;
};