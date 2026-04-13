#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class AnimationController;
class SkyDome;

class TitleScene : public SceneBase
{

public:
	//タイトル画像のY座標
	static constexpr int IMG_TITLE_POS_Y = 150;
	//push画像Y座標
	static constexpr int IMG_PUSH_POS_Y = 500;

	//サブ惑星の大きさ
	static constexpr float SCL_SUB_PLANET = 0.7f;
	//サブ惑星の回転
	static constexpr VECTOR ROT_SUB_PLANET = {90.0f * DX_PI_F/180.0f,0.0f, 0.0f};
	//サブ惑星の座標
	static constexpr VECTOR POS_SUB_PLANET= { -250.0f, -100.0f, -100.0f };

	//playerの大きさ
	static constexpr float SCL_PLAYER = 0.4f;
	//playerの回転
	static constexpr VECTOR ROT_PLAYER = { 0.0f, -90.0f * DX_PI_F / 180.0f,0.0f };
	//playerの回転
	static constexpr VECTOR ROT_LOCAL_PLAYER = { 0.0f, 180.0f * DX_PI_F / 180.0f,0.0f };
	//playerの座標
	static constexpr VECTOR POS_PLAYER = { -250.0f, -32.0f, -105.0f };


	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

private:

	//アニメ―ションコントローラのポインタを作成
	AnimationController* animationController_;
	//スカイドームのポインタ作成
	SkyDome* skyDome_;

	Transform empty_;

	//メイン惑星の座標回転管理
	Transform bigPlanet_;
	//サブ惑星の座標回転管理
	Transform subPlanet_;

	Transform player_;

	int count = 0;

	int imgTitle_;
	int imgPushSpace_;

	bool pushAlive_ = true;

};