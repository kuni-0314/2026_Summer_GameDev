#pragma once
#include <string>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
class FpsController;

class Application
{

public:

	// 固定FPS
	static constexpr int FRAME_RATE = 60;


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

	// オーディオデバイスの初期化
	void InitAudioDevice();

	// FPS制御
	FpsController* fpsController_;

	// オーディオ関連
	IMMDeviceEnumerator* pEnumerator_;	// デバイス列挙子
	IMMDevice* pDevice_;				// オーディオデバイス
	IAudioEndpointVolume* pVolume_;		// ボリュームコントロール

};