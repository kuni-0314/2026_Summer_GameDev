#include <DxLib.h>
#include <algorithm>
#include "SoundTable.cpp"
#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager::AudioManager(void)
{
	currentBgm_ = static_cast<SoundID>(-1);
	bgmVolume_ = 255;
	seVolume_ = 255;
	masterVolume_ = 255;
}

AudioManager::~AudioManager()
{
	// 念のため削除関数
	DeleteAll();
}

void AudioManager::Init(void)
{
	// 現在再生されているBGM
	currentBgm_ = static_cast<SoundID>(-1);

	bgmVolume_ = 255;		// bgm音量
	seVolume_ = 255;		// se音量
	masterVolume_ = 255;	// master音量
}

void AudioManager::LoadSceneSound(LoadScene scene)
{
	// 空のテーブルを用意
	const std::unordered_map<SoundID, std::string>* table = nullptr;

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
	for (auto& [id, path] : *table)
	{
		// サウンドを読み込む
		int handle = LoadSoundMem(path.c_str());
		if (handle < 0)
		{
			// ロード失敗 -> ログを残して登録しない（またはエラーハンドリング）
			// 例: OutputDebugStringA(("LoadSoundMem failed: " + path + "\n").c_str());
			continue;
		}
		handles_[id] = handle;
	}
}

void AudioManager::DeleteSceneSound(LoadScene scene)
{
	// 空のテーブルを用意
	const std::unordered_map<SoundID, std::string>* table = nullptr;

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
		return;

	int handle = it->second;
	// ハンドルが無効なら処理しない（登録ミスがある場合を保護）
	if (handle < 0)
	{
		// 不正ハンドルなら登録から削除して終了
		handles_.erase(it);
		return;
	}

	// 現在のBGMが同じなら再生中かチェックしてスキップ
	if (currentBgm_ == id)
	{
		int state = CheckSoundMem(handle);
		if (state == 1) // 1 = 再生中（DXライブラリの想定値。必要なら実値を確認）
			return;
		// state < 0 はエラー -> 登録をクリアして終了
		if (state < 0)
		{
			handles_.erase(it);
			return;
		}
	}

	// 別のBGMが再生中なら停止
	if (currentBgm_ != static_cast<SoundID>(-1))
		StopBGM();

	// BGMを更新
	currentBgm_ = id;

	// 実音量を計算
	int volume = static_cast<int>(bgmVolume_ * (masterVolume_ / 255.0f));

	// 音量を変更
	ChangeVolumeSoundMem(volume, handle);

	// BGMなのでループ再生
	PlaySoundMem(handle, DX_PLAYTYPE_LOOP, true);
}

void AudioManager::StopBGM()
{
	// 何も再生されていないなら何もしない
	if (currentBgm_ == static_cast<SoundID>(-1))
		return;

	auto it = handles_.find(currentBgm_);
	if (it != handles_.end() && it->second >= 0)
		StopSoundMem(it->second);

	// 現在のBGMを再生していない状態に更新
	currentBgm_ = static_cast<SoundID>(-1);
}

void AudioManager::PlaySE(SoundID id)
{
	auto it = handles_.find(id);
	if (it == handles_.end()) return;

	int handle = it->second;
	if (handle < 0)
	{
		handles_.erase(it);
		return;
	}

	int volume = static_cast<int>(seVolume_ * (masterVolume_ / 255.0f));
	ChangeVolumeSoundMem(volume, handle);
	PlaySoundMem(handle, DX_PLAYTYPE_BACK, true);
}

void AudioManager::DeleteAll(void)
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
			int volume = static_cast<int>(bgmVolume_ * (masterVolume_ / 255.0f));

			// 音量を変更
			ChangeVolumeSoundMem(volume, it->second);
		}
	}
}

void AudioManager::SetSeVolume(int volume)
{
	// 0 ～ 255 までに収める
	seVolume_ = std::clamp(volume, 0, 255);
}

void AudioManager::SetMasterVolume(int volume)
{
	// 0 ～ 255 までに収める
	masterVolume_ = std::clamp(volume, 0, 255);

	// 再生中のBGMに即時反映
	if (currentBgm_ != static_cast<SoundID>(-1))
	{
		// IDからサウンドハンドルを抽出
		auto it = handles_.find(currentBgm_);

		// サウンドが読み込まれているか？
		if (it != handles_.end())
		{
			// 実音量を計算
			int volume = static_cast<int>(bgmVolume_ * (masterVolume_ / 255.0f));

			// 音量を変更
			ChangeVolumeSoundMem(volume, it->second);
		}
	}
}

