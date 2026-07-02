#pragma once
#include <DxLib.h>
#include <functional>
#include "../ItemBase.h"
class EnemyBase;
class Player;

class HpItem : public ItemBase
{
public:


	// コンストラクタ
	HpItem(Player* player);
	// デストラクタ
	~HpItem() override;

	void Update()override;

	void Draw() override;

protected:
	// リソースロード
	void InitLoad() override;
	// 大きさ、回転、座標の初期化
	void InitTransform() override;
	// 衝突判定の初期化
	void InitCollider() override;
	// アニメーションの初期化
	void InitAnimation() override;
	// 初期化後の個別処理
	void InitPost() override;
	// 更新系
	void UpdateProcess() override;
	void UpdateProcessPost() override;


private:

	//enemy
	EnemyBase* enemys_;

	Player* player_;

	// モデルの大きさ
	static constexpr float SCALE = 0.8f;
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 70.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 70.0f;

	// 衝突判定用球体半径
	static constexpr float COL_SPHERE_RADIUS = 80.0f;

	//　モデル縮小スピード
	static constexpr float SCL_REDUCTION_SPEED = 0.9f;
	//　モデル最小サイズ
	static constexpr float SCL_REDUCTION_MIN = 0.2f;

	//回復量
	static constexpr int  HEAL_HP = 1;

	bool isGet_ = false;


	VECTOR worldPos;

	VECTOR spawnPos_;

	void PlayerHpGet();


};


