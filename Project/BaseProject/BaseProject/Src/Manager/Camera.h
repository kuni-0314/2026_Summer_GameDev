#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Actor/ActorBase.h"
class Transform;
class Stage;

class Camera : public ActorBase
{

public:

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		SPHERE,
		MAX,
	};


	// カメラの初期座標
	static constexpr VECTOR DERFAULT_POS = { 0.0f, 200.0f, -500.0f };
	
	// カメラの初期角度
	static constexpr VECTOR DERFAULT_ANGLES = { 
		0.0f, 0.0f, 0.0f
	};

	// カメラの回転量
	const float ROT_POW_DEG = 2.0f;
	const float ROT_POW_RAD = ROT_POW_DEG * DX_PI_F / 180.0f;

	// カメラの移動スピード
	static constexpr float SPEED = 20.0f;

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 10.0f;
	static constexpr float VIEW_FAR = 20000.0f;

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 50.0f, -400.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 500.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 70.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 30.0f * (DX_PI_F / 180.0f);;

	// MOUSEモード用定数
	static constexpr VECTOR MOUSE_CAMERA_LOCAL_POS = { 0.0f, 200.0f, -300.0f };

	// TARGETINGモード用定数
	static constexpr VECTOR TARGETING_CAMERA_LOCAL_POS = { 150.0f, 200.0f, -300.0f };
	
	// イージング定数
	static constexpr float EASING_RATE_POS = 0.2f;			// 位置のイージング速度
	static constexpr float EASING_RATE_TARGET = 0.15f;		// 注視点のイージング速度
	static constexpr float EASING_RATE_ANGLE = 0.08f;		// 角度のイージング速度

	// ズーム関連の定数
	static constexpr float ZOOM_SPEED = 20.0f;				// ズーム速度
	static constexpr float ZOOM_MIN = 0.5f;					// ズームの最小値（最も近い）
	static constexpr float ZOOM_MAX = 2.0f;					// ズームの最大値（最も遠い）
	static constexpr float ZOOM_DEFAULT = 1.0f;				// ズームのデフォルト値
	static constexpr float ZOOM_EASING_RATE = 0.15f;		// ズームのイージング速度

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FREE,
		FOLLOW,
		MANUAL,
		TARGETING,
		OPENING,
	};

	// コンストラクタ
	Camera();

	// デストラクタ
	~Camera()override;


	// 更新
	void Update()override;

	// 描画前のカメラ設定
	void SetBeforeDraw();

	// デバッグ用描画
	void DrawDebug();

	// 解放
	void Release()override;

	// 座標の取得
	const VECTOR& GetPos() const;

	// 角度の取得
	const VECTOR& GetAngles() const;
	const Quaternion& GetQuaRot() const;
	
	// X回転を抜いたY軸のみのカメラ角度
	const Quaternion& GetQuaRotY() const;
	
	// 注視点の取得
	const VECTOR& GetTargetPos() const;

	// カメラの前方方向
	VECTOR GetForward() const;

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// 注視点の設定
	void SetTargetPos(const VECTOR& pos);

	VECTOR debugTargetPos_;

protected:

	// リソースロード
	void InitLoad() override {}
	// 大きさ、回転、座標の初期化
	void InitTransform() override {}
	// 衝突判定の初期化
	void InitCollider() override;
	// アニメーションの初期化
	void InitAnimation() override {}
	// 初期化後の個別処理
	void InitPost() override;
	
	

private:

	Stage* stage_;

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION_CAMERA = 30;

	// 衝突時の押し戻し量
	static constexpr float COLLISION_BACK_DIS = 2.0f;

	// 衝突判定用球体半径
	static constexpr float COL_CAPSULE_SPHERE = 50.0f;


	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;
	// カメラ角度(rad)
	VECTOR angles_;
	// カメラ角度(Y軸のみ)
	Quaternion rotY_;
	// 注視点
	VECTOR targetPos_;

	// イージング用の目標値
	VECTOR targetPosGoal_;		// 注視点の目標位置
	VECTOR posGoal_;			// カメラ位置の目標
	VECTOR anglesGoal_;			// カメラ角度の目標

	// ズーム関連
	float zoomScale_;			// 現在のズーム倍率
	float zoomScaleGoal_;		// 目標のズーム倍率

	// オープニング演出用
	float openingTimer_;		// オープニング演出のタイマー
	// カメラをデフォルト位置に戻す
	void SetDefault();

	// 追従対象との位置を同期
	void SyncFollow();

	// カメラ操作
	void ProcessRot(bool isLimit);
	void ProcessMove();
	void ProcessZoom();	// ズーム処理

	// カメラ回転(キーボード)
	void RotKeyboard(bool isLimit);

	// カメラ回転(ゲームパッド)
	void RotGamePad(bool isLimit);

	// カメラ回転(マウス)
	void RotMouse(bool isLimit);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint();
	void SetBeforeDrawFree();
	void SetBeforeDrawFollow();
	void SetBeforeDrawManual();
	void SetBeforeDrawTargeting();
	void SetBeforeDrawOpening();

	// 衝突判定
	void Collision();

	// イージング処理
	VECTOR LerpVector(const VECTOR& current, const VECTOR& target, float rate);
	float LerpAngle(float current, float target, float rate);

	static constexpr float OPENING_TIME = 2.0f;          // 演出時間(秒)
	static constexpr float OPENING_START_DISTANCE = 10.0f;
	static constexpr float OPENING_END_DISTANCE = 250.0f;
	static constexpr float OPENING_HEIGHT = 10.0f;       // 顔から少し上を見る
};
