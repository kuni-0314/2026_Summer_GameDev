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
	
	auto const ins = InputManager::GetInstance();

	if (!IsSelect_)
	{
		if (ins->IsTrgDown(KEY_INPUT_SPACE))
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

		Application::GetInstance().End();

		break;
	}
}

void TitleScene::SelectDraw(SELECT next)
{
	const char* name = "";

	if (next == SELECT::GAME) name = "ゲームスタート";
	else if (next == SELECT::TUTORIAL) name = "チュートリアル";
	else if (next == SELECT::OPTION) name = "設定";
	else if (next == SELECT::EXIT) name = "終了";

	DrawFormatString(100, 100, GetColor(255, 255, 255), "選択中: %s", name);
}

void TitleScene::SelectUpdate(void)
{
	auto const ins = InputManager::GetInstance();

	//選択コマンド変更

	if (ins->IsTrgDown(KEY_INPUT_UP))
	{
		selectCount_--;
		if (selectCount_ < minIndex)
		{
			selectCount_ = minIndex; // 一番下へ
		}
	}

	if (ins->IsTrgDown(KEY_INPUT_DOWN))
	{
		selectCount_++;
		if (selectCount_ > maxIndex)
		{
			selectCount_ = maxIndex; // 一番上へ
		}
	}
	if (ins->IsTrgDown(KEY_INPUT_SPACE))//決定
	{
		SelectChange((SELECT)selectCount_);
	}
	

}
