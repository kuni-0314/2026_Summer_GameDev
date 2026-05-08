#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/DebugScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	// フェード機能の初期化
	fader_ = new Fader();
	fader_->Init();

	// カメラ
	camera_ = new Camera();
	camera_->Init();

	// 画面遷移中判定
	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(
		BACKGROUND_COLOR_R,
		BACKGROUND_COLOR_G,
		BACKGROUND_COLOR_B);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);
	SetUseLighting(true);
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);
}

void SceneManager::Update(void)
{
	if (scenes_.empty())
	{
		return;
	}

	// デルタタイム計算
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	// フェード機能の更新
	fader_->Update();
	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		Fade();
	}
	else
	{
		// スタックのトップシーンのみ更新
		scenes_.back()->Update();
	}

	// カメラ更新
	camera_->Update();
}

void SceneManager::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	camera_->SetBeforeDraw();
	UpdateEffekseer3D();

	// スタックのトップシーンのみ描画
	if (!scenes_.empty())
	{
		scenes_.back()->Draw();
	}

	camera_->DrawDebug();
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();
}

void SceneManager::Destroy(void)
{
	// スタック内のすべてのシーンを解放
	for (auto& scene : scenes_)
	{
		if (scene != nullptr)
		{
			scene->Release();
		}
	}
	scenes_.clear();

	// フェード機能の解放
	delete fader_;
	fader_ = nullptr;

	camera_->Release();
	delete camera_;
	camera_ = nullptr;

	delete instance_;
	instance_ = nullptr;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	waitSceneId_ = nextId;
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	// シーンをスタックに追加
	scenes_.push_back(scene);
	scene->Init();
	ResetDeltaTime();
}

void SceneManager::PopScene()
{
	// スタックからシーンを削除（最後の1つは残す）
	if (scenes_.size() > 1)
	{
		if (scenes_.back() != nullptr)
		{
			scenes_.back()->Release();
		}
		scenes_.pop_back();
	}
	ResetDeltaTime();
}

void SceneManager::PopAllScenes()
{
	// スタックをクリア（デバッグシーンから抜ける場合など）
	for (auto& scene : scenes_)
	{
		if (scene != nullptr)
		{
			scene->Release();
		}
	}
	scenes_.clear();
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	fader_ = nullptr;
	isSceneChanging_ = false;
	deltaTime_ = 1.0f / 60.0f;
	camera_ = nullptr;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	ResourceManager::GetInstance().Release();

	sceneId_ = sceneId;

	// 既存のシーンをすべて削除
	for (auto& scene : scenes_)
	{
		if (scene != nullptr)
		{
			scene->Release();
		}
	}
	scenes_.clear();

	// 新しいシーンを作成
	std::shared_ptr<SceneBase> newScene;

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		newScene = std::make_shared<TitleScene>();
		break;
	case SCENE_ID::GAME:
		newScene = std::make_shared<GameScene>();
		break;
	case SCENE_ID::DEBUG:
		newScene = std::make_shared<DebugScene>();
		break;
	default:
		return;
	}

	scenes_.push_back(newScene);
	newScene->Init();

	ResetDeltaTime();
	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			DoChangeScene(waitSceneId_);
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}