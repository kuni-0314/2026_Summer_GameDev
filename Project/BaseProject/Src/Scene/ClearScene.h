#pragma once

#include "SceneBase.h"

class ClearScene : public SceneBase
{

public:

	// コンストラクタ
	ClearScene(void);
	// デストラクタ
	~ClearScene(void) override;
	// 初期化
	void Init(void) override;
	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解
	void Release(void) override;


private:




};

