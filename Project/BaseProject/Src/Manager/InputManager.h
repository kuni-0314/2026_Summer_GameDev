#pragma once
#include <initializer_list>
#include <DxLib.h>
#include <map>

// Addのやり方も悪くないけど面倒なので除外

#define ASO_CTRL_DOWN PAD_INPUT_DOWN
#define ASO_CTRL_LEFT PAD_INPUT_LEFT
#define	ASO_CTRL_RIGHT PAD_INPUT_RIGHT
#define ASO_CTRL_UP PAD_INPUT_UP
#define ASO_CTRL_A PAD_INPUT_A
#define ASO_CTRL_B PAD_INPUT_B
#define ASO_CTRL_X PAD_INPUT_C
#define ASO_CTRL_Y PAD_INPUT_X
#define ASO_CTRL_LB PAD_INPUT_Y
#define ASO_CTRL_RB PAD_INPUT_Z
#define ASO_CTRL_BACK PAD_INPUT_L
#define ASO_CTRL_START PAD_INPUT_R
#define ASO_CTRL_LSTICK PAD_INPUT_START
#define ASO_CTRL_RSTICK PAD_INPUT_M

// インプットマネージャー
class InputManager
{
public:
	// パッドボタン
	// トリガーは特殊なので除外
	enum class PadInput
	{
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		LStick,
		LS = 6,
		RStick,
		RS = 7,
		LB,
		RB,
		A = 12,
		B,
		X,
		Y,
		MAX,
	};

	static void CreateInstance();			// インスタンス生成
	static InputManager* GetInstance();		// インスタンス取得
	static void DeleteInstance();			// インスタンス削除

	bool Init();	// 初期化（初回のみ実行）
	void Update();	// 更新
	bool Release();	// 解放

	void Clear();			// 判定を行うキーをクリア
	bool IsNew(int key);		// キーが押されているか
	bool IsNew(std::initializer_list<int> keys, bool isAnd = false);	// キーが押されているか
	bool IsTrgDown(int key);	// キーが今押されたか
	bool IsTrgUp(int key);		// キーは離されたか
	bool IsHold(int key, int holdTime);	// キーが指定時間以上押されているか
	int GetLastHoldTime(int key);	// キーが最後に離されるまで押されていた時間を取得

	void ClearMouse();				// 判定を行うマウスボタンをクリア
	bool IsMouseNew(int button);		// マウスボタンが押されているか
	bool IsMouseTrgDown(int button);	// マウスボタンが今押されたか
	bool IsMouseTrgUp(int button);		// マウスボタンは離されたか
	bool IsMouseHold(int button, int holdTime);	// マウスボタンが指定時間以上押されているか
	int GetMouseWheel();			// マウスホイールの回転量を取得
	void GetMousePos(int& x, int& y);	// マウスの座標を取得
	bool IsMouseMoved();				// マウスが動いたか
	int GetMouseLastHoldTime(int button);	// マウスボタンが最後に離されるまで押されていた時間を取得

	bool IsGamepadNew(int button, int gamepadIndex);		// ゲームパッドボタンが押されているか
	bool IsGamepadTrgDown(int button, int gamepadIndex);	// ゲームパッドボタンが今押されたか
	bool IsGamepadTrgUp(int button, int gamepadIndex);		// ゲームパッドボタンは離されたか
	bool IsGamepadNew(PadInput button, int gamepadIndex);		// ゲームパッドボタンが押されているか
	bool IsGamepadTrgDown(PadInput button, int gamepadIndex);	// ゲームパッドボタンが今押されたか
	bool IsGamepadTrgUp(PadInput button, int gamepadIndex);		// ゲームパッドボタンは離されたか
	bool IsGamepadTriggerNew(bool isLeft, int gamepadIndex);	// トリガーが押されているか
	bool IsGamepadTriggerTrgDown(bool isLeft, int gamepadIndex);	// トリガーが今押されたか
	bool IsGamepadTriggerTrgUp(bool isLeft, int gamepadIndex);	// トリガーは離されたか
	int GetGamepadTriggerValue(bool isLeft, int gamepadIndex);	// トリガーの値を取得
	bool IsGamepadHold(int button, int gamepadIndex, int holdTime);	// ゲームパッドボタンが指定時間以上押されているか
	bool IsGamepadHold(PadInput button, int gamepadIndex, int holdTime);	// ゲームパッドボタンが指定時間以上押されているか
	int GetGamepadLastHoldTime(int button, int gamepadIndex);	// ゲームパッドボタンが最後に離されるまで押されていた時間を取得
	int GetGamepadLastHoldTime(PadInput button, int gamepadIndex);	// ゲームパッドボタンが最後に離されるまで押されていた時間を取得

	void GetStick(int gamepadIndex, short& leftX, short& leftY, short& rightX, short& rightY);	// スティックの値を取得
	void GetLeftStick(int gamepadIndex, short& x, short& y, float deadzone = DEFAULT_STICK_DEADZONE);	// 左スティックの値を取得
	void GetRightStick(int gamepadIndex, short& x, short& y, float deadzone = DEFAULT_STICK_DEADZONE);	// 右スティックの値を取得
	short GetLeftStickX(int gamepadIndex);		// 左スティックのX軸を取得
	short GetLeftStickY(int gamepadIndex);		// 左スティックのY軸を取得
	short GetRightStickX(int gamepadIndex);		// 右スティックのX軸を取得
	short GetRightStickY(int gamepadIndex);		// 右スティックのY軸を取得

