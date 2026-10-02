#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include <vector>
#include "../EnemyBase.h"
#include "../../../../Effect/LoadEffekseer/EffekseerEffect.h"


class GameScene;
class Player;
class ItemManger;
class ColliderCapsule;
class ColliderSphere;


class EnemyDragon : public EnemyBase
{

public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		FRY_IDLE,
		BREARH,
		TORNADO,
		LANDING,
		CLOW,
		DIE,
	};
	
	// 状態
	enum class STATE
	{
		NONE,
		THINK,
		IDLE,
		FLY_IDLE,
		BREARH,
		TORNADO,
		LANDING,
		MOVE,
		CLOW
	};

	EnemyDragon(const EnemyBase::EnemyData& data, int attackModel, Player* player);
	// デストラクタ
	~EnemyDragon() override;

	void Draw(void) override;
	void Release(void) override;

protected:

	// リソースロード
	void InitLoad() override;
	// 大きさ、回転、座標の初期化
	void InitTransform() override;
	// 衝突判定の初期化
	void InitCollider() override;
	// アニメーションの初期化
	void InitAnimation() override;
	// 初期化後の個別処理
	void InitPost() override;
	// 更新系
	void UpdateProcess() override;
	void UpdateProcessPost() override;

	//風圧エフェクト
	void LandingEffect(const VECTOR& pos, const VECTOR& normal, float size);

private:

#pragma region 判定系定数

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -30.0f, 0.0f };

	//ドラゴン当たり判定用
	static constexpr VECTOR COLBODY_CAPSULE_TOP_LOCAL_POS = { 0.0f, 300.0f, 250.0f };
	static constexpr VECTOR COLBODY_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 230.0f, -250.0f };

	//ブレス用カプセルコライダーサイズ(カプセルがステージに当たると座標移動する)
	static constexpr VECTOR CAPSULE_ADD_BREATH_POS = { 0.0f, 0.0f, 50.0f };		//ブレス開始位置調整
	static constexpr VECTOR CAPSULE_DOWN_BREATH_POS = { 0,-110,700 };	        //ブレス終了位置（カプセルの終わり）

	//トルネード用カプセルコライダーサイズ
	static constexpr VECTOR CAPSULE_TOP_TORUNADO_POS = { 0.0f, 200.0f, 0.0f };
	static constexpr VECTOR CAPSULE_DOWN_TORUNADO_POS = { 0.0f, -40.0f, 0.0f };
