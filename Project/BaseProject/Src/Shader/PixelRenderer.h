#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"

class PixelMaterial;

class PixelRenderer
{
public:
	// コンストラクタ
	PixelRenderer(PixelMaterial& pixelMaterial);

	/// <summary>
	/// 描画矩形の生成
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="size">サイズ</param>
	void MakeSquareVertex(Vector2 pos, Vector2 size);

	/// <summary>
	/// フルスクリーン用の描画矩形の生成
	/// </summary>
	void MakeSquareVertex();

	/// <summary>
	/// 描画（指定座標）
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	void Draw(int x, int y);

	/// <summary>
	/// 描画（頂点位置そのまま）
	/// </summary>
	void Draw();

private:
	// 生成
	void Make(Vector2 pos, Vector2 size);

	// 頂点数
	static constexpr int NUM_VERTEX = 4;
	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;
	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;
	// 座標
	Vector2 pos_;
	// 描画サイズ
	Vector2 size_;
	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];
	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];
	// ピクセルマテリアル
	PixelMaterial& pixelMaterial_;
};

