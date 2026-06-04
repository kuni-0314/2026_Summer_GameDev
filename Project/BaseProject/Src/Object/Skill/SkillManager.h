#pragma once
#include <vector>
#include <DxLib.h>
#include "SkillBase.h"

class ColliderBase;
class Player;
class GameScene;
class Stage;

class SkillManager
{
	// コンストラクタ
	SkillManager(GameScene* gamescene, Player* player);
	// デストラクタ
	~SkillManager(void);// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);
	// CSVからスキル情報の読取を行う
	void LoadCsvData(void);
	

private:
	//プレイヤー
	Player* player_;
	//ゲームシーン
	GameScene* gameScene_;

	//スキル
	std::vector<SkillBase*> skills_;

	const ColliderBase* hitCollider_;


};
