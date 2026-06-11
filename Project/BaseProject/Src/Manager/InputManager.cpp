#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

// コンストラクタ
InputManager::InputManager()
{
}

// デストラクタ
InputManager::~InputManager()
{
}

// インスタンス生成
void InputManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	else
	{
		OutputDebugString("InputManagerのインスタンスは作成済みです。\n");
	}
}

// インスタンス取得
InputManager* InputManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		OutputDebugString("InputManagerのインスタンスが作成されていません。\n");
		return 	nullptr;
	}

	return instance_;
}

// インスタンス削除
void InputManager::DeleteInstance()
{
	if (instance_ == nullptr)
	{
		OutputDebugString("InputManagerのインスタンスは削除済みです。\n");
		return;
	}

	delete instance_;
}

// 初期化
bool InputManager::Init()
{
	Add(KEY_INPUT_A);
	Add(KEY_INPUT_B);
	Add(KEY_INPUT_C);
	Add(KEY_INPUT_D);
	Add(KEY_INPUT_E);
	Add(KEY_INPUT_F);
	Add(KEY_INPUT_G);
	Add(KEY_INPUT_H);
	Add(KEY_INPUT_I);
	Add(KEY_INPUT_J);
	Add(KEY_INPUT_K);
	Add(KEY_INPUT_L);
	Add(KEY_INPUT_M);
	Add(KEY_INPUT_N);
	Add(KEY_INPUT_O);
	Add(KEY_INPUT_P);
	Add(KEY_INPUT_Q);
	Add(KEY_INPUT_R);
	Add(KEY_INPUT_S);
	Add(KEY_INPUT_T);
	Add(KEY_INPUT_U);
	Add(KEY_INPUT_V);
	Add(KEY_INPUT_W);
	Add(KEY_INPUT_X);
	Add(KEY_INPUT_Y);
	Add(KEY_INPUT_Z);

	Add(KEY_INPUT_F9);

	Add(KEY_INPUT_LSHIFT);		// ダッシュ
	Add(KEY_INPUT_LCONTROL);	// 向きを変えずに移動


	Add(KEY_INPUT_SPACE);	// シーン遷移

	//Add(KEY_INPUT_RETURN);	// 攻撃

	Add(KEY_INPUT_RSHIFT);		// ダッシュ
	Add(KEY_INPUT_BACKSLASH);

	Add(KEY_INPUT_TAB);

	Add(KEY_INPUT_LEFT);	// 
	Add(KEY_INPUT_RIGHT);	// 
	Add(KEY_INPUT_UP);		// 
	Add(KEY_INPUT_DOWN);	// 


	//num
	Add(KEY_INPUT_NUMPAD1);
	Add(KEY_INPUT_NUMPAD2);
	Add(KEY_INPUT_NUMPAD3);
	Add(KEY_INPUT_NUMPAD4);
	Add(KEY_INPUT_NUMPAD5);
	Add(KEY_INPUT_NUMPAD6);
	Add(KEY_INPUT_NUMPAD7);
	Add(KEY_INPUT_NUMPAD8);
	Add(KEY_INPUT_NUMPAD9);
	Add(KEY_INPUT_NUMPAD0);

	Add(KEY_INPUT_1);	// デバッグ用
	Add(KEY_INPUT_2);	// デバッグ用
	Add(KEY_INPUT_3);	// デバッグ用
	Add(KEY_INPUT_4);	// デバッグ用
	Add(KEY_INPUT_5);	// デバッグ用
	Add(KEY_INPUT_6);	// デバッグ用
	Add(KEY_INPUT_7);	// デバッグ用
	Add(KEY_INPUT_8);	// デバッグ用
	Add(KEY_INPUT_9);
	Add(KEY_INPUT_0);


	AddMouse(MOUSE_INPUT_LEFT);
	AddMouse(MOUSE_INPUT_RIGHT);
	AddMouse(MOUSE_INPUT_MIDDLE);

	return true;	// 処理継続
}

