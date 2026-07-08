#pragma once
#include <map>
#include <functional>
#include "../CharactorBase.h"
#include <DxLib.h>
class Player;
class ShotBase;
class ShotStraight;
class AudioManager;

class EnemyBase : public CharactorBase
{
public:

	// 種別
	enum class TYPE
	{
		RAT,
		RASE,
		LARGE
	};
	// エネミーデータ
	struct EnemyData
	{
		int id;
		EnemyBase::TYPE type;
		int hp;
		VECTOR defaultPos;
		float movableRange;
		int wave;
	};

	// コンストラクタ
	EnemyBase(const EnemyBase::EnemyData& data,int attackModel,Player* player);

	// デストラクタ
	virtual ~EnemyBase() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() override;

	// プレイヤーの剣との衝突判定を行う
	void CheckPlayerSwordCollision();

	void CheckEnemy();

	void Release(void) override;

protected:

	Player* player_;
	// 種別
	TYPE type_;


	// 初期位置
	const VECTOR defaultPos_;
	float movableRange_;
	float playerRad_;

	//攻撃用モデル(魔法の弾とか）
	int attackModle_;
	//ダメージ受ける前のHP保存用
	int preHp_;
	// 状態管理
	int stateBase_;
	//敵の攻撃力
	int power_;

	//wave
	int wave_;
	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void()>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void()> stateUpdate_;

	// ダメージを受けたかどうか
	bool wasHit_ = false;

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

