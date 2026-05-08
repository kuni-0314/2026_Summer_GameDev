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

	static constexpr int BACKGROUND_COLOR_R = 0;
	static constexpr int BACKGROUND_COLOR_G = 139;
	static constexpr int BACKGROUND_COLOR_B = 139;

	static constexpr VECTOR LIGHT_DIRECTION = { 0.3f, -0.7f, 0.8f };

	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		DEBUG,
	};

	static void CreateInstance(void);
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

	// シーン遷移（フェード付き）
	void ChangeScene(SCENE_ID nextId);

	// スタック操作
	void PushScene(std::shared_ptr<SceneBase> scene);	// シーンをプッシュ
	void PopScene();									// シーンをポップ
	void PopAllScenes();								// すべてのシーンをポップ

	SCENE_ID GetSceneID(void);
	float GetDeltaTime(void) const;
	Camera* GetCamera(void) const;

	// 現在のシーン数を取得（デバッグ用）
	size_t GetSceneStackSize(void) const { return scenes_.size(); }

private:

	static SceneManager* instance_;
	std::list<std::shared_ptr<SceneBase>> scenes_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	Fader* fader_;
	Camera* camera_;

	bool isSceneChanging_;
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	SceneManager(void);
	SceneManager(const SceneManager& instance) = default;
	~SceneManager(void) = default;

	void ResetDeltaTime(void);
	void DoChangeScene(SCENE_ID sceneId);
	void Fade(void);
};