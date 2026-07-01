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
	float mouseSensitivity_ = 0.0f;	// マウス感度
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

	static constexpr int ALL_ITEM_NUM_MAX = VOLUME_MAX + DISPLAY_MAX + GEN_CTRL_MAX + MOUSE_MAX + PAD_MAX + OTHER_MAX + DEBUG_MAX;
	int currentItemNum_ = 0;

	const std::string ALL_ITEM_NAME[ALL_ITEM_NUM_MAX] =
	{
		"音声出力デバイス",
		"主音量",
		"BGM音量",
		"効果音音量",
		"ミュート",
		"明るさ",
		"シェーダーの有無",
		"FPS制限",
		"X軸の反転",
		"Y軸の反転",
		"長押し判定まで閾値",
		"パッド接続時のキー入力の受付",
		"マウスの感度",
		"ホイールの回転感度",
		"左スティックの感度",
		"左スティックのデッドゾーン",
		"右スティックの感度",
		"右スティックのデッドゾーン",
		"振動の有無",
		"振動の強さ",
		"色覚",
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
		"終了"
	};

	int fontHandle_ = 0;	// フォントハンドル
};