#pragma once
#include "../../Collider/ColliderBase.h"
#include <vector>
#include "EnemyBase.h"

class ColliderBase;
class Player;
class GameScene;
class Stage;

class EnemyManager
{

public:
	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	EnemyManager(GameScene* gamescene, Player* player);
	// ƒfƒXƒgƒ‰ƒNƒ^
	~EnemyManager();
	// ‰Šú‰»
	void Init();
	// XV
	void Update();
	// •`‰æ
	void Draw();
	// ‰ğ•ú
	void Release();
	// ƒGƒlƒ~[
	const std::vector<EnemyBase*>& GetEnemies() const { return enemies_; }
	// Õ“Ë‘ÎÛ‚Æ‚È‚éƒRƒ‰ƒCƒ_‚ğ“o˜^
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSV‚©‚ç“Gî•ñ‚Ì“Çæ‚ğs‚¤
	void LoadCsvData();
	// ƒGƒlƒ~[¶¬
	EnemyBase* Create(const EnemyBase::EnemyData& data, const Player* player);

	// w’èÀ•W‚ÉÅ‚à‹ß‚¢ƒGƒlƒ~[‚ÌÀ•W‚ğæ“¾
	VECTOR GetNearEnemyPos(const VECTOR& pos) const;

	// w’èID‚ÌƒGƒlƒ~[‚ÌÀ•W‚ğæ“¾
	VECTOR GetEnemyPos(int id) const;

	bool GetEnemyDead();





private:
	//ƒvƒŒƒCƒ„[
	Player* player_;
	//ƒQ[ƒ€ƒV[ƒ“
	GameScene* gameScene_;

	// ƒGƒlƒ~[
	std::vector<EnemyBase*> enemies_;

	const ColliderBase* hitCollider_;

<<<<<<< HEAD
=======
	// UŒ‚ƒRƒ‰ƒCƒ_‚ÌƒŠƒXƒg
	std::vector<const ColliderBase*> attackColliders_;

	// UŒ‚ƒGƒtƒFƒNƒg—p‚Ìƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹ID
	int attackModel_;

>>>>>>> origin/Î±ä¿®æ­£ï¼’
	bool isDead_ = false;

	// ‹–ì”ÍˆÍ—pƒgƒ‰ƒ“ƒXƒtƒH[ƒ€
	Transform attackTransform_;

};

