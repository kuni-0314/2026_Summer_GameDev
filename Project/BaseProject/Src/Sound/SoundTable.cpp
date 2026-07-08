#include "SoundTable.h"
#include "../Application.h"

// サウンドID から サウンドパスを取得

// <SoundID, std::pair<std::string, int>>
// soundID: サウンドID
// std::string: サウンドパス
// int: 基礎音量（0～255）たまにうるさい音があるので、音量を下げておく 

// システム用
namespace SoundTable_System
{
	static const std::unordered_map<SoundID, std::pair<std::string, int>> Table =
	{
		{ SoundID::BGM_TITLE, {"Data/Sound/BGM/system.mp3", 100} },

	};
}

// タイトル画面用
namespace SoundTable_Title
{

	static const std::unordered_map<SoundID, std::pair<std::string, int>> Table =
	{
		{ SoundID::BGM_TITLE, {"Data/Sound/BGM/TitleBgm.wav", 255} },
		{ SoundID::BGM_OVER, {"Data/Sound/BGM/GameOver.wav", 255} },
		{ SoundID::SE_TITLE_SELECT, {"Data/Sound/SE/Title_Select.wav", 255} },
		{ SoundID::SE_TITLE_DECISION, {"Data/Sound/SE/Title_Decision.wav", 120} },	// うるさい

	};
}

// ゲーム画面用
namespace SoundTable_Game
{
	static const std::unordered_map<SoundID, std::pair<std::string, int>> Table =
	{
		{ SoundID::BGM_GAME, {"Data/Sound/BGM/GameBgm.wav", 255} },
		{ SoundID::SE_HEAL, {"Data/Sound/SE/Heal.wav", 255} },
		{ SoundID::SE_PLAYER_RUN, {"Data/Sound/SE/Player_Run.wav", 255} },
		{ SoundID::SE_ENEMY_RAT_ATTAK, {"Data/Sound/SE/Rat_Attack.wav", 255} },
		{ SoundID::SE_ENEMY_RASE_ATTACK, {"Data/Sound/SE/Rase_Attack.mp3", 255} },
		{ SoundID::SE_ATTACK_1, {"Data/Sound/SE/Attack1.wav", 255} },
		{ SoundID::SE_ENEMY_HIT, {"Data/Sound/SE/Enemy_Hit.wav", 255} },
		{ SoundID::BGM_WARNIG, {"Data/Sound/BGM/WarnigBgm.wav", 255} },
		{ SoundID::SE_THUNDER, {"Data/Sound/SE/Thunder.wav", 255} } 
	};
}

// オプション画面用
namespace SoundTable_Option
{
	static const std::unordered_map<SoundID, std::pair<std::string, int>> Table =
	{
		{ SoundID::SE_TITLE_SELECT, {"Data/Sound/SE/Title_Select.wav", 255} },	// tmp
	};
}


// 
namespace SoundTable_Skill
{
	static const std::unordered_map<SoundID, std::pair<std::string, int>> Table =
	{
		{ SoundID::SE_TITLE_SELECT, {"Data/Sound/SE/Title_Select.wav", 255} },
	};
}
