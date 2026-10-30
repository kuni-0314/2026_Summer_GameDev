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
		NONE = -1,

		GAME,
		TUTORIAL,
		OPTION,
		EXIT
	};

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
	

	void Release() override;

	//　選択コマンドの変更
	void SelectChange(SELECT next);

	// 選択コマンド更新処理
	void SelectUpdate();

	// 選択コマンドの描画
	void SelectDraw(SELECT next);

private:

#pragma region 定数

	//タイトル画像のY座標
	static constexpr int IMG_TITLE_POS_Y = 150;

	//push画像Y座標
	static constexpr int IMG_PUSH_POS_Y = 500;

	//選択画像の座標
	static constexpr int IMG_CHOICE_POS_X = 120;
	static constexpr int IMG_NOT_CHOICE_POS_X = 180;
	static constexpr int IMG_CHOICE_POS_Y = 650;
	static constexpr int IMG_CHOICE_POS_Y_OFFSET = 100;

	//カラー（RGB）
	static constexpr int COLOR_R = 255;
	static constexpr int COLOR_G = 255;
	static constexpr int COLOR_B = 255;

	//セレクト位置
	static constexpr int SELECT_POS_X = 100;
	static constexpr int SELECT_POS_Y = 100;

	//パットバイブレーション
	static constexpr int VIBRATION_INDEX = 1;
	static constexpr int VIBRATION_POWER = 1000;
	static constexpr int VIBRATION_TIME = 1;

	//フェードインスピード
	static constexpr float FADE_SPEED = 0.02f;

	//点滅サイクル
	static constexpr int BLINK_CYCLE = 20;

	//タイトル描画調整位置
	static constexpr int IMG_TITLE_REG_POS_X = 50;
	static constexpr int IMG_TITLE_REG_POS_Y = 20;

	//プレイヤー描画調整位置
	static constexpr int IMG_PLAYER_REG_POS_X = 100;
	static constexpr int IMG_PLAYER_REG_POS_Y = 100;

	//基本SE音量
	static constexpr int SE_SET_VOLUME = 150;

	//エフェクト加算量
	static constexpr float  ADD_EFFECT_POW = 0.01f;

#pragma endregion


#pragma region 変数

	//選択コマンド
	SELECT select_;
	//選択中のコマンド
	int selectCount_ = 0;
	//前回の選択中のコマンド
	int pSelectCount_ = 0;

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

	//最大コマンド番号
	int maxIndex = static_cast<int>(SELECT::EXIT);
	//最小コマンド番号
	int minIndex = static_cast<int>(SELECT::GAME);
	//入力判定処理
	bool pushAlive_ = true;

	//画像エフェクト
	int postEffectScreen_;
	//フェードインフラグ
	bool isFadeIn_;
	float effectTime_;


	SceneManager::SCENE_ID nextScene_;

#pragma endregion

	
};