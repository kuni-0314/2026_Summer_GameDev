#pragma once
#include "PlayerState.h"

class PlayerAttackState : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Draw(Player* player) override;

	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL1,
		NORMAL2,
		NORMAL3,
		NORMAL4,
		NORMAL5,
		HEAVY,
		DASH,
		AIR1,
		AIR2,
		AIR3,
		AIR4,
		AIR5,
		FALL,
		MAX
	};

	static constexpr float ATTACK_POW[static_cast<int>(ATTACK_TYPE::MAX)] =
	{
		10.0f,	// NONE
		10.0f,	// NORMAL1
		12.0f,	// NORMAL2
		15.0f,	// NORMAL3
		20.0f,	// NORMAL4
		25.0f,	// NORMAL5
		30.0f,	// HEAVY
		18.0f,  // DASH
		8.0f,   // AIR1
		10.0f,  // AIR2
		12.0f,  // AIR3
		15.0f,  // AIR4
		20.0f,  // AIR5
		25.0f   // FALL
	};

	static constexpr int ATTACK_COOL_TIME[static_cast<int>(ATTACK_TYPE::MAX)] =
	{
		0,		// NONE
		20,		// NORMAL1
		20,		// NORMAL2
		20,		// NORMAL3
		20,		// NORMAL4
		40,		// NORMAL5
		60,		// HEAVY
		30,		// DASH
		20,		// AIR1
		20,		// AIR2
		20,		// AIR3
		20,		// AIR4
		40,		// AIR5
		60		// FALL
	};

	// 攻撃のローカル座標オフセット（x:左右, y:高さ, z:前後）
	static constexpr VECTOR ATTACK_LOCAL_POS = { 10.0f, 100.0f, 70.0f };
	
	// 攻撃の
	static constexpr float ATTACK_RADIUS = 80.0f;

	// 連続攻撃を受け付けるフレーム数
	static constexpr int COMBO_WINDOW_FRAME = 60;

	// 次の攻撃タイプを取得（外部から呼ばれる）
	ATTACK_TYPE GetNextAttackType(Player* player);

private:
	// 攻撃の種類
	ATTACK_TYPE attackType_;

	// 攻撃位置を計算（プレイヤーの向きを考慮）
	VECTOR CalculateAttackPosition(Player* player);
};
