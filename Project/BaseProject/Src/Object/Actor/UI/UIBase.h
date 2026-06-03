#pragma once
#include "../ActorBase.h"

class UIBase : public ActorBase
{
public:
	// コンストラクタ
	UIBase(void);
	// デストラクタ
	virtual ~UIBase(void) override;
	
	// 更新（純粋仮想関数）
	void Update(void) override = 0;
	
	// 描画（純粋仮想関数）
	void Draw(void) override = 0;
	
	// 有効・無効判定
	bool IsActive(void) const;
	// 有効・無効設定
	void SetActive(bool active);
	// 更新ステップ
	float GetStep(void) const;
	
	// UI座標の設定・取得
	void SetUIPos(float x, float y);
	float GetUIPosX(void) const { return uiPosX_; }
	float GetUIPosY(void) const { return uiPosY_; }
	
	// アルファ値の設定・取得（フェード用）
	void SetAlpha(int alpha);
	int GetAlpha(void) const { return alpha_; }

protected:
	// UI用のリソースロード（3Dモデル不要）
	void InitLoad(void) override;
	// Transform不要（UIは2D座標）
	void InitTransform(void) override;
	// コライダー不要
	void InitCollider(void) override;
	// アニメーション不要
	void InitAnimation(void) override;
	// 初期化後処理
	void InitPost(void) override;
	
	// 画像のハンドルID
	int img_;
	// 有効・無効
	bool isActive_;
	// 更新ステップ
	float step_;
	
	// UI座標（スクリーン座標）
	float uiPosX_;
	float uiPosY_;
	
	// 透明度（0~255）
	int alpha_;
};
