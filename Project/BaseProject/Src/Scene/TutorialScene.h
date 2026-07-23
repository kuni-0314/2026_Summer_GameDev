#pragma once
#include "SceneBase.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"

class TutorialScene : public SceneBase
{
public:

	// コンストラクタ
	TutorialScene();
	// デストラクタ
	~TutorialScene() override;
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 解放
	void Release() override;

private:

	int imgTutorialMouse_;
	int imgTutorialPad_;

	int page_;      // 現在表示しているページ
};