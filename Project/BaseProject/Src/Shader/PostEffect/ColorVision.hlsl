#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: モード
cbuffer cbColorVision : register(b4)
{
    float4 g_Param; // x: モード
};

static const float3x3 g_ColorVisionMatrix[8] =
{
    // Normal
    float3x3(
        1.000f, 0.000f, 0.000f,
        0.000f, 1.000f, 0.000f,
        0.000f, 0.000f, 1.000f
    ),

    // Protanomaly
    float3x3(
        0.817f, 0.183f, 0.000f,
        0.333f, 0.667f, 0.000f,
        0.000f, 0.125f, 0.875f
    ),

    // Deuteranomaly
    float3x3(
        0.800f, 0.200f, 0.000f,
        0.258f, 0.742f, 0.000f,
        0.000f, 0.142f, 0.858f
    ),

    // Tritanomaly
    float3x3(
        0.967f, 0.033f, 0.000f,
        0.000f, 0.733f, 0.267f,
        0.000f, 0.183f, 0.817f
    ),

    // Protanopia
    float3x3(
        0.567f, 0.433f, 0.000f,
        0.558f, 0.442f, 0.000f,
        0.000f, 0.242f, 0.758f
    ),

    // Deuteranopia
    float3x3(
        0.625f, 0.375f, 0.000f,
        0.700f, 0.300f, 0.000f,
        0.000f, 0.300f, 0.700f
    ),

    // Tritanopia
    float3x3(
        0.950f, 0.050f, 0.000f,
        0.000f, 0.433f, 0.567f,
        0.000f, 0.475f, 0.525f
    ),

    // Monochromacy
    float3x3(
        0.299f, 0.587f, 0.114f,
        0.299f, 0.587f, 0.114f,
        0.299f, 0.587f, 0.114f
    )
};
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //return float4(g_Param.x / 3.0f, 0, 0, 1);
    
    // テクスチャカラー取得
    float4 color = tex.Sample(texSampler, PSInput.uv);

    // 頂点カラーを反映する場合
    color *= PSInput.diffuse;

    // 色覚変換
    float mode = g_Param.x;
    color.rgb = mul(g_ColorVisionMatrix[(int) mode], color.rgb);
    //color.rgb = mul(g_ColorVisionMatrix[g_Mode], color.rgb);

    // 0～1に収める
    color.rgb = saturate(color.rgb);

    return color;
}