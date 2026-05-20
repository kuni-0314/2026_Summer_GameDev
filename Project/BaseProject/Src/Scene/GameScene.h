#pragma once
#include <memory>
#include "SceneBase.h"
class StageWall;
class Stage;
class SkyDome;
class Player;
class EnemyManager;
class FieldManager;
class ItemManger;

class GameScene : public SceneBase
{

public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	
	ItemManger* GetItemManger() const { return itemManger_; }

private:
	Stage* stage_;
	std::unique_ptr<StageWall> stageWall_;
	SkyDome* skyDome_;
	Player* player_;
	EnemyManager* enemyManager_;
	FieldManager* fieldManager_;
	//アイテムマネージャー
	ItemManger* itemManger_;

	int targetEnemyId_;

	VECTOR targetPos_;
};