// 更新
void InputManager::Update()
{
	// キーの更新
	for (auto& pair : keyInfos_)
	{
		InputManager::Info& info = pair.second;

		info.keyOld = info.keyNew;
		info.keyNew = CheckHitKey(info.key) ? true : false;
		info.keyTrgDown = (!info.keyOld && info.keyNew);
		info.keyTrgUp = (info.keyOld && !info.keyNew);
		
		// holdTimeの更新
		if (info.keyNew)
		{
			info.holdTime++;
		}
		else
		{
			if (info.keyTrgUp)
			{
				info.lastHoldTime = info.holdTime;
			}
			info.holdTime = 0;
		}
	}

	// マウスボタンの更新
	int mouseInput = GetMouseInput();
	for (auto& pair : mouseInfos_)
	{
		MouseInfo& info = pair.second;

		info.mouseOld = info.mouseNew;
		info.mouseNew = (mouseInput & info.button) != 0;
		info.mouseTrgDown = (!info.mouseOld && info.mouseNew);
		info.mouseTrgUp = (info.mouseOld && !info.mouseNew);
		
		// holdTimeの更新
		if (info.mouseNew)
		{
			info.holdTime++;
		}
		else
		{
			if (info.mouseTrgUp)
			{
				info.lastHoldTime = info.holdTime;
			}
			info.holdTime = 0;
		}
	}

	// マウスホイールの更新
	mouseWheel_ = GetMouseWheelRotVol();

	// ゲームパッドの更新
	UpdateGamePad();
}

// 解放
bool InputManager::Release()
{
	// キー情報のstd::mapを削除する
	keyInfos_.clear();

	// マウスボタン情報のstd::mapを削除する
	mouseInfos_.clear();

	return true;	// 処理継続
}

// 判定を行うキーをクリア
void InputManager::Clear()
{
	keyInfos_.clear();
}

// キーが押されているか
bool InputManager::IsNew(int key)
{
	return keyInfos_[key].keyNew;
}

// キーが今押されたか
bool InputManager::IsTrgDown(int key)
{
	return keyInfos_[key].keyTrgDown;
}

// キーが離されたか
bool InputManager::IsTrgUp(int key)
{
	return keyInfos_[key].keyTrgUp;
}

// キーが指定時間以上押されているか
bool InputManager::IsHold(int key, int holdTime)
{
	auto it = keyInfos_.find(key);
	if (it == keyInfos_.end()) return false;
	return it->second.keyNew && it->second.holdTime >= holdTime;
}

// キーが最後に離されるまで押されていた時間を取得
int InputManager::GetLastHoldTime(int key)
{
	return keyInfos_[key].holdTime;
}

// マウスボタンが指定時間以上押されているか
bool InputManager::IsMouseHold(int button, int holdTime)
{
	auto it = mouseInfos_.find(button);
	if (it == mouseInfos_.end()) return false;
	return it->second.mouseNew && it->second.holdTime >= holdTime;
}

// ゲームパッドボタンが指定時間以上押されているか（int版）
bool InputManager::IsGamepadHold(int button, int gamepadIndex, int holdTime)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return false;
	
	for (int i = 0; i < static_cast<int>(PadInput::MAX); i++)
	{
		if (gamepadInfos_[gamepadIndex].inputs[i].button == button)
		{
			return gamepadInfos_[gamepadIndex].inputs[i].buttonNew && 
			       gamepadInfos_[gamepadIndex].inputs[i].holdTime >= holdTime;
		}
	}
	return false;
}

// ゲームパッドボタンが指定時間以上押されているか（PadInput版）
bool InputManager::IsGamepadHold(PadInput button, int gamepadIndex, int holdTime)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return false;
	
	int index = static_cast<int>(button);
	if (index < 0 || index >= static_cast<int>(PadInput::MAX)) return false;
	
	return gamepadInfos_[gamepadIndex].inputs[index].buttonNew && 
	       gamepadInfos_[gamepadIndex].inputs[index].holdTime >= holdTime;
}

