#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>
class Transform
{

public:

	// モデルのハンドルID
	int modelId;

	// 大きさ
	VECTOR scl;
	// 回転
	VECTOR rot;
	// 位置
	VECTOR pos;
	VECTOR localPos;
	VECTOR prevPos;

	// 行列
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;

	// コンストラクタ
	Transform();

	// デストラクタ
	~Transform();

	// モデル制御の基本情報更新
	void Update();
	
	// 解放
	void Release();

	// モデルのハンドルIDを設定
	void SetModel(int modelHId);

	// 前方方向を取得
	VECTOR GetForward() const;

	// 後方方向を取得
	VECTOR GetBack() const;

	// 右方向を取得
	VECTOR GetRight() const;

	// 左方向を取得
	VECTOR GetLeft() const;

	// 上方向を取得
	VECTOR GetUp() const;

	// 下方向を取得
	VECTOR GetDown() const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& dir) const;

};
