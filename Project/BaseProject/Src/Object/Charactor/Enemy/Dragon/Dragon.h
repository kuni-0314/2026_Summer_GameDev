#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include <vector>
#include "../EnemyBase.h"

class EnemyDragon : public EnemyBase
{

public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		FRY_IDLE,
		BREARH,
	};
	
	// 状態
	enum class STATE
	{
		NONE,
		THINK,
		IDLE,
		FLY_IDLE,
		BREARH,
		MOVE,
	};

	EnemyDragon(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// デストラクタ
	~EnemyDragon() override;

	void Draw(void) override;
	void Release(void) override;


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


	//ステージ判定用
	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	//ブレス調整座標
	static constexpr VECTOR ADD_BREATH_POS = { 0.0f, 0.0f, 100.0f };

	static constexpr VECTOR DOWN_BREATH_POS = { 0,100,500 };	//ブレス終了位置（カプセルの終わり）


	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };
	// モデルの大きさ
	static constexpr float SCALE = 120.0f;

	// 状態
	STATE state_;
	// 更新ステップ
	float step_;// 状態管理(更新ステップ)


	struct BreathInfo
	{
		Transform transform = {};
		ColliderCapsule* collider = nullptr;
	};

	BreathInfo breathInfo_;

	//ブレス開始位置
	VECTOR breathTopPos_;
	//ブレス終了位置
	VECTOR breathDownPos_;

	//攻撃ブレス
	


	//状態管理

	// 状態遷移
	void ChangeState(STATE state);


	//切り替え準備
	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateFlayIdle();
	void ChangeStateBreath();

	//状態更新
	void UpdateThink();
	void UpdateIdle();
	void UpdateFlayIdle();
	void UpdateBreath();

	void CreateBreathCollider(BreathInfo& breathInfo);
	
};