int InputManager::GetGamepadLastHoldTime(int button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[button].lastHoldTime;
}

int InputManager::GetGamepadLastHoldTime(PadInput button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[static_cast<int>(button)].lastHoldTime;
}

// 判定を行うマウスボタンをクリア
void InputManager::ClearMouse()
{
	mouseInfos_.clear();
}

// マウスが押されているか
bool InputManager::IsMouseNew(int button)
{
	return mouseInfos_[button].mouseNew;
}

// マウスが今押されたか
bool InputManager::IsMouseTrgDown(int button)
{
	return mouseInfos_[button].mouseTrgDown;
}

// マウスは離されたか
bool InputManager::IsMouseTrgUp(int button)
{
	return mouseInfos_[button].mouseTrgUp;
}

// マウスホイールの回転量を取得
int InputManager::GetMouseWheel()
{
	return mouseWheel_;
}

// マウスの座標を取得
void InputManager::GetMousePos(int& x, int& y)
{
	GetMousePoint(&x, &y);
}

int InputManager::GetMouseLastHoldTime(int button)
{
	return mouseInfos_[button].lastHoldTime;
}

// ゲームパッドボタンが押されているか
bool InputManager::IsGamepadNew(int button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[button].buttonNew;
}

// ゲームパッドボタンが今押されたか
bool InputManager::IsGamepadTrgDown(int button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[button].buttonTrgDown;
}

// ゲームパッドボタンは離されたか
bool InputManager::IsGamepadTrgUp(int button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[button].buttonTrgUp;
}

// ゲームパッドボタンが押されているか
bool InputManager::IsGamepadNew(PadInput button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[static_cast<int>(button)].buttonNew;
}

// ゲームパッドボタンが今押されたか
bool InputManager::IsGamepadTrgDown(PadInput button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[static_cast<int>(button)].buttonTrgDown;
}

// ゲームパッドボタンは離されたか
bool InputManager::IsGamepadTrgUp(PadInput button, int gamepadIndex)
{
	return gamepadInfos_[gamepadIndex].inputs[static_cast<int>(button)].buttonTrgUp;
}

// スティックの値を取得
void InputManager::GetStick(int gamepadIndex, short& leftX, short& leftY, short& rightX, short& rightY)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX)
	{
		leftX = 0;
		leftY = 0;
		rightX = 0;
		rightY = 0;
		return;
	}
	leftX = gamepadInfos_[gamepadIndex].AKeyLX;
	leftY = gamepadInfos_[gamepadIndex].AKeyLY;
	rightX = gamepadInfos_[gamepadIndex].AKeyRX;
	rightY = gamepadInfos_[gamepadIndex].AKeyRY;
}

// 左スティックの値を取得
void InputManager::GetLeftStick(int gamepadIndex, short& x, short& y)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX)
	{
		x = 0;
		y = 0;
		return;
	}
	x = gamepadInfos_[gamepadIndex].AKeyLX;
	y = gamepadInfos_[gamepadIndex].AKeyLY;
}

// 右スティックの値を取得
void InputManager::GetRightStick(int gamepadIndex, short& x, short& y)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX)
	{
		x = 0;
		y = 0;
		return;
	}
	x = gamepadInfos_[gamepadIndex].AKeyRX;
	y = gamepadInfos_[gamepadIndex].AKeyRY;
}

// 左スティックのX軸を取得
short InputManager::GetLeftStickX(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return 0;
	return gamepadInfos_[gamepadIndex].AKeyLX;
}

// 左スティックのY軸を取得
short InputManager::GetLeftStickY(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return 0;
	return gamepadInfos_[gamepadIndex].AKeyLY;
}

// 右スティックのX軸を取得
short InputManager::GetRightStickX(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return 0;
	return gamepadInfos_[gamepadIndex].AKeyRX;
}

