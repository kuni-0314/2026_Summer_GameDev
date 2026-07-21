#pragma once
#include "PlayerState.h"

class PlayerAttackState : public PlayerState
{
public:
	void Enter(Player* player) override;
	void Update(Player* player) override;
	void Draw(Player* player) override;
	void Exit(Player* player) override;

	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL1,
		NORMAL2,
		NORMAL3,
		NORMAL4,
		NORMAL5,
		HEAVY,
		//DASH,
		//AIR1,
		//AIR2,
		//AIR3,
		//AIR4,
		//AIR5,
		//FALL,
		MAX
	};

	static constexpr float ATTACK_POW[static_cast<int>(ATTACK_TYPE::MAX)] =
	{
		0.0f,	// NONE
		1.0f,	// NORMAL1
		1.0f,	// NORMAL2
		2.0f,	// NORMAL3
		1.0f,	// NORMAL4
		3.0f,	// NORMAL5
		5.0f,	// HEAVY
		//25.0f,  // DASH
		//8.0f,   // AIR1
		//10.0f,  // AIR2
		//12.0f,  // AIR3
		//15.0f,  // AIR4
		//20.0f,  // AIR5
		//25.0f   // FALL
	};

#define ATK_S_ANIM_INDEX 0
#define ATK_E_ANIM_INDEX 1
	static constexpr int ATTACK_FRAME[static_cast<int>(ATTACK_TYPE::MAX)][2] =
	{
		// 攻撃判定のフレーム数（開始フレーム, 終了フレーム）
		{ 0, 0 },	// NONE
		{ 25, 45 },	// NORMAL1
		{ 18, 32 },	// NORMAL2
		{ 45, 72 },	// NORMAL3
		{ 42, 110 },	// NORMAL4
		{ 48, 68 },	// NORMAL5
		{ 88, 89 },	// HEAVY
		//{ 25, 0 },  // DASH
		//{ 8, 0 },   // AIR1
		//{ 10, 0 },  // AIR2
		//{ 12, 0 },  // AIR3
		//{ 15, 0 },  // AIR4
		//{ 20, 0 },  // AIR5
		//{ 25, 0 }   // FALL
	};

	// 攻撃のローカル座標オフセット（x:左右, y:高さ, z:前後）
	static constexpr VECTOR ATTACK_LOCAL_POS = { 10.0f, 100.0f, 70.0f };
	
	// 攻撃の
	static constexpr float ATTACK_RADIUS = 80.0f;

	// アニメーション終了後のコンボ受付時間（フレーム数）
	static constexpr int COMBO_WINDOW_FRAME = 60;

	// 次の攻撃タイプを取得（外部から呼ばれる）
	ATTACK_TYPE GetNextAttackType(Player* player);

private:
	// 攻撃の種類
	ATTACK_TYPE attackType_;

	// 攻撃位置を計算（プレイヤーの向きを考慮）
	VECTOR CalculateAttackPosition(Player* player);

	// 攻撃種ごとの処理
	void UpdateAttack(Player* player);

	bool isAnimationSkipped_ = false; // アニメーションをスキップするかどうか
};
