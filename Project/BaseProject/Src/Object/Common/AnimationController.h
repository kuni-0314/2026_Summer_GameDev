#pragma once
#include <string>
#include <map>

class AnimationController
{

public:

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// コンストラクタ
	AnimationController(int modelId);

	// デストラクタ
	~AnimationController();

	// 外部FBXからアニメーション追加
	void Add(int type, float speed, const std::string path);
	
	// 同じFBX内のアニメーションを準備
	void AddInFbx(int type, float speed, int animIndex);

	// アニメーション再生
	void Play(int type, bool isLoop = true, bool isReset = false);

	// 更新
	void Update();

	// 解放
	void Release();

	// 再生中のアニメーション
	int GetPlayType() const;

	// 再生終了
	bool IsEnd() const;

	// 再生中のアニメーション情報を取得
	const Animation& GetPlayAnim() const;

	// ルートフレームの移動値を無効化するかしないか
	void SetIgnoreRootMove(bool isIgnore) { isIgnoreRootMove_ = isIgnore; }

	// ルートフレームの設定
	void SetRootFrameNo(int frameNo);
	void SetRootFrameNo(const std::string& frameName);

	// ルートフレームの移動値の調整
	void SetRootMoveOffset(const VECTOR& offset) { rootMoveOffset_ = offset; }

	// 動的オフセットを有効にするかどうか
	void SetDynamicOffset(bool isDynamicOffset) { isDynamicOffsetY_ = isDynamicOffset; }

	// ルートフレームの設定をまとめて行う
	void SetRootFrameParams(bool isIgnore, int frameNo, const VECTOR& offset)
	{
		SetIgnoreRootMove(isIgnore);
		SetRootFrameNo(frameNo);
		SetRootMoveOffset(offset);
	}
	void SetRootFrameParams(bool isIgnore, const std::string& frameName, const VECTOR& offset)
	{
		SetIgnoreRootMove(isIgnore);
		SetRootFrameNo(frameName);
		SetRootMoveOffset(offset);
	}

	// 再生中のアニメーションのフレーム数を取得
	int GetAnimFrameNum();

	// アニメーションの停止状態を設定
	bool IsStopped() const { return isStopped_; }
	void SetStopped(bool isStopped) { isStopped_ = isStopped; }

private:

	// アニメーションするモデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	// 再生中のアニメーション
	int playType_;
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーション追加の共通処理
	void Add(int type, float speed, Animation& animation);

	// アニメーション更新前処理
	void UpdateBeforeAnimation();

	// ROOTフレームの移動値を無効化するかしないか
	bool isIgnoreRootMove_ = false;

	// ルートフレーム番号
	int rootFrameNo_ = -1;

	// 調整用のルートフレームの移動値
	VECTOR rootMoveOffset_ = { 0.0f, 0.0f, 0.0f };

	// 動的オフセットを有効にするかどうか
	bool isDynamicOffsetY_;

	// アニメーションの停止状態
	bool isStopped_ = false;
};
