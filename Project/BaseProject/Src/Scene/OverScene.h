#pragma once
#include "SceneBase.h"

class OverScene : public SceneBase
{

public:

	enum class SELECT
	{
		CONTINUE, //そのWAVEから
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


