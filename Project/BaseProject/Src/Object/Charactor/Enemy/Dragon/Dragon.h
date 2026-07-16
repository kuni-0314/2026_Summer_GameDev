#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include <vector>
#include "../EnemyBase.h"

class Dragon : public EnemyBase
{

public:

	Dragon(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// デストラクタ
	~Dragon() override;

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
};

