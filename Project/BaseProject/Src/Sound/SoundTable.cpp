#include "SoundTable.h"
#include "../Application.h"
#include <unordered_map>

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
		{ SoundID::BGM_TITLE, Application::PATH_SOUND_BGM + "GameBgm.wav" },
		

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

