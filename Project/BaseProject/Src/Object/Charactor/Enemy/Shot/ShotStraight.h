#pragma once

class ShotStraight : public ShotBase
{

public:
	//コントラスタ
	ShotStraight(TYPE type, int baseModelId);
	//デストラクタ
	~ShotStraight(void);



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

	void SetParam(void)override;

};

