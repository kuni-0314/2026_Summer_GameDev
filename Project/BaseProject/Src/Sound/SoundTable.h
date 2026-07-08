#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

// サウンドID
enum class SoundID
{
	BGM_TITLE,
	BGM_TITLE2,		//タイトルBGM
	BGM_GAME,
	BGM_WARNIG,		//警告音（ループするからBGM）
	BGM_OVER,
	SE_TITLE_SELECT,//タイトル選択
	SE_TITLE_DECISION,//タイトル決定
	SE_ATTACK_1,//攻撃１
	SE_PLAYER_RUN,
	SE_HEAL,
	SE_ENEMY_RAT_ATTAK,
	SE_ENEMY_RASE_ATTACK,
	SE_ENEMY_HIT,
	MAX

};

// 読み込むシーン
enum class LoadScene
{
	SYSTEM,		// システム
	TITLE,		// タイトル
	GAME,		// ゲーム
	OPTION,		// オプション
	SKILL,
};

// サウンド属性
struct SoundData
{
	std::string path;	// サウンドのパス
};

// サウンドテーブル（名称被りや拡張性の為にクラス化)
class SoundTable
{
public:
	// サウンドテーブルのマップ
	// サウンドID から サウンドパスを取得
	static const std::unordered_map<SoundID, SoundData> Table;

};


