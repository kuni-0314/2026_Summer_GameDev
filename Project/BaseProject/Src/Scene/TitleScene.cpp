#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Common/Quaternion.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Actor/SkyDome/SkyDome.h"


TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	imgPushSpace_(-1),
	bigPlanet_(),
	subPlanet_(),
	player_(),
	animationController_(nullptr),
	skyDome_(nullptr),
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// 定点カメラ
	sceMng_.GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//タイトル画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPushSpace_ = resMng_.Load(ResourceManager::SRC::TITLE_PUSH).handleId_;


	// メイン惑星
	bigPlanet_.SetModel(resMng_.Load(ResourceManager::SRC::PIT_FALL_PLANET).handleId_);
	bigPlanet_.scl = AsoUtility::VECTOR_ONE;
	bigPlanet_.quaRot = Quaternion::Identity();
	bigPlanet_.quaRotLocal = Quaternion::Identity();
	bigPlanet_.pos = AsoUtility::VECTOR_ZERO;

	bigPlanet_.Update();

	//サブ惑星初期化
	subPlanet_.SetModel(resMng_.Load(ResourceManager::SRC::SPHERE_PLANET).handleId_);
	subPlanet_.scl = { SCL_SUB_PLANET,SCL_SUB_PLANET,SCL_SUB_PLANET };
	subPlanet_.quaRot = Quaternion::Euler(ROT_SUB_PLANET);
	subPlanet_.quaRotLocal = Quaternion::Identity();// VGet{AsoUtility::Deg2RadF(90.0f),0.0f,0.0f};
	subPlanet_.pos = POS_SUB_PLANET;

	subPlanet_.Update();

	//プレイヤー初期化
	player_.SetModel(resMng_.Load(ResourceManager::SRC::PLAYER).handleId_);
	player_.scl = { SCL_PLAYER ,SCL_PLAYER ,SCL_PLAYER };
	player_.quaRot = Quaternion::Euler(ROT_PLAYER);
	player_.quaRotLocal = Quaternion::Euler(ROT_LOCAL_PLAYER);
	player_.pos = POS_PLAYER;

	player_.Update();

	//アニメーションコントローラー
	animationController_ = new AnimationController(player_.modelId);
	animationController_->Add(0, 20.0f,Application::PATH_MODEL + "Player/Run.mv1");
	animationController_->Play(0, true);

	//スカイドーム
	skyDome_ = new SkyDome(empty_);
	skyDome_->Init();
	
	//初期選択コマンド
	select_ = SELECT::GAME;
	

}

void TitleScene::Update(void)
{
	// シーン遷移
	auto const& ins = InputManager::GetInstance();

	if (!IsSelect_)
	{
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			pushAlive_ = true;
			IsSelect_ = true;
		}

	
		count++;
		if (count % 70 < 30)
		{
			pushAlive_ = true;
		}
		else
		{
			pushAlive_ = false;
		}	
	}
	else
	{
		SelectUpdate();

		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			SelectChange((SELECT)selectCount_);
			Decision_ = true;
		}


	}

	//サブ惑星の回転
	subPlanet_.quaRot = subPlanet_.quaRot.Quaternion::Mult(
		Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-1.2f), 0.0f));
	subPlanet_.Update();//Updateがない場合は回転しない

	animationController_->Update();

	skyDome_->Update();
}


void TitleScene::Draw(void)
{
	skyDome_->Draw();//スカイドーム描画

	// モデル描画//ここ
	MV1DrawModel(bigPlanet_.modelId);//メイン惑星描画
	MV1DrawModel(subPlanet_.modelId);//サブ惑星描画
	MV1DrawModel(player_.modelId);//player描画


	DrawGraph(Application::SCREEN_SIZE_X/3,IMG_TITLE_POS_Y, imgTitle_, TRUE);

	if (!IsSelect_)
	{
		if (!pushAlive_)
		{
			DrawGraph(Application::SCREEN_SIZE_X / 3, IMG_PUSH_POS_Y, imgPushSpace_, TRUE);
		}
	}
	

	SelectDraw((SELECT)selectCount_);

	const char* name = "";

	if (selectCount_ == static_cast<int>(SELECT::GAME)) name = "ゲームスタート";
	else if (selectCount_ == static_cast<int>(SELECT::TUTORIAL)) name = "チュートリアル";
	else if (selectCount_ == static_cast<int>(SELECT::OPTION)) name = "設定";
	else if (selectCount_ == static_cast<int>(SELECT::EXIT)) name = "終了";

	DrawFormatString(100, 100, GetColor(255, 255, 255), "選択中: %s", name);
}

