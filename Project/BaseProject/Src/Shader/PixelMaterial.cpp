#include <DxLib.h>
#include "../Application.h"
#include "PixelMaterial.h"

PixelMaterial::PixelMaterial(std::string shaderFileName, int constBufFloat4Size)
	: constBufFloat4Size_(constBufFloat4Size)
{
	// ピクセルシェーダのロード
	shader_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileName).c_str());

	// ピクセルシェーダー用の定数バッファを作成
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);
}

PixelMaterial::~PixelMaterial()
{
	// シェーダーの削除
	DeleteShader(shader_);
	// 定数バッファの削除
	DeleteShaderConstantBuffer(constBuf_);
}

void PixelMaterial::Begin(void)
{
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// シェーダー設定
	SetUsePixelShader(shader_);
}

void PixelMaterial::End(void)
{
	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
}

void PixelMaterial::SetTexture(int slot, int textureHandle)
{
	SetUseTextureToShader(slot, textureHandle);
}

FLOAT4* PixelMaterial::GetConstantBuffer(void)
{
	return (FLOAT4*)GetBufferShaderConstantBuffer(constBuf_);
}

void PixelMaterial::UpdateConstantBuffer(int slotPS)
{
	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf_);

	// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(constBuf_, DX_SHADERTYPE_PIXEL, slotPS);
}
