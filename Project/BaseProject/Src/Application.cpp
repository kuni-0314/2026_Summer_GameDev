#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"
#include "Common/FpsController.h"
#include "Sound/AudioManager.h"
#define _CRTDBG_MAP_ALLOC

#pragma comment(lib, "wbemuuid.lib")

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SHADER = "Data/Shader/";
const std::string Application::PATH_CSV = "Data/Csv/";
const std::string Application::PATH_SOUND_SE = "Data/Sound/SE/";
const std::string Application::PATH_SOUND_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_FONT = "Data/Font/";


void Application::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance()
{
	return *instance_;
}

void Application::Init()
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _DEBUG
	// 出力ウィンドウにメモリリーク情報を詳細表示
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	// 特定のメモリ割り当て番号でブレークする場合（出力ウィンドウに表示される番号を使用）
	// 例: {123} のようなメモリ割り当て番号が表示されたら、その番号を設定
	// _CrtSetBreakAlloc(123);
	//_CrtSetBreakAlloc(17282);
#endif
	
	// アプリケーションの初期設定
	SetWindowText("3DAction");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// FPS制御初期化
	fpsController_ = new FpsController(fpsLimit_);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	//サウンド管理初期化
	AudioManager::CreateInstance();
	auto audioManager = AudioManager::GetInstance();
	audioManager->Init();
	int volume = audioManager->GetMasterVolume();

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();
	InputManager::GetInstance()->Init();
	InputManager::GetInstance()->SetKeyAndMouseEnabled(true);


	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

#ifndef _DEBUG
	if (volume < 0.01f)
	{
		int result = MessageBoxA(
			nullptr,
			"音量が0やんけ、試遊なのに音なしでするんか？",
			"確認",
			MB_YESNO | MB_ICONEXCLAMATION
		);
		if (result == IDYES)
		{
			MessageBoxA(
				nullptr,
				"音量上げてね",
				"確認",
				MB_OK | MB_ICONINFORMATION
			);
			isEnd_ = true;
		}
		else
		{
			MessageBoxA(
				nullptr,
				"じゃあ遊べんなぁ",
				"残念",
				MB_OK | MB_ICONERROR
			);
			isEnd_ = true;
		}
	}
#endif
}

void Application::Run()
{

	InputManager* inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && !isEnd_)
	{
		// Escapeキーが押されたか判定
		if (CheckHitKey(KEY_INPUT_ESCAPE) || isExitRequested_)
		{
			SetMouseDispFlag(true);
			int result = MessageBoxA
			(
				nullptr,
				"ゲームを終了しウィンドウを閉じますか？セーブしていないゲームデータは失われます。",
				"確認",
				MB_YESNO | MB_ICONEXCLAMATION
			);

			if (result == IDYES)
			{
				break;
			}
			else
			{
				if (isExitRequested_)
				{
					isExitRequested_ = false;	// 終了要求フラグをリセット
				}
			}
		}

		//InitAudioDevice();
		//float volume = 0.0f;
		//pVolume_->GetMasterVolumeLevelScalar(&volume);

		inputManager->Update();
		sceneManager.Update();

		sceneManager.Draw();

		#ifdef _DEBUG
		// 平均FPS描画
		fpsController_->Draw();
		#endif // _DEBUG

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * 0.8f * (1.0f - brightness_ / 100.0f));
		DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();


	}

}

void Application::Destroy()
{
	// FPS制御メモリ解放
	delete fpsController_;
	fpsController_ = nullptr;

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	ResourceManager::GetInstance().Destroy();

	InputManager::GetInstance()->Release();

	// サウンド管理削除
	AudioManager::GetInstance()->DeleteAll();
	AudioManager::DeleteInstance();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	CoUninitialize();

	// インスタンスのメモリ解放
	delete instance_;
	instance_ = nullptr;

	#ifdef _DEBUG
	// プログラム終了直前にメモリリークレポートを出力
	_CrtDumpMemoryLeaks();
	#endif
}

void Application::RequestExit(void)
{
	isExitRequested_ = true;
}

bool Application::IsInitFail() const
{
	return isInitFail_;
}

bool Application::IsReleaseFail() const
{
	return isReleaseFail_;
}

Application::Application()
	:
	isInitFail_(false),
	isReleaseFail_(false),
	fpsController_(nullptr)
{
	isEnd_ = false;
	isExitRequested_ = false;
}

void Application::InitEffekseer()
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

int Application::GetBrightness()
{
	return brightness_;
}

void Application::SetBrightness(int value)
{
	if (value < 0)
		value = 0; // 範囲外の場合は0に設定
	else if (value > 100)
		value = 100; // 範囲外の場合は100に設定
	brightness_ = value;
}

void Application::SetFPSLimit(int fpsLimit)
{
	if (fpsLimit == fpsLimit_) return;
	
	fpsLimit_ = fpsLimit;
	
	// 0（無制限）の場合は非常に高い値に変換
	int actualFPS = (fpsLimit == 0) ? 1200 : fpsLimit;
	fpsController_->ChangeFixedFPS(actualFPS);
}
