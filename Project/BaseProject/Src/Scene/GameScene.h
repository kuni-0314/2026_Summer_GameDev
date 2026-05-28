#pragma once
#include <map>
#include <memory>
#include "SceneBase.h"
#include "../Object/Collider/ColliderBase.h"
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

	ItemManger* GetItemManger(void) const { return itemManger_; }

	struct AttackColliderData
	{
		ColliderBase* collider = nullptr;
		float damage = 0.0f;
		int lifeTime = 0;
	};
	
	void CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float Damage, int lifeTime);

private:
	Stage* stage_;
	std::unique_ptr<StageWall> stageWall_;
	SkyDome* skyDome_;
	Player* player_;
	EnemyManager* enemyManager_;
	FieldManager* fieldManager_;
	ItemManger* itemManger_;

	int targetEnemyId_;

	VECTOR targetPos_;

	std::vector<AttackColliderData*> attackColliders_;
};
