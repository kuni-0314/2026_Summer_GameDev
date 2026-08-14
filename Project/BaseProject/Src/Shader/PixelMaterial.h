#pragma once
#include <vector>
#include <string>
#include <DxLib.h>

class PixelMaterial
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName">シェーダーファイル名</param>
	/// <param name="constBufFloat4Size">定数バッファのFLOAT4サイズ</param>
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PixelMaterial();

	/// <summary>
	/// シェーダーの適用開始
	/// </summary>
	void Begin();

	/// <summary>
	/// シェーダーの適用終了
	/// </summary>
	void End();

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="slot">スロット番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetTexture(int slot, int textureHandle);

	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns>定数バッファのポインタ</returns>
	FLOAT4* GetConstantBuffer();

	/// <summary>
	/// 定数バッファの更新と適用
	/// </summary>
	/// <param name="slotPS">ピクセルシェーダー用スロット番号</param>
	void UpdateConstantBuffer(int slotPS);

	// シェーダハンドル
	int shader_;
	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;
	// 定数バッファハンドル
	int constBuf_;
};

