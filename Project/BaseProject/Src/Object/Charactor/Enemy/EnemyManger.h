#pragma once
#include "../../Collider/ColliderBase.h"
#include "../../../Lib/nlohmann/json.hpp"
#include <vector>
#include "EnemyBase.h"

class ColliderBase;
class Player;
class GameScene;
class Stage;

class EnemyManager
{

public:

	enum class WAVE
	{
		//wave開始（演出とか入れる用）
		START,
		WAVE1,
		WAVE2,
		WAVE3,
		BOSS,
		END
	};


	// コンストラクタ
	EnemyManager(GameScene* gamescene, Player* player);
	// デストラクタ
	~EnemyManager();
	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 解放
	void Release();
	// エネミー
	const std::vector<EnemyBase*>& GetEnemies() const { return enemies_; }
	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);
	// 衝突対象となるコライダを指定して削除
	void RemoveHitCollider(const ColliderBase* hitCollider);

	// CSVから敵情報の読取を行う
	void LoadCsvData();
	// エネミー生成
	EnemyBase* Create(const EnemyBase::EnemyData& data, const Player* player);

	// 指定座標に最も近いエネミーの座標を取得
	VECTOR GetNearEnemyPos(const VECTOR& pos) const;

	// 指定IDのエネミーの座標を取得
	VECTOR GetEnemyPos(int id) const;

	//enemyの絶滅フラグ渡し
	bool GetEnemyDead();

	void SpawnEffect(const VECTOR& pos);

	void DeadEffect(const VECTOR& pos);

	void CheckHit(const VECTOR& pos, float radius, int damage);

	//外部wave情報渡し
	WAVE GetWave() const { return wave_; }

	void SetWave(const WAVE wave);

private:
	//プレイヤー
	Player* player_;
	//ゲームシーン
	GameScene* gameScene_;
	//ウェーブ
	WAVE wave_;

	// 敵を追加生成する間隔
	static constexpr float BOSS_SPAWN_INTERVAL = 100.0f; // 約5秒

	static constexpr VECTOR BOSS_POS = { 0,40,0 };
	static constexpr VECTOR LARGE_01_POS = { 300,40,0 };
	static constexpr VECTOR LARGE_02_POS = { -300,40,0 };

	// エネミー
	std::vector<EnemyBase*> enemies_;
	std::vector<EnemyBase::EnemyData> enemyData_;
	//エネミーステータス情報
	std::vector<EnemyBase::EnemyStatus> enemyStatusData_;
	//エネミーWAVE情報
	std::vector<EnemyBase::EnemyWaveData> enemyWaveData_;

	std::vector<int> usedPos_;

	//テスト
	//std::vector<std::shared_ptr<EnemyBase>> testEnemys_;

	const ColliderBase* hitCollider_;

	// 衝突対象コライダ（複数登録を保持するため vector に変更）
	std::vector<const ColliderBase*> hitColliders_;

	// 攻撃コライダのリスト
	std::vector<const ColliderBase*> attackColliders_;

	// 攻撃エフェクト用のモデルハンドルID
	int attackModel_;

	bool isDead_ = false;

	//wave敵全滅フラグ
	bool wave1Clear_ = false;
	bool wave2Clear_ = false;
	bool wave3Clear_ = false;
	bool wave4Clear_ = false;
	bool waveBossClear_ = false;

	// 視野範囲用トランスフォーム
	Transform attackTransform_;

	//HPアイテム生成
	void CreateHpItem();
	//enemysの削除
	void EnemysDelete();
	//enemysの衝突判定
	void EnemysCollision();

	//WAVE切り替え
	void ChangeWave(WAVE wave);
	//WAVEデータ読み込み
	void LoadWaveData(WAVE wave);
	void UpdateWave();

	//ウェーブ別更新
	void UpdateWaveStart();
	void UpdateWave1();
	void UpdateWave2();
	void UpdateWave3();
	void UpdateWave4();
	void UpdateWaveBoss();

	//エネミーWAVE情報読み込み
	void LoadJsonWaveData();

	std::vector<VECTOR>LargePos_;
	std::vector<VECTOR>EnemyPos_;
	void InitEnemyPos();

	// BOSS WAVEの敵生成タイマー
	float bossSpawnTimer_ = 0.0f;
	void SpawnBossEnemy();

	bool bossAlive_ = false;
};

