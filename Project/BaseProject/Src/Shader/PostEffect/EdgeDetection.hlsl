#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: エッジ検出エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: エッジの強度（検出感度）
}

// テクセルサイズ（1ピクセルのUV座標上でのサイズ）
static const float2 TEXEL_SIZE = float2(0.001f, 0.001f);
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- Sobelフィルタによるエッジ検出 ---
    // 3x3の近傍ピクセルをサンプリング
    // 配置:
    //   tl  t  tr
    //   l  (c)  r
    //   bl  b  br
    
    float4 tl = tex.Sample(texSampler, PSInput.uv + float2(-TEXEL_SIZE.x, -TEXEL_SIZE.y)); // 左上
    float4 t = tex.Sample(texSampler, PSInput.uv + float2(0.0f, -TEXEL_SIZE.y)); // 上
    float4 tr = tex.Sample(texSampler, PSInput.uv + float2(TEXEL_SIZE.x, -TEXEL_SIZE.y)); // 右上
    float4 l = tex.Sample(texSampler, PSInput.uv + float2(-TEXEL_SIZE.x, 0.0f)); // 左
    float4 r = tex.Sample(texSampler, PSInput.uv + float2(TEXEL_SIZE.x, 0.0f)); // 右
    float4 bl = tex.Sample(texSampler, PSInput.uv + float2(-TEXEL_SIZE.x, TEXEL_SIZE.y)); // 左下
    float4 b = tex.Sample(texSampler, PSInput.uv + float2(0.0f, TEXEL_SIZE.y)); // 下
    float4 br = tex.Sample(texSampler, PSInput.uv + float2(TEXEL_SIZE.x, TEXEL_SIZE.y)); // 右下
    
    // 中心ピクセル
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- Sobelオペレータの適用 ---
    // 水平方向の勾配（Gx）:
    //   -1  0  +1
    //   -2  0  +2
    //   -1  0  +1
    float4 gx = -tl - 2.0f * l - bl + tr + 2.0f * r + br;
    
    // 垂直方向の勾配（Gy）:
    //   -1 -2 -1
    //    0  0  0
    //   +1 +2 +1
    float4 gy = -tl - 2.0f * t - tr + bl + 2.0f * b + br;
    
    // 勾配の大きさを計算（マンハッタン距離近似）
    // 正確には sqrt(gx^2 + gy^2) だが、length(gx) + length(gy) で近似して高速化
    float edge = length(gx) + length(gy);
    
    // --- エッジ強度に応じて色を調整 ---
    // エッジ部分を暗く（黒く）する
    // saturate関数で0.0～1.0の範囲にクランプ
    float4 dstCol = srcCol;
    dstCol.rgb = lerp(srcCol.rgb, float3(0, 0, 0), saturate(edge * g_param.x));
    
    return dstCol;
}