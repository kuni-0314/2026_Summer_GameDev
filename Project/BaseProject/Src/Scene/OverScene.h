#pragma once
#include "SceneBase.h"

class OverScene : public SceneBase
{

public:

	// コンストラクタ
	OverScene(void);

	// デストラクタ
	~OverScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;


private:



};


