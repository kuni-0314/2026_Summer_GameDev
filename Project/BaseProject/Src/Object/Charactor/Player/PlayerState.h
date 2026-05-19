#pragma once
class Player;

class PlayerState
{
public:
	virtual void Enter(Player* player) {};
	virtual void Update(Player* player) = 0;
	virtual void CollisionReserve(Player* player) {};
	virtual void Draw(Player* player) {};
	virtual void Exit(Player* player) {};

protected:
	// 遷移チェック
	bool CheckTransitions(Player* player);
};
