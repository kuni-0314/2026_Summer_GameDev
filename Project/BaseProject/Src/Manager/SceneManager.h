#pragma once
#include <chrono>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// 背景色
	//static constexpr int BACKGROUND_COLOR_R = 0;
	//static constexpr int BACKGROUND_COLOR_G = 139;
	//static constexpr int BACKGROUND_COLOR_B = 139;

	static constexpr int BACKGROUND_COLOR_R = 255;
	static constexpr int BACKGROUND_COLOR_G = 255;
	static constexpr int BACKGROUND_COLOR_B = 255;

	// ディレクショナルライトの方向
	static constexpr VECTOR LIGHT_DIRECTION = { 0.3f, -0.7f, 0.8f };

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		CLEAR,
		OVER,
		DEBUG,
		OPTION,
	};
	
	// インスタンスの生成
	static void CreateInstance();

	// インスタンスの取得
	static SceneManager& GetInstance();

	// 初期化
	void Init();
	
	// 3Dの初期化
	void Init3D();

	// 更新
	void Update();

	// 描画
	void Draw();

	// リソースの破棄
	void Destroy();

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID();

	// デルタタイムの取得
	float GetDeltaTime() const;

	// カメラの取得
	Camera* GetCamera() const;

	// メインスクリーンの取得
	int GetMainScreen() const;

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// カメラ
	Camera* camera_;


	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// メインスクリーン
	int mainScreen_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager() = default;

	// デルタタイムをリセットする
	void ResetDeltaTime();

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade();

};