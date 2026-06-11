#include "UIBase.h"

UIBase::UIBase()
	:
	ActorBase(),
	img_(-1),
	isActive_(false),
	step_(0.0f),
	uiPosX_(0.0f),
	uiPosY_(0.0f),
	alpha_(255)
{
}

UIBase::~UIBase()
{
}

bool UIBase::IsActive() const
{
	return isActive_;
}

void UIBase::SetActive(bool active)
{
	isActive_ = active;
}

float UIBase::GetStep() const
{
	return step_;
}

void UIBase::SetUIPos(float x, float y)
{
	uiPosX_ = x;
	uiPosY_ = y;
}

void UIBase::SetAlpha(int alpha)
{
	// 0?255の範囲に制限
	if (alpha < 0) alpha = 0;
	if (alpha > 255) alpha = 255;
	alpha_ = alpha;
}

void UIBase::InitLoad()
{
	// UIは画像リソースを使用
	// 派生クラスで実装
}

void UIBase::InitTransform()
{
	// UIはTransform不要
	// 空実装
}

void UIBase::InitCollider()
{
	// UIはコライダー不要
	// 空実装
}

void UIBase::InitAnimation()
{
	// UIはアニメーション不要
	// 空実装
}

void UIBase::InitPost()
{
	// 派生クラスで必要に応じて実装
}
