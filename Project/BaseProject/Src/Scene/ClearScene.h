#pragma once
#include "SceneBase.h"
//class Application;

class ClearScene : public SceneBase
{

public:


	// コンストラクタ
	ClearScene();

	// デストラクタ
	~ClearScene() override;

	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;


private:

	int playerHandle_;

	int imgOnTitleHandle_;
	int imgOffTitleHandle_;

	bool isMouseOver_ = false;
};
