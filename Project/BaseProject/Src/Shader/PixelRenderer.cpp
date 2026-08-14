#include <DxLib.h>
#include "../Application.h"
#include "PixelMaterial.h"
#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial)
	: pixelMaterial_(pixelMaterial)
{
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexs_[i] = {};
	}
	for (int i = 0; i < NUM_VERTEX_IDX; i++)
	{
		indexes_[i] = 0;
	}
}

void PixelRenderer::MakeSquareVertex(Vector2 pos, Vector2 size)
{
	Make(pos, size);
}

void PixelRenderer::MakeSquareVertex()
{
	Vector2 pos = Vector2(0, 0);
	Vector2 size = Vector2(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	Make(pos, size);
}

void PixelRenderer::Draw(int x, int y)
{
	// 頂点位置を一時的にオフセット
	VERTEX2DSHADER tempVertexs[NUM_VERTEX];
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		tempVertexs[i] = vertexs_[i];
		tempVertexs[i].pos.x += x;
		tempVertexs[i].pos.y += y;
	}

	// 描画
	DrawPolygonIndexed2DToShader(tempVertexs, NUM_VERTEX, indexes_, NUM_POLYGON);
}

void PixelRenderer::Draw()
{
	// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);
}

void PixelRenderer::Make(Vector2 pos, Vector2 size)
{
	// 位置とサイズを保存
	pos_ = pos;
	size_ = size;

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	// ４頂点の初期化
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// 左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;
}
