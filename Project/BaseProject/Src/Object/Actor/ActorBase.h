#pragma once
#include <map>
#include <vector>
#include <string>
#include "../Common/Transform.h"
class ResourceManager;
class SceneManager;
class ColliderBase;

class ActorBase
{

public:


	enum class COLLIDER_TYPE : std::uint8_t
	{
		LINE,
		CAPSULE,
		SPHERE,
		MODEL,
		VIEW_RANGE,
		PLAYER,
		ITEM,
		MAX,
	};

	// コンストラクタ
	ActorBase();

	// デストラクタ
	virtual ~ActorBase();

	// 初期化
	void Init();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw();

	// 解放
	virtual void Release();

	// 大きさ、回転、座標等の取得
	const Transform& GetTransform() const;
	// 自身の衝突情報取得
	const std::map<int, ColliderBase*>& GetOwnColliders() const
	{
		return ownColliders_;
	}
	// 特定の自身の衝突情報取得
	const ColliderBase* GetOwnCollider(int key) const;

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);
	// 衝突対象となるコライダをクリア
	void ClearHitCollider();

	// 生存状態の取得
	bool IsAlive() const { return isAlive_; }

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

	// 自身の衝突情報
	std::map<int, ColliderBase*> ownColliders_;

	// 衝突相手の情報
	std::vector<const ColliderBase*> hitColliders_;

	// リソースロード
	virtual void InitLoad() = 0;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform() = 0;

	// 衝突判定の初期化
	virtual void InitCollider() = 0;

	// アニメーションの初期化
	virtual void InitAnimation() = 0;

	// 初期化後の個別処理
	virtual void InitPost() = 0;

	//変数
	bool isAlive_;

};