#pragma endregion



	//トルネード生成位置
	static constexpr VECTOR TORNADO_CREATE_START_POS = { 0,0,0 };
	//トルネード最大移動距離（生存切り替え距離)
	static constexpr float MAX_DIST = 2000.0f;

	static constexpr float PUSH_MIN_SEPARATION = 300.0f; // 最小許容距離（水平）
	static constexpr float PUSH_PLAYER_RATIO = 0.60f;    // 重なりを何割プレイヤーへ押すか
	static constexpr float PUSH_ENEMY_RATIO = 0.40f;     // 何割ドラゴンへ押すか（合計1.0になるように）

	//最大上昇量
	static constexpr float ANIM_SPEED = 20.0f;

	//上昇量
	static constexpr float FLY_POW = 0.5f;
	//最大上昇量
	static constexpr float FLY_POW_MAX = 500.0f;

	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };
	// モデルの大きさ
	static constexpr float SCALE = 120.0f;

	//ブレスの判定開始フレーム時間
	static constexpr float ATTACK_FREAM_BREATH_TIME = 0.20f;
	//トルネードクールタイム時間
	static constexpr int TORNADO_COOL_TIME_MAX = 200;


	//待機状態切替時間
	static constexpr int IDLE_CHANGE_TIME = 60;

	//待機状態切替時間
	static constexpr int BREATH_FRAME_INDEX = 16;

	//クローの判定開始フレーム　
	static constexpr float ATTACK_FRAME_CLOW_TIME = 0.14f;
	// 生成後に何フレーム待つか（調整可）
	static constexpr int COLLIDER_ACTIVE_DELAY = 6; 

	//確率最大値
	static constexpr int RAND_MAX_POW = 100;
	//飛行状態切り替え値
	static constexpr int CHANGE_FLY_POW = 50;
	//トルネード状態切り替え値
	static constexpr int CHANGE_TORUNADO_POW = 30;

	//上昇時の最大
	static constexpr float FLY_POS_Y_MAX = 500.0f;

	//降下量
	static constexpr float LANDING_POW = 5.0f;
	//降下状態切替座標
	static constexpr float LANDING_CHANGE_POS_Y = -40.0f;


	//ゲームシーン
	GameScene* gamescene_;
	// 状態
	STATE state_;

	// 更新ステップ
	float step_;// 状態管理(更新ステップ)
	//空中上昇量
	float pow = 10;

	// 待機時間カウント
	int idleTime_;
	//トルネードクールタイム(カウント)
	int tornadoCoolTime_;

	//頭のローカル回転
	Quaternion headRot_;

	//空中上昇量
	float pow = 10;

	//ブレス情報関連
	struct BreathInfo
	{
		Transform transform = {};				//座標等の情報
		ColliderCapsule* collider = nullptr;	//コライダー
		std::shared_ptr<EffekseerEffect> effect;

		bool wasHitPlayer = false;
	};

	//ブレス情報関連
	BreathInfo breathInfo_;
	//ブレス生存状態
	bool isAliveBreath_;					

	//ブレス開始位置
	VECTOR breathTopPos_;
	//ブレス終了位置
	VECTOR breathDownPos_;
	//胴体座標
	VECTOR bodyPos_;

	//ブレス攻撃
	//ブレスコライダー生成
	void CreateBreathCollider(BreathInfo& breathInfo);
	//ブレス削除
	void DestoryBreathCollider(BreathInfo& breathInfo);
	//ブレス生成
	void CreateBreath();
	
	//トルネード情報関連
	struct TornadoInfo
	{
		Transform transform = {};				//座標等の情報
		ColliderCapsule* collider = nullptr;	//コライダー
		float dist;
		std::shared_ptr<EffekseerEffect> effect;
		VECTOR startPos;
		VECTOR moveDir;
		float speed;
		bool isDestory = false;
		bool wasHitPlayer = false;
	};

	//トルネード生成カウント
	static constexpr int tornadoCount_ = 2;

	//トルネード情報
	TornadoInfo tornadoInfo_[tornadoCount_];

	//トルネード生存状態
	bool isAliveTornado_;
	//降下状態判定
	bool landing_ ;

	bool isTakeOffEffect_ = false;

	//トルネードコライダー生成
	void CreateTornadoCollider(TornadoInfo& tornadoInfo);
	//トルネードコライダー削除
	void DestoryTornadoCollider(TornadoInfo& tornadoInfo);
	//トルネード生成
	void CreateTornado();

	//トルネード情報関連
	struct ClowInfo
	{
		Transform transform = {};				//座標等の情報
		ColliderSphere* collider = nullptr;	//コライダー
		std::shared_ptr<EffekseerEffect> effect;

		bool wasHitPlayer = false;
		bool colliderRegistered = false;
		int colliderActiveDelay = 0;
	};

	//クロー情報
	ClowInfo clowInfo_;
	//クロー生存状態
	bool isAliveClow_;

	//クローコライダー作成・削除
	void CreateClowCollider(ClowInfo& clowInfo);
	void DestroyClowColier(ClowInfo& clowInfo);
	//クロー生成
	void CreateClow();

	// 状態遷移
	void ChangeState(STATE state);

	//切り替え準備
	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateFlayIdle();
	void ChangeStateBreath();
	void ChangeStateTornado();
	void ChangeStateLanding();
	void ChangeStateClow();

	//状態更新
	void UpdateThink();
	void UpdateIdle();
	void UpdateFlayIdle();
	void UpdateBreath();
	void UpdateTornado();
	void UpdateLanding();
	void UpdateClow();

	//押し出し処理
	void ResolvePushWithPlayer();
};

