#pragma once
#include "../ActorBase.h"
class SkyDome :public ActorBase
{ 
public:
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW,
	};

	//モデルスケール
	static constexpr float SCL_SKYDOME= 100.0f;
	//初期座標
	static constexpr VECTOR POS_SKYDOME = { 0.0f, 0.0f, 0.0f };
	//初期回転
	static constexpr VECTOR ROT_LOCAL_SKYDOME = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	//static constexpr VECTOR ROT_LOCAL_SKYDOME = { 0.0f, 0.0f, 0.0f };

	//コンストラクタ
	SkyDome(const Transform& followTransform);
	//デストラクタ
	~SkyDome() override;

	void Update() override;

	void Draw() override;


protected:

	// リソースロード
	void InitLoad() override;

	// 大きさ、回転、座標の初期化
	void InitTransform() override;

	// 衝突判定の初期化
	void InitCollider() override;

	// アニメーションの初期化
	void InitAnimation() override;

	// 初期化後の個別処理
	void InitPost() override;

private:

	const Transform& followTransform_;

	STATE state_; //状態

	void ChangeState(STATE state);

	void ChangeStateNone();
	void ChangeStateStay();
	void ChangeStateFollow();

	void UpdateNone();
	void UpdateStay();
	void UpdateFollow();

	
};


