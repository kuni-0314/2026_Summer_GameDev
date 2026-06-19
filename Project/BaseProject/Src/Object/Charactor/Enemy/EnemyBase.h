#pragma once
#include <functional>
#include "../CharactorBase.h"
#include <DxLib.h>
class Player;

class EnemyBase : public CharactorBase
{
public:



	// 種別
	enum class TYPE
	{
		RAT,
		RASE,
	};
	// エネミーデータ
	struct EnemyData
	{
		int id;
		EnemyBase::TYPE type;
		int hp;
		VECTOR defaultPos;
		float movableRange;
		int power;
	};

	// コンストラクタ
	EnemyBase(const EnemyBase::EnemyData& data,Player* player);
	// デストラクタ
	virtual ~EnemyBase() override;
	

	//描画
	virtual void Draw() override;

	// プレイヤーの剣との衝突判定を行う
	void CheckPlayerSwordCollision();

protected:

	Player* player_;
	// 種別
	TYPE type_;
	// 初期位置
	const VECTOR defaultPos_;
	float movableRange_;
	int power_;

	// 状態管理
	int stateBase_;
	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void()>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void()> stateUpdate_;

	// リソースロード
	void InitLoad() override {}
	// 大きさ、回転、座標の初期化
	void InitTransform() override {}
	// 衝突判定の初期化
	void InitCollider() override {}
	// アニメーションの初期化
	void InitAnimation() override {}
	// 初期化後の個別処理
	void InitPost() override {}


	// 状態遷移
	void ChangeState(int state);


	// 更新系
	virtual void UpdateProcessPost() override {}
	// 移動可能範囲判定
	bool InMovableRange() const;

	//視線ベクトル
	void LookPlayer();

};