// 右スティックのY軸を取得
short InputManager::GetRightStickY(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= GAMEPAD_NUM_MAX) return 0;
	return gamepadInfos_[gamepadIndex].AKeyRY;
}

// 入力キーからXZ平面上の方向ベクトルを取得
void InputManager::GetInputDirXZ(VECTOR& vec, int keyUp, int keyDown, int keyLeft, int keyRight)
{
	if (IsNew(keyUp)) vec.z = 1.0f;
	if (IsNew(keyDown)) vec.z = -1.0f;
	if (IsNew(keyLeft)) vec.x = -1.0f;
	if (IsNew(keyRight)) vec.x = 1.0f;
}

// ゲームパッドの更新
void InputManager::UpdateGamePad()
{
	DINPUT_JOYSTATE dState;
	XINPUT_STATE xState;

	auto& pad = gamepadInfos_;

	for (int i = 0; i < GAMEPAD_NUM_MAX; i++)
	{
		GetJoypadDirectInputState(DX_INPUT_PAD1 + i, &dState);
		int ret = GetJoypadXInputState(DX_INPUT_PAD1 + i, &xState);
		if (ret != 0) continue;	// 接続されていない場合はスキップ

		for (int j = 0; j < static_cast<int>(PadInput::MAX); j++)
		{
			pad[i].inputs[j].buttonOld = pad[i].inputs[j].buttonNew;
			pad[i].inputs[j].buttonNew = xState.Buttons[j] != 0;
			if (/*pad[i].isStickAsDpad*/true)
			{
				if (static_cast<PadInput>(j) == PadInput::Up)
					pad[i].inputs[j].buttonNew = pad[i].inputs[j].buttonNew || dState.Y < 0;
				else if (static_cast<PadInput>(j) == PadInput::Down)
					pad[i].inputs[j].buttonNew = pad[i].inputs[j].buttonNew || dState.Y > 0;
				else if (static_cast<PadInput>(j) == PadInput::Left)
					pad[i].inputs[j].buttonNew = pad[i].inputs[j].buttonNew || dState.X < 0;
				else if (static_cast<PadInput>(j) == PadInput::Right)
					pad[i].inputs[j].buttonNew = pad[i].inputs[j].buttonNew || dState.X > 0;
			}
			pad[i].inputs[j].buttonTrgDown = (!pad[i].inputs[j].buttonOld && pad[i].inputs[j].buttonNew);
			pad[i].inputs[j].buttonTrgUp = (pad[i].inputs[j].buttonOld && !pad[i].inputs[j].buttonNew);
			
			// holdTimeの更新
			if (pad[i].inputs[j].buttonNew)
			{
				pad[i].inputs[j].holdTime++;
			}
			else
			{
				if (pad[i].inputs[j].buttonTrgUp)
				{
					pad[i].inputs[j].lastHoldTime = pad[i].inputs[j].holdTime;
				}
				pad[i].inputs[j].holdTime = 0;
			}
		}	

		pad[i].AKeyLX = xState.ThumbLX;
		pad[i].AKeyLY = xState.ThumbLY;
		pad[i].AKeyRX = xState.ThumbRX;
		pad[i].AKeyRY = xState.ThumbRY;
		pad[i].LTrigger = xState.LeftTrigger;
		pad[i].RTrigger = xState.RightTrigger;
	}
}

// 判定を行うキーを追加
void InputManager::Add(int key)
{
	// 重複登録を回避する
	if (keyInfos_.find(key) == keyInfos_.end())
	{
		keyInfos_[key] = { key, false, false, false, false, 0, 0 };
	}
}

// 判定を行うマウスボタンを追加
void InputManager::AddMouse(int button)
{
	// 重複登録を回避する
	if (mouseInfos_.find(button) == mouseInfos_.end())
	{
		mouseInfos_[button] = { button, false, false, false, false, 0, 0 };
	}
}