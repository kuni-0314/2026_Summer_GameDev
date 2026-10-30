#pragma once
#include <map>
#include <functional>
#include <DxLib.h>
#include "../CharactorBase.h"

class Player;
class ShotBase;
class ShotStraight;
class AudioManager;

class EnemyBase : public CharactorBase
{
public:

	// エネミー種別
	enum class TYPE
	{
		RAT,
		RASE,
		LARGE,
		DRAGON
	};

	// 生成用エネミーデータ
	struct EnemyData
	{
		int id;
		TYPE type;
		int hp;
		VECTOR defaultPos;
		float movableRange;
		int wave;
	};

	//JSON用エネミー読み込みデータ
	struct EnemyStatus
	{
		TYPE type;
		int hp;
	};
	//JSON用WAVE読み込みデータ
	struct EnemyWave
	{
		std::vector<int>enemies;
		int wave;
	};

	// コンストラクタ
	EnemyBase(const EnemyBase::EnemyData& data,
		int attackModel,
		Player* player);

	// デストラクタ
	virtual ~EnemyBase() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() override;

	// プレイヤーの剣との衝突判定
	void CheckPlayerSwordCollision();
	//　プレイヤーの魔法攻撃の衝突判定
	void CheckPlayerMagicCollision();

	void Release(void) override;

	//HITエフェクト
	void HitEffect(const VECTOR& pos, const VECTOR& normal, float size);
	//サンダーエフェクト
	void HitThunderEffect(const VECTOR& pos, const VECTOR& normal, float size);

	// 球同士の押し出し（Aだけを押し出す）
	static bool PushOutSphere(
		VECTOR& posA,
		float radiusA,
		const VECTOR& posB,
		float radiusB,
		bool debugDraw = false);

	//半径呼び出し
	float GetCollRadius();
	//座標呼び出し
	VECTOR& GetPos();

protected:

	// プレイヤー
	Player* player_;
	// 種別
	TYPE type_;

#pragma region 定数

	//最大ランダム値
	static constexpr int CHANGESTATE_RAND_MAX = 100;
	//デフォルトアニメ-ションスピード
	static constexpr float DEFAULT_ANIM_SPEED = 20.0f;

#pragma endregion

#pragma region 変数

	// 初期位置
	const VECTOR defaultPos_;
	// 移動範囲
	float movableRange_;
	//プレイヤーコライダー半径
	float playerRad_;
	//押し出し距離
	float pushOutRadius_;

	//攻撃用モデル(魔法の弾とか）
	int attackModle_;
	//ダメージ受ける前のHP保存用
	int preHp_;
	// 状態管理
	int stateBase_;
	//敵の攻撃力
	int power_;
	//視認判定
	bool look_;
	//生存判定
	bool isAlive_ = true;

	//wave
	int wave_;

	// 剣の攻撃判定
	bool wasHitSword_ = false;
	//　魔法の攻撃判定
	bool wasHitMagic_ = false;

#pragma endregion
	
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

private:

#pragma region 定数


	//サンダー生存時間
	static constexpr int EFFECT_THUNDER_LIFE_TIME = 30;
	//サンダーエフェクトサイズ
	static constexpr float EFFECT_THUNDER_SCALE = 30.0f;


	//追加分の最小距離
	static constexpr float ADD_MIN_DISTANCE = 20.0f;
	//ヒットエフェクトサイズ
	static constexpr float HIT_EFFECT_SCALE = 2.0f;
	// ヒット位置(敵と剣の中点)の割合
	static constexpr float HIT_POS_MID_RATE = 0.5f; 


#pragma endregion



};

