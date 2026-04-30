#include <chrono>
#include <DxLib.h>
#include <vector>
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
	ChangeScene(std::make_shared<TitleScene>());

	

}

void SceneManager::Init3D(void)
{

	// 背景色設定
	SetBackgroundColor(
		BACKGROUND_COLOR_R, 
		BACKGROUND_COLOR_G, 
		BACKGROUND_COLOR_B);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	
	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{
	if (scene_.empty()) return;
	
	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	// フェード機能の更新
	fader_->Update();

	//現在のシーンを取得
	std::shared_ptr<SceneBase> nowScene = scene_.back();



	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		//Fade();
		nowScene->Init();
		isSceneChanging_ = false;
	}
	else
	{
		// シーンの更新処理
		if(nowScene) nowScene->Update();
	}

	// カメラ更新
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// シーンの描画処理
	std::vector<std::shared_ptr<SceneBase>> scenesCopy(scene_.begin(), scene_.end());
	for (auto& scene : scenesCopy)
	{
		if (scene) scene->Draw();
	}
	// カメラ描画
	camera_->DrawDebug();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
	
	// 暗転・明転
	//fader_->Draw();

}

void SceneManager::Destroy(void)
{

	// シーンの解放
	for (auto& scene : scene_)
	{
		scene->Release();
	}
	scene_.clear();

	// フェード機能の解放
	delete fader_;
	fader_ = nullptr;

	camera_->Release();
	delete camera_;
	camera_ = nullptr;

	// インスタンスのメモリ解放
	delete instance_;
	instance_ = nullptr;

}

//void SceneManager::ChangeScene(SCENE_ID nextId)
//{
//
//	// フェード処理が終わってからシーンを変える場合もあるため、
//	// 遷移先シーンをメンバ変数に保持
//	waitSceneId_ = nextId;
//
//	// フェードアウト(暗転)を開始する
//	fader_->SetFade(Fader::STATE::FADE_OUT);
//	isSceneChanging_ = true;
//
//}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	//古いシーンの解放
	for (auto& s : scene_)
		s->Release();
	scene_.clear();

	//新しいシーンの追加
	scene_.push_back(scene);
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	//シーンの追加
	scene_.push_back(scene);

	//初期化
	scene->Init();
}

void SceneManager::PopScene(void)
{
	//最新のシーンを外す
	if (scene_.size() > 1)
	{
		scene_.back()->Release();
		scene_.pop_back();
	}

}
//シーンをジャンプする（削除->新規ロード)
void SceneManager::JumpScene(std::shared_ptr<SceneBase> scene)
{
	scene_.clear();
	isSceneChanging_ = true;
	scene_.push_back(scene);
}


float SceneManager::GetDeltaTime(void) const
{	
	//return 1.0f / 60.0f;
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}




SceneManager::SceneManager(void)
{

	
	fader_ = nullptr;
	isSceneChanging_ = false;
	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

//void SceneManager::DoChangeScene(SCENE_ID sceneId)
//{
//
//	// リソースの解放
//	ResourceManager::GetInstance().Release();
//
//	// シーンを変更する
//	sceneId_ = sceneId;
//
//	//// 現在のシーンを解放
//	//if (scene_ != nullptr)
//	//{
//	//	delete scene_;
//	//}
//
//	//switch (sceneId_)
//	//{
//	//case SCENE_ID::TITLE:
//	//	scene_ = new TitleScene();
//	//	break;
//	//case SCENE_ID::GAME:
//	//	scene_ = new GameScene();
//	//	break;
//	//case SCENE_ID::DEBUG:
//	//	scene_ = new DebugScene();
//	//	break;
//	//}
//
//	//// 各シーンの初期化
//	//scene_->Init();
//
//	ResetDeltaTime();
//
//	waitSceneId_ = SCENE_ID::NONE;
//
//}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ =  true;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


