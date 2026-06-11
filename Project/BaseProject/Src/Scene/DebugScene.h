#pragma once
#include <vector>
#include <DxLib.h>
#include "SceneBase.h"
class Stage;
class DebugScene : public SceneBase
{
public:
	// コンストラクタ
	DebugScene();
	// デストラクタ
	~DebugScene() override;
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 解放
	void Release() override;
private:
	// ステージ
	Stage* stage_;
	// デバッグポイント群
	std::vector<VECTOR> points_;
	// デバッグポイントの配置
	void PlaceDebugPoint();
	// デバッグポイントの保存
	void SavePoints();
};
