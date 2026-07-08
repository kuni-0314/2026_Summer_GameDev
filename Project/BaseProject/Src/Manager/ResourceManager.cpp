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

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_FNT = Application::PATH_FONT;

	Resource* res = nullptr;

	//画僧登録
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);	//mapに登録

	res = new RES(RES_T::IMG, PATH_IMG + "Title/ImgPlayer.png");
	resourcesMap_.emplace(SRC::TITLE_PLAYER, res);	//mapに登録

	//ゲームスタート画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_GAMESTART, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_GameStart.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_GAMESTART, res);	//mapに登録
	//チュートリアル画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_TUTORIAL, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Tutorial.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_TUTORIAL, res);	//mapに登録
	//オプション画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_OPTION, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_Option.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_OPTION, res);	//mapに登録
	//終了画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title/Choice_End.png");
	resourcesMap_.emplace(SRC::TITLE_END, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "Title/NotChoice_End.png");
	resourcesMap_.emplace(SRC::TITLE_NOT_END, res);	//mapに登録
	// カーソル画像
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Orange.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_ORANGE, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "UI/Target_Cursor_Blue.png");
	resourcesMap_.emplace(SRC::TARGET_CURSOR_BLUE, res);	//mapに登録
	//PlayerHPUI画像
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_0.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_0, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_1.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_1, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_2.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_2, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_3.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_3, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_4.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_4, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_5.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_5, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_6.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_6, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_7.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_7, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_8.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_8, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_9.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_9, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "PlayerHP/PlayerHP_10.png");
	resourcesMap_.emplace(SRC::IMG_PLAYER_HP_10, res);	//mapに登録
	//ゲームオーバー
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Continue.png");
	resourcesMap_.emplace(SRC::IMG_OFF_CONTINUE, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Retry.png");
	resourcesMap_.emplace(SRC::IMG_OFF_RETRY, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/OFF_Title.png");
	resourcesMap_.emplace(SRC::IMG_OFF_TITLE, res);	//mapに登録

	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Continue.png");
	resourcesMap_.emplace(SRC::IMG_ON_CONTINUE, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Retry.png");
	resourcesMap_.emplace(SRC::IMG_ON_RETRY, res);	//mapに登録
	res = new RES(RES_T::IMG, PATH_IMG + "GameOver/ON_Title.png");
	resourcesMap_.emplace(SRC::IMG_ON_TITLE, res);	//mapに登録



	//モデル登録 
	//メインの惑星モデル
	res = new RES(RES_T::MODEL, PATH_MDL +"Stage/PitfallPlanet/PitfallPlanet.mv1");
	resourcesMap_.emplace(SRC::PIT_FALL_PLANET, res);	//mapに登録
	//サブの惑星モデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/SpherePlanet/SpherePlanet.mv1");
	resourcesMap_.emplace(SRC::SPHERE_PLANET, res);	//mapに登録

	//プレイヤーモデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);	//mapに登録

	// キーブレード１モデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade1.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_1, res);	//mapに登録
	// キーブレード２モデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade2.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_2, res);	//mapに登録	
	// キーブレード３モデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/KeyBlade3.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE_3, res);	//mapに登録

	//スカイドームモデル
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);	//mapに登録
	//エネミーラット
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rat/Rat.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RAT, res);	//mapに登録

	// プレイヤー影
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	//エネミーラット
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Rase.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE, res);	//mapに登録

	//敵の視野
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Robot/Cone.mv1");
	resourcesMap_.emplace(SRC::VIEW_RANGE, res);	//mapに登録

	// 赤色セル
	res = new RES(RES_T::IMG, PATH_IMG + "Blocks/RedBlock.png");
	resourcesMap_.emplace(SRC::CELL_RED, res);

	//ゲームステージ
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/GameStage/GameStage.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAGE, res);	//mapに登録

	//ゲームステージ
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/MainStage/StageWall/StageWall.mv1");
	resourcesMap_.emplace(SRC::BATTLE_WALL, res);	//mapに登録

	//アイテムHP
	res = new RES(RES_T::MODEL, PATH_MDL + "Item/HpItem.mv1");
	resourcesMap_.emplace(SRC::ITEM_HP, res);	//mapに登録

	//アイテムスキル
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Rase/Fireball.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RASE_BALL, res);	//mapに登録

<<<<<<< HEAD
	//ラージエネミー
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE, res);	//mapに登録

	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/Large/Ring.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LARGE_RING, res);	//mapに登録
	//プレイヤーゲームオーバー用
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player_GameOver.mv1");
	resourcesMap_.emplace(SRC::PLAYER_GAMEOVER, res);	//mapに登録

	// オプション背景
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::OPTION_BACKGROUND, res);	//mapに登録

	// スライダー枠
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderFrame.png");
	resourcesMap_.emplace(SRC::SLIDER_FRAME, res);	//mapに登録

	// スライダーノブ
	res = new RES(RES_T::IMG, PATH_IMG + "UI/SliderKnob.png");
	resourcesMap_.emplace(SRC::SLIDER_KNOB, res);	//mapに登録

	// チェックボックスON
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckON.png");
	resourcesMap_.emplace(SRC::CHECKBOX_ON, res);	//mapに登録
	// チェックボックスOFF
	res = new RES(RES_T::IMG, PATH_IMG + "UI/CheckOFF.png");
	resourcesMap_.emplace(SRC::CHECKBOX_OFF, res);	//mapに登録
=======
	//いったんエフェクトを置いときますわ
	//パワーアップエフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "PowerUp/PowerUp.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_POWER_UP, res);	//mapに登録

	//回復エフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Heal/Heal.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HEAL, res);	//mapに登録

	//ヒットエフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Ster/Ster.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, res);	//mapに登録
>>>>>>> origin/EF
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

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
