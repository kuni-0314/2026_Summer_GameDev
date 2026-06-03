#include "UIBase.h"

UIBase::UIBase(void)
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

UIBase::~UIBase(void)
{
}

bool UIBase::IsActive(void) const
{
	return isActive_;
}

void UIBase::SetActive(bool active)
{
	isActive_ = active;
}

float UIBase::GetStep(void) const
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

void UIBase::InitLoad(void)
{
	// UIは画像リソースを使用
	// 派生クラスで実装
}

void UIBase::InitTransform(void)
{
	// UIはTransform不要
	// 空実装
}

void UIBase::InitCollider(void)
{
	// UIはコライダー不要
	// 空実装
}

void UIBase::InitAnimation(void)
{
	// UIはアニメーション不要
	// 空実装
}

void UIBase::InitPost(void)
{
	// 派生クラスで必要に応じて実装
}
