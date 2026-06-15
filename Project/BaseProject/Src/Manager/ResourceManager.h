#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE, //タイトル画像
		TITLE_PUSH,	//push画像
		TITLE_GAMESTART,	//スタート画像
		TITLE_NOT_GAMESTART,//非選択スタート画像
		TITLE_END,	//終了画像
		TITLE_NOT_END,//非選択終了画像
		TITLE_TUTORIAL,	//チュートリアル画像
		TITLE_NOT_TUTORIAL,//非選択チュートリアル画像
		TITLE_OPTION,	//オプション画像
		TITLE_NOT_OPTION,//非選択オプション画像
		TITLE_PLAYER,	//タイトルプレイヤー
		PIT_FALL_PLANET,	//メイン惑星
		SPHERE_PLANET,		//サブ惑星
		PLAYER,				//プレイヤー
		MAIN_STAGE,			//ステージ（ゲームシーン）
		SKY_DOME,			//スカイドーム（ゲームシーン
		PLAYER_SHADOW,		//シャドウ
		ENEMY_RAT,
		ENEMY_RASE,			//らせ
		ENEMY_RASE_BALL,	//攻撃用モデル
		VIEW_RANGE,			//敵の視野
		CELL_RED,			// 赤色セル
		BATTLE_STAGE,		//バトルステージ
		BATTLE_WALL,		//ステージ壁
		ITEM_HP,			//体力アイテム
		ITEM_SKILL,			//スキルアイテム
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
