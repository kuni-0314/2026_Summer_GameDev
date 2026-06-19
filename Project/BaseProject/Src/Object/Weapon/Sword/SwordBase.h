#pragma once
#include "../../Actor/ActorBase.h"
#include <vector>
#include <map>

class AnimationController;

class SwordBase : public ActorBase
{
public:
	// 剣の種類
	enum class SWORD_TYPE
	{
		NONE,
		KEY_BLADE_1,
		KEY_BLADE_2,
		KEY_BLADE_3,
		MAX
	};

	// コンストラクタ
	SwordBase(SWORD_TYPE type, VECTOR startPos, VECTOR endPos, float radius, const Transform& ownerTransform);

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
	void InitCollider() override;
	virtual void InitAnimation() override = 0;
	virtual void InitPost() override = 0;

	// 剣の種類
	const SWORD_TYPE TYPE;
	
	// 判定開始位置（武器ローカル座標）
	const VECTOR LOCAL_POS_START;

	// 判定終了位置（武器ローカル座標）
	const VECTOR LOCAL_POS_END;

	// 判定半径
	const float RADIUS;

	// 所有者のTransform（プレイヤーなど）
	const Transform& ownerTransform_;

	// 武器モデルの大きさの倍率
	float scl;
};

