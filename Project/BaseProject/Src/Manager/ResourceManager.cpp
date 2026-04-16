#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// „§‚µ‚Ü‚¹‚ñ‚ªA‚Ç‚¤‚µ‚Ä‚àŽg‚¢‚½‚¢•û‚Í
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	Resource* res;

	//‰æ‘m“o˜^
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);	//map‚É“o˜^

	res = new RES(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::TITLE_PUSH, res);	//map‚É“o˜^

	//ƒ‚ƒfƒ‹“o˜^ 

	//ƒƒCƒ“‚Ì˜f¯ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL +"Stage/PitfallPlanet/PitfallPlanet.mv1");
	resourcesMap_.emplace(SRC::PIT_FALL_PLANET, res);	//map‚É“o˜^
	//ƒTƒu‚Ì˜f¯ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/SpherePlanet/SpherePlanet.mv1");
	resourcesMap_.emplace(SRC::SPHERE_PLANET, res);	//map‚É“o˜^
	//ƒvƒŒƒCƒ„[ƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);	//map‚É“o˜^
	//ƒXƒe[ƒWƒ‚ƒfƒ‹
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/MainStage.mv1");
	resourcesMap_.emplace(SRC::MAIN_STAGE, res);	//map‚É“o˜^
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
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Robot/Robot.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ROBOT, res);	//map‚É“o˜^

	//“G‚ÌŽ‹–ì
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Robot/Cone.mv1");
	resourcesMap_.emplace(SRC::VIEW_RANGE, res);	//map‚É“o˜^
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
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

ResourceManager::ResourceManager(void)
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
