#pragma once
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

	static void CreateInstance(void);			// インスタンス生成
	static InputManager* GetInstance(void);		// インスタンス取得
	static void DeleteInstance(void);			// インスタンス削除

	bool Init(void);	// 初期化（初回のみ実行）
	void Update(void);	// 更新
	bool Release(void);	// 解放

	void Clear(void);			// 判定を行うキーをクリア
	bool IsNew(int key);		// キーが押されているか
	bool IsTrgDown(int key);	// キーが今押されたか
	bool IsTrgUp(int key);		// キーは離されたか

	void ClearMouse(void);				// 判定を行うマウスボタンをクリア
	bool IsMouseNew(int button);		// マウスボタンが押されているか
	bool IsMouseTrgDown(int button);	// マウスボタンが今押されたか
	bool IsMouseTrgUp(int button);		// マウスボタンは離されたか
	int GetMouseWheel(void);			// マウスホイールの回転量を取得
	void GetMousePos(int& x, int& y);	// マウスの座標を取得

	bool IsGamepadNew(int button, int gamepadIndex);		// ゲームパッドボタンが押されているか
	bool IsGamepadTrgDown(int button, int gamepadIndex);	// ゲームパッドボタンが今押されたか
	bool IsGamepadTrgUp(int button, int gamepadIndex);		// ゲームパッドボタンは離されたか
	bool IsGamepadNew(PadInput button, int gamepadIndex);		// ゲームパッドボタンが押されているか
	bool IsGamepadTrgDown(PadInput button, int gamepadIndex);	// ゲームパッドボタンが今押されたか
	bool IsGamepadTrgUp(PadInput button, int gamepadIndex);		// ゲームパッドボタンは離されたか

	void GetStick(int gamepadIndex, short& leftX, short& leftY, short& rightX, short& rightY);	// スティックの値を取得
	void GetLeftStick(int gamepadIndex, short& x, short& y);	// 左スティックの値を取得
	void GetRightStick(int gamepadIndex, short& x, short& y);	// 右スティックの値を取得
	short GetLeftStickX(int gamepadIndex);		// 左スティックのX軸を取得
	short GetLeftStickY(int gamepadIndex);		// 左スティックのY軸を取得
	short GetRightStickX(int gamepadIndex);		// 右スティックのX軸を取得
	short GetRightStickY(int gamepadIndex);		// 右スティックのY軸を取得

	// 入力キーからXZ平面上の方向ベクトルを取得
	void GetInputDirXZ(VECTOR& vec, int keyUp, int keyDown, int keyLeft, int keyRight);

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

private:
	InputManager(void);		// コンストラクタ
	~InputManager(void);	// デストラクタ

	void UpdateGamePad(void);	// ゲームパッドの更新

	// キーの入力状態の情報
	struct Info
	{
		int key;
		bool keyOld;
		bool keyNew;
		bool keyTrgDown;
		bool keyTrgUp;
	};

	// マウスボタンの入力状態の情報
	struct MouseInfo
	{
		int button;
		bool mouseOld;
		bool mouseNew;
		bool mouseTrgDown;
		bool mouseTrgUp;
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
		};
		ButtonInfo inputs[static_cast<int>(PadInput::MAX)];
		short AKeyLX;
		short AKeyLY;
		short AKeyRX;
		short AKeyRY;
		unsigned char LTrigger;
		unsigned char RTrigger;
		bool isStickAsDpad;
	};

	static constexpr int GAMEPAD_NUM_MAX = 4;	// サポートするゲームパッドの最大数

	void Add(int key);				// 判定を行うキーを追加
	void AddMouse(int button);		// 判定を行うマウスボタンを追加

	int mouseWheel_;	// マウスホイールの回転量

	std::map<int, Info> keyInfos_;			// キーと情報紐付け
	std::map<int, MouseInfo> mouseInfos_;	// マウスボタンと情報の紐付け
	PadInfo gamepadInfos_[GAMEPAD_NUM_MAX];	// ゲームパッドボタンと情報の紐付け

	// インスタンス
	static InputManager* instance_;	// 自己
};