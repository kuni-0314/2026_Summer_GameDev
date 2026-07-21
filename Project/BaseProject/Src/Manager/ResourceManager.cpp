#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance()
{
	return *instance_;
}

void ResourceManager::Init()
{

	// êÑèßÇµÇ‹ÇπÇÒÇ™ÅAÇ«Ç§ÇµÇƒÇ‡égÇ¢ÇΩÇ¢ï˚ÇÕ
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_FNT = Application::PATH_FONT;

	Resource* res = nullptr;

	//âÊëmìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "Title/ImgPlayer.png");
	resourcesMap_.emplace(SRC::TITLE_PLAYER, res);	//mapÇ…ìoò^

	//ÉQÅ[ÉÄÉXÉ^Å[ÉgâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_GAMESTART, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_GAMESTART, res);	//mapÇ…ìoò^
	//É`ÉÖÅ[ÉgÉäÉAÉãâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_TUTORIAL, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_TUTORIAL, res);	//mapÇ…ìoò^
	//ÉIÉvÉVÉáÉìâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_OPTION, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_OPTION, res);	//mapÇ…ìoò^
	//èIóπâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_End.png");
	resourcesMap_.emplace(SRC::TITLE_END, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_End.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_END, res);	//mapÇ…ìoò^
	// ÉJÅ[É\ÉãâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Orange.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_ORANGE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Blue.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_BLUE, res);	//mapÇ…ìoò^
	//PlayerHPUIâÊëú
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_0.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_0, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_1.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_1, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_2.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_2, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_3.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_3, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_4.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_4, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_5.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_5, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_6.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_6, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_7.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_7, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_8.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_8, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_9.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_9, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_10.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_10, res);	//mapÇ…ìoò^
	//ÉQÅ[ÉÄÉIÅ[ÉoÅ[
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Continue.png");
	resourcesMap_.emplace(SRC::IMG_OFF_CONTINUE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Retry.png");
	resourcesMap_.emplace(SRC::IMG_OFF_RETRY, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Title.png");
	resourcesMap_.emplace(SRC::IMG_OFF_TITLE, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Continue.png");
	resourcesMap_.emplace(SRC::IMG_ON_CONTINUE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Retry.png");
	resourcesMap_.emplace(SRC::IMG_ON_RETRY, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Title.png");
	resourcesMap_.emplace(SRC::IMG_ON_TITLE, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "UI/Command_Center.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_FIRE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Command_Down.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_RECOVERY, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Command_Up.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_SANDER, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Command_AllUp.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_ALL, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "UI/notuse_Sander.png");
	resourcesMap_.emplace(SRC::IMG_NOTUSE_SANDER, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/notuse_Fire.png");
	resourcesMap_.emplace(SRC::IMG_NOTUSE_FIRE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/notuse_Heal.png");
	resourcesMap_.emplace(SRC::IMG_NOTUSE_RECOVERY, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/used_Sander.png");
	resourcesMap_.emplace(SRC::IMG_USE_SANDER, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/used_Fire.png");
	resourcesMap_.emplace(SRC::IMG_USE_FIRE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/used_Heal.png");
	resourcesMap_.emplace(SRC::IMG_USE_RECOVERY, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_Normal.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_UI_DEF, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_Damage.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_UI_DAMEGE, res);	//mapÇ…ìoò^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_Limit.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_UI_WARNIG, res);	//mapÇ…ìoò^

	res = new RES(RES_T::IMG, PATH_IMG + "UI/LockOn.png");
	resourcesMap_.emplace(SRC::IMG_LOCKON_FONT_UI, res);	//mapÇ…ìoò^

	// ê‘êFÉZÉã
	res = new RES(RES_T::IMG, PATH_IMG + "Blocks/RedBlock.png");
	resourcesMap_.emplace(SRC::CELL_RED, res);
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb

	//ÉQÅ[ÉÄÉXÉeÅ[ÉW
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/GameStage/Stage2.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAGE, res);	//mapÇ…ìoò^

	//ÉQÅ[ÉÄÉXÉeÅ[ÉW
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/StageWall/StageWall.mv1");
	resourcesMap_.emplace(SRC::BATTLE_WALL, res);	//mapÇ…ìoò^

	//ÉAÉCÉeÉÄHP
	res = new RES(RES_T::MODEL, PATH_MDL + "Item/HpItem.mv1");
	resourcesMap_.emplace(SRC::ITEM_HP, res);	//mapÇ…ìoò^

	//ÉAÉCÉeÉÄÉXÉLÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Fireball.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE_BALL, res);	//mapÇ…ìoò^

	//ÉâÅ[ÉWÉGÉlÉ~Å[
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE, res);	//mapÇ…ìoò^

	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Ring.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE_RING, res);	//mapÇ…ìoò^
	//ÉvÉåÉCÉÑÅ[ÉQÅ[ÉÄÉIÅ[ÉoÅ[óp
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player_GameOver.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);	//mapÇ…ìoò^

<<<<<<< HEAD
=======
=======
>>>>>>> origin/FIXÔºí
>>>>>>> 5f503d1053713b141298797423397fa6eca8cdcb
	// ÉIÉvÉVÉáÉìîwåi
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::OPTION_BACKGROUND, res);	//mapÇ…ìoò^

	// ÉXÉâÉCÉ_Å[òg
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderFrame.png");
	resourcesMap_.emplace(SRC::SLIDER_FRAME, res);	//mapÇ…ìoò^

	// ÉXÉâÉCÉ_Å[ÉmÉu
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderKnob.png");
	resourcesMap_.emplace(SRC::SLIDER_KNOB, res);	//mapÇ…ìoò^

	// É`ÉFÉbÉNÉ{ÉbÉNÉXON
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckON.png");
	resourcesMap_.emplace(SRC::CHECKBOX_ON, res);	//mapÇ…ìoò^
	// É`ÉFÉbÉNÉ{ÉbÉNÉXOFF
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckOFF.png");
	resourcesMap_.emplace(SRC::CHECKBOX_OFF, res);	//mapÇ…ìoò^

	//ÉÇÉfÉãìoò^ 

	//ÉvÉåÉCÉÑÅ[ÉÇÉfÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);	//mapÇ…ìoò^

	// ÉLÅ[ÉuÉåÅ[ÉhÇPÉÇÉfÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade1.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_1, res);	//mapÇ…ìoò^
	// ÉLÅ[ÉuÉåÅ[ÉhÇQÉÇÉfÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade2.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_2, res);	//mapÇ…ìoò^	
	// ÉLÅ[ÉuÉåÅ[ÉhÇRÉÇÉfÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade3.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_3, res);	//mapÇ…ìoò^

	//ÉXÉJÉCÉhÅ[ÉÄÉÇÉfÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);	//mapÇ…ìoò^
	// ÉvÉåÉCÉÑÅ[âe
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	//ÉGÉlÉ~Å[ÉâÉbÉg
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rat/Rat.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RAT, res);	//mapÇ…ìoò^
	//ÉGÉlÉ~Å[ÉåÉCÉY
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Rase.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE, res);	//mapÇ…ìoò^
	//ÉâÅ[ÉWÉGÉlÉ~Å[
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE, res);	//mapÇ…ìoò^
	//ÉâÅ[ÉWópè’åÇîg
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Ring.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE_RING, res);	//mapÇ…ìoò^
	//ÉGÉlÉ~Å[ÉhÉâÉSÉì
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Dragon/Idle_1.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DRAGON, res);	//mapÇ…ìoò^
	

	//ÉQÅ[ÉÄÉXÉeÅ[ÉW
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/GameStage/GameStage.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAGE, res);	//mapÇ…ìoò^
	//ÉAÉCÉeÉÄHP
	res = new RES(RES_T::MODEL, PATH_MDL + "Item/HpItem.mv1");
	resourcesMap_.emplace(SRC::ITEM_HP, res);	//mapÇ…ìoò^
	//ÉAÉCÉeÉÄÉXÉLÉã
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Fireball.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE_BALL, res);	//mapÇ…ìoò^
	//ÉvÉåÉCÉÑÅ[ÉQÅ[ÉÄÉIÅ[ÉoÅ[óp
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player_GameOver.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);	//mapÇ…ìoò^

	

	//Ç¢Ç¡ÇΩÇÒÉGÉtÉFÉNÉgÇíuÇ¢Ç∆Ç´Ç‹Ç∑ÇÌ
	//ÉpÉèÅ[ÉAÉbÉvÉGÉtÉFÉNÉg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "PowerUp/PowerUp.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_POWER_UP, res);	//mapÇ…ìoò^

	//âÒïúÉGÉtÉFÉNÉg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Heal/Heal.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HEAL, res);	//mapÇ…ìoò^

	//ÉqÉbÉgÉGÉtÉFÉNÉg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Ster/Ster.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, res);	//mapÇ…ìoò^

	//éÄñSÉGÉtÉFÉNÉg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Death/Death.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_DEATH, res);	//mapÇ…ìoò^

	// ÉIÉvÉVÉáÉìòg
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Option_Frame.png");
	resourcesMap_.emplace(SRC::OPTION_FRAME, res);	//mapÇ…ìoò^

}

void ResourceManager::Release()
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy()
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

const Resource& ResourceManager::Load(SRC src)
{
	const Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager()
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ÉçÅ[ÉhçœÇ›É`ÉFÉbÉN
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// ÉäÉ\Å[ÉXìoò^É`ÉFÉbÉN
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// ìoò^Ç≥ÇÍÇƒÇ¢Ç»Ç¢
		return dummy_;
	}

	// ÉçÅ[Éhèàóù
	rPair->second->Load();

	// îOÇÃÇΩÇﬂÉRÉsÅ[ÉRÉìÉXÉgÉâÉNÉ^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
