#include "AudioManager.h"
#include <DxLib.h>
#include <algorithm>
#include "SoundTable.cpp"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager::AudioManager()
{
	currentBgm_ = static_cast<SoundID>(-1);
}

AudioManager::~AudioManager()
{
	// 念のため削除関数
	DeleteAll();

	pVolume_->Release();
	pDevice_->Release();
	pEnumerator_->Release();
}

void AudioManager::Init()
{
	HRESULT hr = CoInitialize(nullptr);
	//if (FAILED(hr))
	//{
	//	return;
	//}
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pEnumerator_);
	//if (FAILED(hr))
	//{
	//	return;
	//}
	InitAudioDevice();
	float volume = 0.0f;
	pVolume_->GetMasterVolumeLevelScalar(&volume);

	// 現在再生されているBGM
	currentBgm_ = static_cast<SoundID>(-1);

	const int DEFAULT_VOLUME = 100;
	bgmVolume_ = DEFAULT_VOLUME;// bgm音量
	seVolume_ = DEFAULT_VOLUME;// se音量
	//masterVolume_ = 255;// master音量
	masterVolume_ = static_cast<int>(volume * 255.0f);
}

void AudioManager::LoadSceneSound(LoadScene scene)
{
	// 空のテーブルを用意
	const std::unordered_map<SoundID, std::pair<std::string, int>>* table = nullptr;

	// シーンで読み込みたいテーブルを切り替える
	switch (scene)
	{
	case LoadScene::SYSTEM: table = &SoundTable_System::Table; break;
	case LoadScene::TITLE: table = &SoundTable_Title::Table; break;
	case LoadScene::GAME:  table = &SoundTable_Game::Table; break;
	}

	// 存在しないテーブルを選んでいたら処理しない
	if (!table) return;

	// 指定したテーブルのサウンドを全て読み込む
	for (auto& [id, info] : *table)
	{
		const auto& [path, baseVol] = info;

		// すでに読み込まれているか？
		if (handles_.find(id) != handles_.end())
			// 読み込まれているのでスキップ
			continue;

		// サウンドを読み込む
		handles_[id] = LoadSoundMem(path.c_str());
		
		// 基礎音量を設定（0～255に収める）
		baseVolumes_[id] = std::clamp(baseVol, 0, 255);
	}
}

void AudioManager::DeleteSceneSound(LoadScene scene)
{
	// 空のテーブルを用意
	const std::unordered_map<SoundID, std::pair<std::string, int>>* table = nullptr;

	// シーンで削除したいテーブルを切り替える
	switch (scene)
	{
	case LoadScene::SYSTEM: table = &SoundTable_System::Table; break;
	case LoadScene::TITLE: table = &SoundTable_Title::Table; break;
	case LoadScene::GAME:  table = &SoundTable_Game::Table; break;
	}

	// 存在しないテーブルを選んでいたら処理しない
	if (!table) return;

	// 指定したテーブルのサウンドを削除する
	for (auto& [id, _] : *table)
	{
		// IDからサウンドハンドルを抽出
		auto it = handles_.find(id);

		// サウンドが読み込まれているか？
		if (it != handles_.end())
		{
			// 削除する
			DeleteSoundMem(it->second);
			// 配列からも削除
			handles_.erase(it);
		}
	}
}

void AudioManager::PlayBGM(SoundID id)
{
	// IDからサウンドハンドルを抽出
	auto it = handles_.find(id);

	// サウンドが読み込まれているか？
	if (it == handles_.end())
		// 読み込まれていないので終了
		return;

	// 現在のBGMが同じならスキップ
	if (currentBgm_ == id && CheckSoundMem(it->second))
		return;

	// 別のBGMが再生中なら停止
	if (currentBgm_ != static_cast<SoundID>(-1))
		StopBGM();

	// BGMを更新
	currentBgm_ = id;

	// 実音量を計算
	int volume = isMuted_ ? 0 : static_cast<int>(bgmVolume_ * baseVolumes_[id] / 255);

	// 音量を変更
	ChangeVolumeSoundMem(volume, it->second);

	// BGMなのでループ再生
	PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, true);
}

