#pragma once
#include "../../Collider/ColliderBase.h"
#include <vector>
#include "EnemyBase.h"

class ColliderBase;
class Player;
class GameScene;
class Stage;

class EnemyManager
{

public:
	// コンストラクタ
	EnemyManager(GameScene* gamescene, Player* player);
	// デストラクタ
	~EnemyManager(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);
	// エネミー
	const std::vector<EnemyBase*>& GetEemies(void) const { return enemies_; }
	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVから敵情報の読取を行う
	void LoadCsvData(void);
	// エネミー生成
	EnemyBase* Create(const EnemyBase::EnemyData& data, const Player* player);

	// 指定座標に最も近いエネミーの座標を取得
	VECTOR GetNearEnemyPos(const VECTOR& pos) const;

	// 指定IDのエネミーの座標を取得
	VECTOR GetEnemyPos(int id) const;

	bool GetEnemyDead(void);

	void CheckAttackCollision(void);

	// 攻撃コライダを追加
	void AddAttackCollider(const ColliderBase* attackCollider);
	// 攻撃コライダをクリア
	void ClearAttackColliders(void);

private:
	//プレイヤー
	Player* player_;
	//ゲームシーン
	GameScene* gameScene_;

	// エネミー
	std::vector<EnemyBase*> enemies_;

	const ColliderBase* hitCollider_;

	// 攻撃コライダのリスト
	std::vector<const ColliderBase*> attackColliders_;

	bool isDead_ = false;

};

