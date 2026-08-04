#pragma once
class ResourceManager;
class SceneManager;

class SceneBase
{

public:

	// コンストラクタ
	SceneBase();

	// デストラクタ
	virtual ~SceneBase() = 0;

	// 初期化
	virtual void Init() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 解放
	virtual void Release() = 0;

protected:

	// リソース管理
	ResourceManager& resMng_;

	// シーン管理
	SceneManager& sceMng_;

};
