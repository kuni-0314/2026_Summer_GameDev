#pragma once
#include <DxLib.h>
#include <functional>
#include "../ItemBase.h"
class EnemyBase;

class HpItem : public ItemBase
{
public:


	// コンストラクタ
	HpItem(void);
	// デストラクタ
	~HpItem(void) override;

	void Draw(void) override;
protected:
	// リソースロード
	void InitLoad(void) override;
	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;
	// 衝突判定の初期化
	void InitCollider(void) override;
	// アニメーションの初期化
	void InitAnimation(void) override;
	// 初期化後の個別処理
	void InitPost(void) override;
	// 更新系
	void UpdateProcess(void) override;
	void UpdateProcessPost(void) override;


private:

	//enemy
	EnemyBase* enemys_;

	// モデルの大きさ
	static constexpr float SCALE = 0.7f;
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 20.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 40.0f;

	// 衝突判定用カプセル球体半径
	static constexpr float COL_SPHERE_RADIUS = 50.0f;

	//生存判定
	bool isAlive_ = true;

	VECTOR worldPos;

	VECTOR spawnPos_;


};


