#pragma once
#include "../../../Actor/ActorBase.h"

class ShotBase: public ActorBase
{
public:
	// 弾の種類
	enum class TYPE
	{
		STRAIGHT,
		ROCK
	};
	// コンストラクタ(弾種別、元となるモデルのハンドルID)
	ShotBase(TYPE type, int baseModelId);
	// デストラクタ
	virtual ~ShotBase(void);
	// 弾の生成(表示開始座標、弾の進行方向)
	virtual void CreateShot(VECTOR pos, VECTOR dir);
	//更新
	virtual void Update(void) override;
	//描画
	virtual void Draw(void) override;
	//解放
	virtual void Release(void) override;
	// 生存判定
	bool IsAlive(void);
	// 弾の座標
	VECTOR GetPos(void);
	// 弾の衝突判定用半径
	float GetCollisionRadius(void);
	// 弾の種別
	TYPE GetType(void);
	// 弾の衝突判定用半径
	float GetcollisionRadius_(void);

protected:

	// モデルの大きさ
	static constexpr float SCALE = 0.3f;
	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	void InitLoad(void) override {}
	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {}
	// 衝突判定の初期化
	void InitCollider(void) override {}
	// アニメーションの初期化
	void InitAnimation(void) override {}
	// 初期化後の個別処理
	void InitPost(void) override {}

	// 弾の種別
	TYPE type_;
	// 弾のモデルID
	int modelId_;
	// 方向
	VECTOR dir_;
	// 弾の大きさ
	VECTOR scales_;
	// 弾の座標
	VECTOR pos_;
	// 弾の移動速度
	float speed_;
	// 弾の生存判定
	bool isAlive_;
	// 弾の生存期間
	int cntAlive_;
	// 弾の衝突判定用半径
	float collisionRadius_;


	// パラメータ設定
	virtual void SetParam(void) = 0;
	// 弾の生存期間の減少
	void ReduceCntAlive(void);


};