	// キー入力からXZ平面上の方向ベクトルを取得
	void GetInputDirXZ(VECTOR& vec, int keyUp, int keyDown, int keyLeft, int keyRight);

	// スティック入力からXZ平面上の方向ベクトルを取得
	static constexpr float DEFAULT_STICK_DEADZONE = 0.2f;	// デッドゾーンのデフォルト値
	void GetStickDirXZ(VECTOR& vec, int gamepadIndex, bool isLeftStick, float deadzone = DEFAULT_STICK_DEADZONE);

	// トリガー入力
	static constexpr float TRIGGER_THRESHOLD = 30.0f;	// トリガーが押されたとみなす値の閾値

	// 対象のボタンのうち指定したボタンだけが押された瞬間を判定
	// MainButtonとOtherButtonsの型にはIntまたはPadInputを指定すること
	template<typename MainButton, typename ...OtherButtons>
	bool IsOnlyGamepadButtonRemain(MainButton mainButton, int gamepadIndex, OtherButtons ...otherButtons)
	{
		bool pattern1 = false, pattern2 = false;

		bool anyTrgUp = false;
		for (auto button : { otherButtons... })
		{
			// otherButtonsのいずれかがTrgUpで、それ以外は押されていないか
			if (IsGamepadTrgUp(button, gamepadIndex)) anyTrgUp = true;
			if (IsGamepadNew(button, gamepadIndex)) return false;	// otherButtonsのいずれかが押されている場合はfalseを返す
		}

		// パターン1: mainButtonのみが押された瞬間
		if (IsGamepadTrgDown(mainButton, gamepadIndex)) pattern1 = true;

		// パターン2: otherButtonsのいずれかが離され、mainButtonが押されている
		if (anyTrgUp && IsGamepadNew(mainButton, gamepadIndex)) pattern2 = true;

		return (pattern1 || pattern2);
	}

	void SetKeyAndMouseEnabled(bool enabled) { isKeyAndMouseEnabled_ = enabled; }	// キーとマウスの両方を有効/無効にする
	bool IsEnableKeyAndMouse() const { return isKeyAndMouseEnabled_; }	// キーとマウスの両方が有効か

	int GetMouseSensitivity() const;	// マウスの感度の取得
	void SetMouseSensitivity(int value);	// マウスの感度の設定
	float GetMouseWheelSensitivity();	// マウスホイールの回転感度の取得
	void SetMouseWheelSensitivity(float value);	// マウスホイールの回転感度の設定

	void VibrateGamepad(int gamepadIndex, int power, int time);	// ゲームパッドを振動させる
	void SetVibrationEnabled(bool enabled);	// 振動の有効/無効を設定
	bool IsVibrationEnabled() const;	// 振動が有効かどうかを取得

private:
	InputManager();		// コンストラクタ
	~InputManager();	// デストラクタ

	void UpdateGamePad();	// ゲームパッドの更新

	// キーの入力状態の情報
	struct Info
	{
		int key;
		bool keyOld;
		bool keyNew;
		bool keyTrgDown;
		bool keyTrgUp;
		int holdTime;
		int lastHoldTime;
	};

	// マウスボタンの入力状態の情報
	struct MouseInfo
	{
		int button;
		bool mouseOld;
		bool mouseNew;
		bool mouseTrgDown;
		bool mouseTrgUp;
		int holdTime;
		int lastHoldTime;
	};

	// パッドの状態
	struct PadInfo
	{
		// ボタンの入力状態の情報
		struct ButtonInfo
		{
			int button;
			bool buttonOld;
			bool buttonNew;
			bool buttonTrgDown;
			bool buttonTrgUp;
			int holdTime;
			int lastHoldTime;
		};
		struct TriggerInfo
		{
			unsigned char value;
			bool buttonOld;
			bool buttonNew;
			bool buttonTrgDown;
			bool buttonTrgUp;
			int holdTime;
			int lastHoldTime;
		};
		ButtonInfo inputs[static_cast<int>(PadInput::MAX)];
		short AKeyLX;
		short AKeyLY;
		short AKeyRX;
		short AKeyRY;
		TriggerInfo LTrigger;
		TriggerInfo RTrigger;
		bool isStickAsDpad;
	};

	static constexpr int GAMEPAD_NUM_MAX = 4;	// サポートするゲームパッドの最大数

	void Add(int key);				// 判定を行うキーを追加
	void AddMouse(int button);		// 判定を行うマウスボタンを追加

	//float mouseSensitivity_ = 1.0f;	// マウスの感度
	int mouseWheel_;					// マウスホイールの回転量
	float wheelSensitivity_ = 10.0f;	// マウスホイールの回転感度
	int prevMouseX_;					// 前フレームのマウスX座標
	int prevMouseY_;					// 前フレームのマウスY座標
	bool isMouseMoved_;					// マウスが動いたか

	std::map<int, Info> keyInfos_;			// キーと情報紐付け
	std::map<int, MouseInfo> mouseInfos_;	// マウスボタンと情報の紐付け
	PadInfo gamepadInfos_[GAMEPAD_NUM_MAX];	// ゲームパッドボタンと情報の紐付け

	bool isKeyAndMouseEnabled_;	// キーとマウスの両方が有効か

	// 振動が有効かどうか
#if _DEBUG
	bool isVibrationEnabled_ = false;
#else
	bool isVibrationEnabled_ = true;
#endif


	// インスタンス
	static InputManager* instance_;	// 自己
};