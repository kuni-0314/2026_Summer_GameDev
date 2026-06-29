#include "SoundTable.h"
#include "../Application.h"

// サウンドID から サウンドパスを取得

// システム用
namespace SoundTable_System
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_TITLE, "Data/Sound/BGM/system.mp3" },
		
	};
}

// タイトル画面用
namespace SoundTable_Title
{

	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_TITLE,"Data/Sound/BGM/TitleBgm.wav"},
		{ SoundID::BGM_OVER,"Data/Sound/BGM/GameOver.wav"},
		{ SoundID::SE_TITLE_SELECT,"Data/Sound/SE/Title_Select.wav"},
		{ SoundID::SE_TITLE_DECISION,"Data/Sound/SE/Title_Decision.wav"},

	};
}

// ゲーム画面用
namespace SoundTable_Game
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_GAME,"Data/Sound/BGM/GameBgm.wav"},
		{ SoundID::SE_HEAL,"Data/Sound/SE/Heal.wav"},
		{ SoundID::SE_PLAYER_RUN,"Data/Sound/SE/Player_Run.wav"},
		{ SoundID::SE_ENEMY_RAT_ATTAK,"Data/Sound/SE/Rat_Attack.wav"},
		{ SoundID::SE_ENEMY_RASE_ATTACK,"Data/Sound/SE/Rase_Attack.mp3"},
		{ SoundID::SE_ATTACK_1,"Data/Sound/SE/Attack1.wav"},
		{ SoundID::SE_ENEMY_HIT,"Data/Sound/SE/Enemy_Hit.wav"},
		{ SoundID::BGM_WARNIG,"Data/Sound/BGM/WarnigBgm.wav"},
	};
}

// システム用
namespace SoundTable_Skill
{
	static const std::unordered_map<SoundID, std::string> Table =
	{


	};
}
