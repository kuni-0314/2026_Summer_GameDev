#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: スキャンラインエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 周波数（ライン数）, y: 強度（明暗の差）
}

// 円周率の2倍（正弦波の周期計算用）
static const float TWO_PI = 6.28318530718f;
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 水平走査線パターンを生成 ---
    // sin関数で周期的な明暗パターンを生成
    // PSInput.uv.y: Y座標（0.0～1.0）
    // g_param.x: 周波数（ライン数の調整）
    // TWO_PI: 一周期分の角度
    float scanline = sin(PSInput.uv.y * g_param.x * TWO_PI);
    
    // sin関数の出力範囲（-1.0～1.0）を（0.0～1.0）に変換
    scanline = scanline * 0.5f + 0.5f;
    
    // 強度パラメータで効果の強さを調整
    // g_param.y = 0.0: 効果なし（scanline = 1.0）
    // g_param.y = 1.0: 最大効果（scanline = 元の計算値）
    scanline = lerp(1.0f, scanline, g_param.y);
    
    // 元の色にスキャンライン係数を乗算
    float4 dstCol = srcCol;
    dstCol.rgb *= scanline;
    
    return dstCol;
}