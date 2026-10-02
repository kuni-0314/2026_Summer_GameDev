#pragma once
#include "SceneBase.h"

class OverScene : public SceneBase
{
public:

	enum class SELECT
	{
		RETRY,	  //最初から
		TITLE	  //タイトルから
	};

	// コンストラクタ
	OverScene();

	// デストラクタ
	~OverScene() override;

	// 初期化
	void Init() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;


private:

	//セレクト位置
	static constexpr int SELECT_RETRY_POS_X = 500;
	static constexpr int SELECT_TITLE_POS_X = 500;
	static constexpr int SELECT_RETRY_POS_Y = 550;
	static constexpr int SELECT_TITLE_POS_Y = 750;

	SELECT select_;

	int playerHandle_;

	//非選択時
	int imgOnTitleHandle_;
	int imgOnRetryHandle_;
	int imgOnContinueHandle_;
	//選択時
	int imgOffTitleHandle_;
	int imgOffRetryHandle_;
	int imgOffContinueHandle_;

	int maxIndex ;
	int minIndex ;

	int selectCount_;

	void SelectChange(SELECT next);

	void SelectUpdate();
};


