#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Collider/ColliderBase.h"
#include "../../Effect/EffectManager.h"
#include "ActorBase.h"

ActorBase::ActorBase()
	: 
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	isAlive_(true)
{
}

ActorBase::~ActorBase()
{
}

void ActorBase::Init()
{

	// リソースロード
	InitLoad();

	// Transform初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

}

void ActorBase::Draw()
{
	if (transform_.modelId != -1)
	{
		MV1DrawModel(transform_.modelId);
	}

#ifdef _DEBUG
	// 所有しているコライダの描画
	for (const auto& own : ownColliders_)
	{
		own.second->Draw();
	}
#endif // _DEBUG
}

void ActorBase::Release()
{
	transform_.Release();

	//エフェクト解放
	EffectManager::GetInstance().Clear();
	EffectManager::GetInstance().Release();

	// 自身のコライダ解放
	for (auto& own : ownColliders_)
	{
		delete own.second;
	}
}

const Transform& ActorBase::GetTransform() const
{
	return transform_;
}

const ColliderBase* ActorBase::GetOwnCollider(int key) const
{
	if (ownColliders_.count(key) == 0)
	{
		return nullptr;
	}
	return  ownColliders_.at(key);
}

void ActorBase::AddHitCollider(const ColliderBase* hitCollider)
{
	for (const auto& c : hitColliders_)
	{
		if (c == hitCollider)
		{
			return;
		}
	}
	hitColliders_.emplace_back(hitCollider);
}

void ActorBase::ClearHitCollider()
{
	hitColliders_.clear();
}

void ActorBase::RemoveHitCollider(const ColliderBase* hitCollider)
{
	auto it = std::find(hitColliders_.begin(), hitColliders_.end(), hitCollider);
	if (it != hitColliders_.end())
	{
		hitColliders_.erase(it);
	}
}
