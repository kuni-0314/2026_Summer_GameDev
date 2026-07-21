#pragma once
#include <DxLib.h>
#include <functional>
#include <memory>
#include <vector>
#include "../EnemyBase.h"

class GameScene;
class Player;
class ItemManger;
class ColliderCapsule;


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
		LANDING
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

private:


	//ステージ判定用
	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -30.0f, 0.0f };

	//ドラゴン当たり判定用
	static constexpr VECTOR COLBODY_CAPSULE_TOP_LOCAL_POS = { 0.0f, 230.0f, 30.0f };
	static constexpr VECTOR COLBODY_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 230.0f, -250.0f };




	//ブレス用カプセルコライダーサイズ(カプセルがステージに当たると座標移動する)
	static constexpr VECTOR CAPSULE_ADD_BREATH_POS = { 0.0f, 0.0f, 50.0f };		//ブレス開始位置調整
	static constexpr VECTOR CAPSULE_DOWN_BREATH_POS = { 0,-110,700 };	        //ブレス終了位置（カプセルの終わり）

	//トルネード用カプセルコライダーサイズ
	static constexpr VECTOR CAPSULE_TOP_TORUNADO_POS = { 0.0f, 200.0f, 0.0f };
	static constexpr VECTOR CAPSULE_DOWN_TORUNADO_POS = { 0.0f, -40.0f, 0.0f };

	//トルネード生成位置
	static constexpr VECTOR TORNADO_CREATE_START_POS = { 0,0,0 };
	//トルネード最大移動距離（生存切り替え距離)
	static constexpr float MAX_DIST = 2000.0f;


	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };
	// モデルの大きさ
	static constexpr float SCALE = 120.0f;

	//ブレスの判定開始フレーム時間
	static constexpr float ATTACK_BREATH_TIME = 0.20f;
	static constexpr int TORNADO_RESET_TIME = 200;

	//ゲームシーン
	GameScene* gamescene_;

	// 状態
	STATE state_;
	// 更新ステップ
	float step_;// 状態管理(更新ステップ)

	int idleTime_;
	int changetime = 60;

	int tornadoCoolTime_;


	//空中上昇量
	int pow = 5;

	//ブレス情報関連
	struct BreathInfo
	{
		Transform transform = {};				//座標等の情報
		ColliderCapsule* collider = nullptr;	//コライダー
	};

	//ブレス情報関連
	BreathInfo breathInfo_;
	//ブレス生存状態
	bool isAliveBreath_;					

	//ブレス開始位置
	VECTOR breathTopPos_;
	//ブレス終了位置
	VECTOR breathDownPos_;

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
	bool landing_ ;




	//トルネードコライダー生成
	void CreateTornadoCollider(TornadoInfo& tornadoInfo);
	//トルネードコライダー削除
	void DestoryTornadoCollider(TornadoInfo& tornadoInfo);
	//トルネード生成
	void CreateTornado();



	// 状態遷移
	void ChangeState(STATE state);


	//切り替え準備
	void ChangeStateThink();
	void ChangeStateIdle();
	void ChangeStateFlayIdle();
	void ChangeStateBreath();
	void ChangeStateTornado();
	void ChangeStateLanding();

	//状態更新
	void UpdateThink();
	void UpdateIdle();
	void UpdateFlayIdle();
	void UpdateBreath();
	void UpdateTornado();
	void UpdateLanding();

	
	
};

