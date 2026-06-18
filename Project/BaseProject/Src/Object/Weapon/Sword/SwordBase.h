#pragma once
#include "../../Actor/ActorBase.h"
#include <vector>
#include <map>

class AnimationController;

class SwordBase : public ActorBase
{
public:
	// 武器の種類
	enum class TYPE
	{
		NONE,
		KEY_BLADE_1,
		KEY_BLADE_2,
		KEY_BLADE_3,
		MAX
	};

	// 攻撃判定情報
	struct HitboxInfo
	{
		// 武器モデルの初期状態はグリップ位置が下で、先端が上に向いている想定
		VECTOR localPosStart;	// 判定開始位置（武器ローカル座標）
		VECTOR localPosEnd;     // 判定終了位置（武器ローカル座標）
		float radius;           // 判定半径
		int activeStartFrame;   // 判定開始フレーム
		int activeEndFrame;     // 判定終了フレーム
		float damageMultiplier; // ダメージ倍率
		bool isActive;          // 現在有効かどうか
	};

	// コンストラクタ
	SwordBase(TYPE type, const Transform& ownerTransform);

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 攻撃開始
	void BeginAttack(int attackType);

	// 攻撃終了
	void EndAttack();

protected:
	virtual void InitLoad() override = 0;
	virtual void InitTransform() override = 0;
	virtual void InitCollider() override = 0;
	virtual void InitAnimation() override = 0;
	virtual void InitPost() override = 0;

	// 武器の種類
	const TYPE type_;

	// 所有者のTransform（プレイヤーなど）
	const Transform& ownerTransform_;

	// 攻撃判定のリスト
	std::vector<HitboxInfo> hitboxes_; 

	// 武器モデルの大きさの倍率
	float scl;
};

