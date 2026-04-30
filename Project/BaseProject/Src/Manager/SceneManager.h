#pragma once
#include <chrono>
#include <memory>
#include <list>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// 背景色
	static constexpr int BACKGROUND_COLOR_R = 0;
	static constexpr int BACKGROUND_COLOR_G = 139;
	static constexpr int BACKGROUND_COLOR_B = 139;

	// ディレクショナルライトの方向
	static constexpr VECTOR LIGHT_DIRECTION = { 0.3f, -0.7f, 0.8f };

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		BATTELE,
		SEARCH,
		DEBUG,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期化
	void Init(void);
	
	// 3Dの初期化
	void Init3D(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	//// 状態遷移
	//void ChangeScene(SCENE_ID nextId);
	
	// シーンを変更する（全削除→新規追加）
	void ChangeScene(std::shared_ptr<SceneBase> scene);

	// シーンを積む（上に追加する）
	void PushScene(std::shared_ptr<SceneBase> scene);

	// シーンを外す（上を削除する）
	void PopScene(void);

	// シーンをジャンプする（全削除→新規ロード）
	void JumpScene(std::shared_ptr<SceneBase> scene);


	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera* GetCamera(void) const;



private:

	// 静的インスタンス
	static SceneManager* instance_;


	// フェード
	Fader* fader_;

	// 各種シーン
	//SceneBase* scene_;
	//スタック式で複数シーンを管理をする
	std::list<std::shared_ptr<SceneBase>> scene_;

	// カメラ
	Camera* camera_;


	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	//void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

};