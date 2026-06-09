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
		{ SoundID::SE_TITLE_SELECT,"Data/Sound/SE/Title_Select.wav"},
		{ SoundID::SE_TITLE_DECISION,"Data/Sound/SE/Title_Decision.wav"},
	};
}

// ゲーム画面用
namespace SoundTable_Game
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		
	};
}

// システム用
namespace SoundTable_Skill
{
	static const std::unordered_map<SoundID, std::string> Table =
	{


	};
}

