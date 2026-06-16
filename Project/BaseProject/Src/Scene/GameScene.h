#pragma once
#include <memory>
#include <vector>
#include <DxLib.h>
#include "SceneBase.h"
#include "../Manager/PostEffectManager.h"
#include "../Object/Collider/ColliderBase.h"

class Stage;
class StageWall;
class SkyDome;
class Player;
class EnemyManager;
class ItemManger;
//class ColliderBase;

class GameScene : public SceneBase
{
public:
	// 攻撃コライダーデータ
	struct AttackColliderData
	{
		ColliderBase* collider;
		float damage;
		int lifeTime;
	};

	// 
	enum CAM_MODE
	{
		MANUAL,
		TARGETING,
	};;

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	// 攻撃コライダー生成
	void CreateAttackCollider(ColliderBase::TAG tag, VECTOR pos, float radius, float damage, int lifeTime);

	ItemManger* GetItemManger() const { return itemManger_; }

private:
	// ステージ
	Stage* stage_;

	// ステージの壁
	std::unique_ptr<StageWall> stageWall_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;

	// 敵マネージャー
	EnemyManager* enemyManager_;

	// アイテムマネージャー
	ItemManger* itemManger_;

	// 攻撃コライダーリスト
	std::vector<AttackColliderData*> attackColliders_;

	// ポストエフェクト用スクリーン
	int postEffectScreen_;

	// 現在適用しているエフェクト(単一選択モード用)
	PostEffectManager::EFFECT_TYPE currentEffect_;

	// 複数エフェクト適用リスト
	std::vector<PostEffectManager::EFFECT_TYPE> activeEffects_;

	// エフェクトモード切り替え (true: 複数, false: 単一)
	bool multiEffectMode_;

	// エフェクトの時間パラメータ
	float effectTime_;

	// ターゲット敵のID
	int targetEnemyId_;

	// ターゲット位置
	VECTOR targetPos_;

	// カメラモード
	CAM_MODE camMode_;

	// エフェクトをトグル(追加/削除)
	void ToggleEffect(PostEffectManager::EFFECT_TYPE effectType);

	// エフェクト名を取得
	const char* GetEffectName(PostEffectManager::EFFECT_TYPE effectType);
};