void AudioManager::StopBGM()
{
	// 何も再生されていないなら何もしない
	if (currentBgm_ == static_cast<SoundID>(-1))
		return;

	// IDからサウンドハンドルを抽出
	auto it = handles_.find(currentBgm_);

	// サウンドが読み込まれているか？
	if (it != handles_.end())
		// 読み込まれているのでサウンドを止める
		StopSoundMem(it->second);

	// 現在のBGMを再生していない状態に更新
	currentBgm_ = static_cast<SoundID>(-1);
}

void AudioManager::PlaySE(SoundID id)
{
	// IDからサウンドハンドルを抽出
	auto it = handles_.find(id);

	// サウンドが読み込まれているか？
	if (it == handles_.end())
		// 読み込まれていないので終了
		return;

	// 実音量を計算
	int volume = isMuted_ ? 0 : static_cast<int>(seVolume_ * baseVolumes_[id] / 255);

	// 音量を変更
	ChangeVolumeSoundMem(volume, it->second);

	// SEは複数同時再生を許可
	PlaySoundMem(it->second, DX_PLAYTYPE_BACK, true);
}

void AudioManager::DeleteAll()
{
	// サウンドが1つも読み込まれてないなら処理しない
	if (handles_.empty())
		return;

	// 全てのサウンドを検索
	for (auto& [id, handle] : handles_)
	{
		// エラーサウンドかをチェック
		if (CheckSoundMem(handle) != -1)
		{
			// 削除する
			DeleteSoundMem(handle);
		}
	}

	// 配列をクリア
	handles_.clear();
}

void AudioManager::SetBgmVolume(int volume)
{
	// 0 ～ 255 までに収める
	bgmVolume_ = std::clamp(volume, 0, 255);

	// 別のBGMが再生中
	if (currentBgm_ != static_cast<SoundID>(-1))
	{
		// IDからサウンドハンドルを抽出
		auto it = handles_.find(currentBgm_);

		// サウンドが読み込まれているか？
		if (it != handles_.end())
		{
			// 実音量を計算
			int volume = isMuted_ ? 0 : static_cast<int>(bgmVolume_ * baseVolumes_[currentBgm_] / 255);

			// 音量を変更
			ChangeVolumeSoundMem(volume, it->second);
		}
	}
}

void AudioManager::SetSeVolume(int volume)
{
	// 0 ～ 255 までに収める
	seVolume_ = (std::clamp(volume, 0, 255));
}

void AudioManager::SetMasterVolume(int volume)
{
	// 0 ～ 255 までに収める
	masterVolume_ = std::clamp(volume, 0, 255);

	pVolume_->SetMasterVolumeLevelScalar(static_cast<float>(masterVolume_) / 255.0f, nullptr);
}

void AudioManager::SetMute(bool mute)
{
	if (isMuted_ == mute)
		return;
	isMuted_ = mute;
	
	// 現在のBGMが再生中
	if (currentBgm_ != static_cast<SoundID>(-1))
	{
		// IDからサウンドハンドルを抽出
		auto it = handles_.find(currentBgm_);
		// サウンドが読み込まれているか？
		if (it != handles_.end())
		{
			// 実音量を計算
			int volume = isMuted_ ? 0 : static_cast<int>(bgmVolume_ * baseVolumes_[currentBgm_] / 255);
			// 音量を変更
			ChangeVolumeSoundMem(volume, it->second);
		}
	}
}

void AudioManager::InitAudioDevice()
{
	// 古いものを解放
	if (pVolume_)
	{
		pVolume_->Release();
		pVolume_ = nullptr;
	}

	if (pDevice_)
	{
		pDevice_->Release();
		pDevice_ = nullptr;
	}

	// 現在の既定デバイス取得
	pEnumerator_->GetDefaultAudioEndpoint(
		eRender,
		eConsole,
		&pDevice_
	);

	// 音量操作用インターフェース取得
	pDevice_->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL,
		nullptr,
		(void**)&pVolume_
	);
}

