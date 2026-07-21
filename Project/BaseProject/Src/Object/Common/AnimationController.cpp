#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
	:
	modelId_(modelId),
	playType_(-1),
	playAnim_(),
	isLoop_(true)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	Add(type, speed, animation);
}

void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.model = -1;
	animation.animIndex = animIndex;

	Add(type, speed, animation);
}

void AnimationController::Play(int type, bool isLoop, bool isReset)
{

	if (playType_ == type)
	{
		if (!isReset)
		{
			// 同じアニメーションだったら再生を継続する
			return;
		}
	}

	if (playType_ != -1)
	{
		// モデルからアニメーションを外す
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}

	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	// 初期化
	playAnim_.step = 0.0f;

	// モデルにアニメーションを付ける
	if (playAnim_.model == -1)
	{
		// モデルと同じファイルからアニメーションをアタッチする
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else
	{
		// 別のモデルファイルからアニメーションをアタッチする
		// DxModelViewerを確認すること(大体0か1)
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}

	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	// アニメーションループ
	isLoop_ = isLoop;

}

void AnimationController::Update()
{
	// アニメーション更新前処理
	UpdateBeforeAnimation();

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 再生
	if (!isStopped_)
	{
		playAnim_.step += (deltaTime * playAnim_.speed);
	}

	// アニメーションが終了したら
	if (playAnim_.step > playAnim_.totalTime)
	{
		if (isLoop_)
		{
			// ループ再生
			playAnim_.step = 0.0f;
		}
		else
		{
			// ループしない
			playAnim_.step = playAnim_.totalTime;
		}
	}

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

}

void AnimationController::UpdateBeforeAnimation()
{
	if (isIgnoreRootMove_)
	{
		// 対象フレームのローカル行列を初期値にリセットする
		MV1ResetFrameUserLocalMatrix(modelId_, rootFrameNo_);
		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(modelId_, rootFrameNo_);
		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す
		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転
		// ここでローカル座標を行列に、そのまま戻さず、
			// 調整したローカル座標を設定する
		if (isDynamicOffset_)
		{
			// 動的に設定
			VECTOR offset = { 0.0f, pos.y, 0.0f };
			mix = MMult(mix, MGetTranslate(offset));
		}
		else
		{
			// 固定値で設定
			mix = MMult(mix, MGetTranslate(rootMoveOffset_));
		}
		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(modelId_, rootFrameNo_, mix);
	}
	else
	{
		// 対象フレームのローカル行列を初期値にリセットする
		MV1ResetFrameUserLocalMatrix(modelId_, rootFrameNo_);
	}
}

void AnimationController::Release()
{

	// 外部FBXのモデル(アニメーション)解放
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}
	
	// 可変長配列をクリアする
	animations_.clear();
	
}

int AnimationController::GetPlayType() const
{
	return playType_;
}

bool AnimationController::IsEnd() const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

const AnimationController::Animation& AnimationController::GetPlayAnim() const
{
	return playAnim_;
}

void AnimationController::SetRootFrameNo(int frameNo)
{
	rootFrameNo_ = frameNo;
}

void AnimationController::SetRootFrameNo(const std::string& frameName)
{
	// フレーム番号をフレーム名で取得する
	rootFrameNo_ = MV1SearchFrame(modelId_, frameName.c_str());
}

int AnimationController::GetAnimFrameNum()
{
	return MV1GetAttachAnimTime(modelId_, playAnim_.attachNo);
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 追加
		animations_.emplace(type, animation);
	}
}
