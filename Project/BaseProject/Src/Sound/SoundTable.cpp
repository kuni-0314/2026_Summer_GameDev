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
		
		{ SoundID::BGM_TITLE,"Data/Sound/BGM/TitleBgm.wav" },
		{ SoundID::BGM_TITLE2,Application::PATH_SOUND_BGM+"TitleBgm.wav" }

	};
}

// ゲーム画面用
namespace SoundTable_Game
{
	static const std::unordered_map<SoundID, std::string> Table =
	{
		{ SoundID::BGM_TITLE, Application::PATH_SOUND_BGM + "TitleBgm.wav" },
	};
}

// システム用
namespace SoundTable_Skill
{
	static const std::unordered_map<SoundID, std::string> Table =
	{


	};
}

