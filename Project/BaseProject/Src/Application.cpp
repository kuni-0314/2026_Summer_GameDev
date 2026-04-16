#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"
#include "Common/FpsController.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_CSV = "Data/Csv/";


void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
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
	fpsController_ = new FpsController(FRAME_RATE);

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

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		#ifdef _DEBUG
		// 平均FPS描画
		fpsController_->Draw();
		#endif // _DEBUG

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();


	}

}

void Application::Destroy(void)
{
	// FPS制御メモリ解放
	delete fpsController_;
	fpsController_ = nullptr;

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	
	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;
	instance_ = nullptr;

	#ifdef _DEBUG
	// プログラム終了直前にメモリリークレポートを出力
	_CrtDumpMemoryLeaks();
	#endif
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
	:
	isInitFail_(false),
	isReleaseFail_(false)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
