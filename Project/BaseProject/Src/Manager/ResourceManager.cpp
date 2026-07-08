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

	// „§‚µ‚Ü‚¹‚ñ‚ªA‚Ç‚¤‚µ‚Ä‚àŽg‚¢‚½‚¢•û‚Í
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_FNT = Application::PATH_FONT;

	Resource* res = nullptr;

	//‰æ‘m“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);	//map‚É“o˜^

	res = new RES(RES_T::IMG, PATH_IMG + "Title/ImgPlayer.png");
	resourcesMap_.emplace(SRC::TITLE_PLAYER, res);	//map‚É“o˜^

	//ƒQ[ƒ€ƒXƒ^[ƒg‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_GAMESTART, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_GAMESTART, res);	//map‚É“o˜^
	//ƒ`ƒ…[ƒgƒŠƒAƒ‹‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_TUTORIAL, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_TUTORIAL, res);	//map‚É“o˜^
	//ƒIƒvƒVƒ‡ƒ“‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_OPTION, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_OPTION, res);	//map‚É“o˜^
	//I—¹‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_End.png");
	resourcesMap_.emplace(SRC::TITLE_END, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_End.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_END, res);	//map‚É“o˜^
	// ƒJ[ƒ\ƒ‹‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Orange.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_ORANGE, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Blue.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_BLUE, res);	//map‚É“o˜^
	//PlayerHPUI‰æ‘œ
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_0.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_0, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_1.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_1, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_2.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_2, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_3.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_3, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_4.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_4, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_5.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_5, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_6.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_6, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_7.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_7, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_8.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_8, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_9.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_9, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_10.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_10, res);	//map‚É“o˜^
	//ƒQ[ƒ€ƒI[ƒo[
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Continue.png");
	resourcesMap_.emplace(SRC::IMG_OFF_CONTINUE, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Retry.png");
	resourcesMap_.emplace(SRC::IMG_OFF_RETRY, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Title.png");
	resourcesMap_.emplace(SRC::IMG_OFF_TITLE, res);	//map‚É“o˜^

	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Continue.png");
	resourcesMap_.emplace(SRC::IMG_ON_CONTINUE, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Retry.png");
	resourcesMap_.emplace(SRC::IMG_ON_RETRY, res);	//map‚É“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Title.png");
	resourcesMap_.emplace(SRC::IMG_ON_TITLE, res);	//map‚É“o˜^



	//ƒ‚ƒfƒ‹“o˜^ 
	//ƒƒCƒ“‚Ì˜f¯ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL +"Stage/PitfallPlanet/PitfallPlanet.mv1");
	resourcesMap_.emplace(SRC::PIT_FALL_PLANET, res);	//map‚É“o˜^
	//ƒTƒu‚Ì˜f¯ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/SpherePlanet/SpherePlanet.mv1");
	resourcesMap_.emplace(SRC::SPHERE_PLANET, res);	//map‚É“o˜^

	//ƒvƒŒƒCƒ„[ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);	//map‚É“o˜^

	// ƒL[ƒuƒŒ[ƒh‚Pƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade1.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_1, res);	//map‚É“o˜^
	// ƒL[ƒuƒŒ[ƒh‚Qƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade2.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_2, res);	//map‚É“o˜^	
	// ƒL[ƒuƒŒ[ƒh‚Rƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade3.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_3, res);	//map‚É“o˜^

	//ƒXƒJƒCƒh[ƒ€ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);	//map‚É“o˜^
	//ƒGƒlƒ~[ƒ‰ƒbƒg
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rat/Rat.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RAT, res);	//map‚É“o˜^

	// ƒvƒŒƒCƒ„[‰e
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	//ƒGƒlƒ~[ƒ‰ƒbƒg
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Rase.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE, res);	//map‚É“o˜^

	//“G‚ÌŽ‹–ì
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Robot/Cone.mv1");
	resourcesMap_.emplace(SRC::VIEW_RANGE, res);	//map‚É“o˜^

	// ÔFƒZƒ‹
	res = new RES(RES_T::IMG, PATH_IMG + "Blocks/RedBlock.png");
	resourcesMap_.emplace(SRC::CELL_RED, res);

	//ƒQ[ƒ€ƒXƒe[ƒW
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/GameStage/GameStage.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAGE, res);	//map‚É“o˜^

	//ƒQ[ƒ€ƒXƒe[ƒW
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/StageWall/StageWall.mv1");
	resourcesMap_.emplace(SRC::BATTLE_WALL, res);	//map‚É“o˜^

	//ƒAƒCƒeƒ€HP
	res = new RES(RES_T::MODEL, PATH_MDL + "Item/HpItem.mv1");
	resourcesMap_.emplace(SRC::ITEM_HP, res);	//map‚É“o˜^

	//ƒAƒCƒeƒ€ƒXƒLƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Fireball.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE_BALL, res);	//map‚É“o˜^

	//ƒ‰[ƒWƒGƒlƒ~[
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE, res);	//map‚É“o˜^

	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Ring.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE_RING, res);	//map‚É“o˜^
	//ƒvƒŒƒCƒ„[ƒQ[ƒ€ƒI[ƒo[—p
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player_GameOver.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);	//map‚É“o˜^

	// ƒIƒvƒVƒ‡ƒ“”wŒi
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::OPTION_BACKGROUND, res);	//map‚É“o˜^

	// ƒXƒ‰ƒCƒ_[˜g
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderFrame.png");
	resourcesMap_.emplace(SRC::SLIDER_FRAME, res);	//map‚É“o˜^

	// ƒXƒ‰ƒCƒ_[ƒmƒu
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderKnob.png");
	resourcesMap_.emplace(SRC::SLIDER_KNOB, res);	//map‚É“o˜^

	// ƒ`ƒFƒbƒNƒ{ƒbƒNƒXON
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckON.png");
	resourcesMap_.emplace(SRC::CHECKBOX_ON, res);	//map‚É“o˜^
	// ƒ`ƒFƒbƒNƒ{ƒbƒNƒXOFF
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckOFF.png");
	resourcesMap_.emplace(SRC::CHECKBOX_OFF, res);	//map‚É“o˜^

	//‚¢‚Á‚½‚ñƒGƒtƒFƒNƒg‚ð’u‚¢‚Æ‚«‚Ü‚·‚í
	//ƒpƒ[ƒAƒbƒvƒGƒtƒFƒNƒg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "PowerUp/PowerUp.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_POWER_UP, res);	//map‚É“o˜^

	//‰ñ•œƒGƒtƒFƒNƒg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Heal/Heal.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HEAL, res);	//map‚É“o˜^

	//ƒqƒbƒgƒGƒtƒFƒNƒg
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Ster/Ster.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, res);	//map‚É“o˜^

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

	// ƒ[ƒhÏ‚Ýƒ`ƒFƒbƒN
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// ƒŠƒ\[ƒX“o˜^ƒ`ƒFƒbƒN
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// “o˜^‚³‚ê‚Ä‚¢‚È‚¢
		return dummy_;
	}

	// ƒ[ƒhˆ—
	rPair->second->Load();

	// ”O‚Ì‚½‚ßƒRƒs[ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
