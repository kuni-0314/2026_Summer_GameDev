#pragma once
#include <string>
#include <windows.h>
//#include <Wbemidl.h>
//#include <comdef.h>
class FpsController;

class Application
{

public:

	// 固定FPS
	//static constexpr int FRAME_RATE = 60;


	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;

	// 重力
	static constexpr float GRAVITY = 9.81f * 100.0f;
	static constexpr float GRAVITY_SCALE = 0.975f;


	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_EFFECT;
	static const std::string PATH_SHADER;
	static const std::string PATH_CSV;
	static const std::string PATH_SOUND_BGM;
	static const std::string PATH_SOUND_SE;
	static const std::string PATH_FONT;
	static const std::string PATH_JSON;


	static const std::string PATH_KEY_CONFIG;
	static const std::string PATH_KEY_CONFIG_GAMEPAD;
	static const std::string PATH_KEY_CONFIG_KEYBOARD;
	//-------------------------------------------


	// インスタンスを明示的に生成
	static void CreateInstance();

	// インスタンスの取得
	static Application& GetInstance();

	// 初期化
	void Init();

	// ゲームループの開始
	void Run();

	// リソースの破棄
	void Destroy();

	// アプリケーション終了要求
	void RequestExit(void);	

	// 初期化成功／失敗の判定
	bool IsInitFail() const;

	// 解放成功／失敗の判定
	bool IsReleaseFail() const;

	// 重力の取得
	float GetGravityPow() const { return GRAVITY * GRAVITY_SCALE; }

	void End() { isEnd_ = true; };

	int GetBrightness();
	void SetBrightness(int brightness);

	int GetFPSLimit() const { return fpsLimit_; }
	void SetFPSLimit(int fpsLimit);

	// デバッグ情報の表示設定
	struct ShowInfos
	{
		bool fps = false;				// FPSの表示
		bool memoryUsage = false;		// メモリ使用量の表示
		bool batteryStatus = false;		// バッテリー残量の表示
		bool collider = false;			// コライダー表示
		bool xyzAxis = false;			// XYZ軸の表示
		bool playerPosition = false;	// プレイヤー座標の表示

		bool operator==(const ShowInfos& other) const
		{
			return fps == other.fps &&
				memoryUsage == other.memoryUsage &&
				batteryStatus == other.batteryStatus &&
				collider == other.collider &&
				xyzAxis == other.xyzAxis &&
				playerPosition == other.playerPosition;
		}
	};

	ShowInfos& GetShowInfos();

	struct MemorySize
	{
		SIZE_T bytes;								// バイト数
		SIZE_T Bits() const { return bytes * 8; }	// ビット数
		SIZE_T KB() const { return bytes / 1024; }	// キロバイト数
		SIZE_T Kilobytes() const { return KB(); }	// キロバイト数
		SIZE_T MB() const { return KB() / 1024; }	// メガバイト数
		SIZE_T Megabytes() const { return MB(); }	// メガバイト数
		SIZE_T GB() const { return MB() / 1024; }	// ギガバイト数
		SIZE_T Gigabytes() const { return GB(); }	// ギガバイト数
	};

private:

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// アプリケーション終了要求フラグ
	bool isExitRequested_;

	// アプリケーション終了フラグ
	bool isEnd_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application();

	// コピーコンストラクタも同様
	Application(const Application& instance) = default;

	// デストラクタも同様
	~Application() = default;

	// エフェクシアの初期化
	void InitEffekseer();

	// デバッグ情報の表示更新
	void CheckInfo();

	// メモリ使用量の更新
	void UpdateMemoryUsage();

	// バッテリー残量の更新
	void UpdateBatteryStatus();

	// FPS制御
	FpsController* fpsController_;

	float brightness_ = 100.0f;	// 明るさ（0～100）

	int fpsLimit_ = 60;	// FPS制限（0で制限なし）

	bool isShowBatteryStatus_ = false;		// バッテリー残量の表示

	ShowInfos showInfos_;
	ShowInfos pShowInfos_;

	MemorySize totalMemory_;		// 総メモリ量
	MemorySize availableMemory_;	// 利用可能メモリ量
	MemorySize usedMemory_;			// 使用中メモリ量

	SYSTEM_POWER_STATUS batteryStatus_;	// バッテリー残量情報
};