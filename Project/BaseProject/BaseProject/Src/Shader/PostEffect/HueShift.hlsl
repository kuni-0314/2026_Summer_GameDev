#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 色相シフトエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 色相シフト量（0～2π、360度回転）
}

// HSVカラースペース変換の定数
static const float4 RGB_TO_HSV_K = float4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
static const float4 HSV_TO_RGB_K = float4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
static const float EPSILON = 1.0e-10f; // ゼロ除算回避用の微小値
static const float TWO_PI = 6.28318530718f; // 2π
 
// RGB色空間からHSV色空間への変換
// 戻り値: float3(色相[0-1], 彩度[0-1], 明度[0-1])
float3 rgb2hsv(float3 c)
{
    // RGBの最大値と最小値を見つけるためのアルゴリズム
    float4 p = lerp(float4(c.bg, RGB_TO_HSV_K.wz), float4(c.gb, RGB_TO_HSV_K.xy), step(c.b, c.g));
    float4 q = lerp(float4(p.xyw, c.r), float4(c.r, p.yzx), step(p.x, c.r));
    
    // 最大値と最小値の差（彩度の計算に使用）
    float d = q.x - min(q.w, q.y);
    
    // HSV値を計算
    return float3(
        abs(q.z + (q.w - q.y) / (6.0f * d + EPSILON)), // 色相（Hue）
        d / (q.x + EPSILON), // 彩度（Saturation）
        q.x                                              // 明度（Value）
    );
}
 
// HSV色空間からRGB色空間への変換
// c: float3(色相[0-1], 彩度[0-1], 明度[0-1])
float3 hsv2rgb(float3 c)
{
    // 色相から基本的なRGB値を計算
    float3 p = abs(frac(c.xxx + HSV_TO_RGB_K.xyz) * 6.0f - HSV_TO_RGB_K.www);
    
    // 彩度と明度を適用
    return c.z * lerp(HSV_TO_RGB_K.xxx, saturate(p - HSV_TO_RGB_K.xxx), c.y);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- RGB から HSV へ変換 ---
    float3 hsv = rgb2hsv(srcCol.rgb);
    
    // --- 色相をシフト ---
    // g_param.x（0～2π）を正規化範囲（0～1）に変換して加算
    // frac関数で0～1の範囲に循環させる（色相環を1周）
    hsv.x = frac(hsv.x + g_param.x / TWO_PI);
    
    // --- HSV から RGB へ変換 ---
    float3 rgb = hsv2rgb(hsv);
    
    // アルファ値は元のまま保持
    return float4(rgb, srcCol.a);
}