void TitleScene::Release(void)
{
	//アニメーションコントローラー解放
	animationController_->Release();
	delete animationController_;

	//スカイドーム解放
	skyDome_->Release();
	delete skyDome_;
}

void TitleScene::SelectChange(SELECT next)
{
	select_ = next;

	switch (next)
	{
	case SELECT::GAME:
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case SELECT::TUTORIAL:

		break;
	case SELECT::OPTION:
		
		break;
	case SELECT::EXIT:

		// EXITを選択したときは、まずデフォルトを「いいえ(NO)」にしておく
		ExitCount_ = static_cast<int>(EXIT::NO);
		SelectExit();
		
		

		break;
	}

	
}

void TitleScene::SelectDraw(SELECT next)
{
	select_ = next;

	switch (next)
	{
	case SELECT::GAME:

		break;
	case SELECT::TUTORIAL:
		if (Decision_)
		{
			SelectTutorialDraw();
		}
		break;
	case SELECT::OPTION:
		if (Decision_)
		{
			SelectOptionDraw();
		}
		break;
	case SELECT::EXIT:
		if (Decision_)
		{
			SelectExitDraw();
		}

		break;
	}

}

void TitleScene::SelectUpdate(void)
{
	auto const& ins = InputManager::GetInstance();

	//選択コマンド変更
	if (!Decision_)
	{
		if (ins.IsTrgDown(KEY_INPUT_UP))
		{
			selectCount_--;
			if (selectCount_ < minIndex)
			{
				selectCount_ = minIndex; // 一番下へ
			}
		}

		if (ins.IsTrgDown(KEY_INPUT_DOWN))
		{
			selectCount_++;
			if (selectCount_ > maxIndex)
			{
				selectCount_ = maxIndex; // 一番上へ
			}
		}
	}
	else
	{
		auto const& ins = InputManager::GetInstance();

		if (select_ == SELECT::EXIT || selectCount_ == static_cast<int>(SELECT::EXIT))
		{
			// 左右キーの入力を個別に判定（確実に反応させるため）
			if (ins.IsTrgDown(KEY_INPUT_LEFT) || ins.IsTrgDown(KEY_INPUT_RIGHT))
			{
				// 値を反転させる
				if (ExitCount_ == static_cast<int>(EXIT::YES)) {
					ExitCount_ = static_cast<int>(EXIT::NO);
				}
				else {
					ExitCount_ = static_cast<int>(EXIT::YES);
				}
			}

			// スペースキーで最終決定
			if (ins.IsTrgDown(KEY_INPUT_SPACE))
			{
				if (ExitCount_ == static_cast<int>(EXIT::YES))
				{
					Application::GetInstance().End(); // 終了
				}
				else
				{
					Decision_ = false; // キャンセル
				}
			}

			// Cキーで戻る
			if (ins.IsTrgDown(KEY_INPUT_C))
			{
				Decision_ = false;
			}
		}
	}

}

void TitleScene::SelectOption(void)
{
}

void TitleScene::SelectExit(void)
{
	
}


void TitleScene::SelectTutorial(void)
{
}

void TitleScene::SelectOptionDraw(void)
{
}

void TitleScene::SelectExitDraw(void)
{
	// 背景を暗くする
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);

	int white = GetColor(255, 255, 255);
	int yellow = GetColor(255, 255, 0); // 選択中の方を黄色にする

	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 100, Application::SCREEN_SIZE_Y / 2 - 40, white, "ゲームを終了しますか？");

	// 「はい」の描画
	int yesColor = (ExitCount_ == static_cast<int>(EXIT::YES)) ? yellow : white;
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 60, Application::SCREEN_SIZE_Y / 2 + 20, yesColor, "はい");

	// 「いいえ」の描画
	int noColor = (ExitCount_ == static_cast<int>(EXIT::NO)) ? yellow : white;
	DrawFormatString(Application::SCREEN_SIZE_X / 2 + 20, Application::SCREEN_SIZE_Y / 2 + 20, noColor, "いいえ");
}

void TitleScene::SelectTutorialDraw(void)
{
}
