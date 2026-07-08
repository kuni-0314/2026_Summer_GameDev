#pragma once
#include "SceneBase.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"

class AnimationController;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	enum class SELECT
	{
		GAME,
		TUTORIAL,
		OPTION,
		EXIT
	};
	//タイトル画像のY座標
	static constexpr int IMG_TITLE_POS_Y = 150;
	//push画像Y座標
	static constexpr int IMG_PUSH_POS_Y = 500;

	//
	static constexpr int IMG_CHOICE_POS_X = 120;

	static constexpr int IMG_NOT_CHOICE_POS_X = 180;

	static constexpr int IMG_CHOICE_POS_Y = 650;

	static constexpr int IMG_CHOICE_POS_Y_OFFSET = 100;



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
	TitleScene();

	// デストラクタ
	~TitleScene() override;

	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;

	//　選択コマンドの変更
	void SelectChange(SELECT next);

	// 選択コマンドの描画(仮（後で画像に変更））
	void SelectDraw(SELECT next);

	void SelectUpdate();


private:

	SELECT select_; //選択コマンド

	int selectCount_ = 0; //選択中のコマンド

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

	//点滅管理用
	int count = 0;

	//画像ハンドルID
	int imgTitle_;//タイトル
	int imgPushSpace_;//PUSH
	int imgGameStart_;//ゲームスタート
	int imgNotGameStart_;//非選択ゲームスタート
	int imgTutorial_;//チュートリアル
	int imgNotTutorial_;//非選択チュートリアル
	int imgOption_;//オプション
	int imgNotOption_;//非選択オプション
	int imgEnd_;//終了
	int imgNotEnd_;//非選択終了

	int imgPlayer_;//タイトルプレイヤー

	int maxIndex = static_cast<int>(SELECT::EXIT);
	int minIndex = static_cast<int>(SELECT::GAME);

	bool pushAlive_ = true;

	//bool IsSelect_ = false;

	//int mainScreen_;

	int postEffectScreen_;

	bool isFadeIn_;
	float effectTime_;
	static constexpr float FADE_SPEED = 0.02f;

	SceneManager::SCENE_ID nextScene_;

};