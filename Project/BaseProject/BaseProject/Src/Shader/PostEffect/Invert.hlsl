#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// ネガポジ反転エフェクト
// RGB各チャンネルを反転させる（1.0 - 元の値）
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 色を反転 ---
    // RGB各チャンネルを1.0から減算することで反転
    // 白(1.0, 1.0, 1.0) → 黒(0.0, 0.0, 0.0)
    // 黒(0.0, 0.0, 0.0) → 白(1.0, 1.0, 1.0)
    // 赤(1.0, 0.0, 0.0) → シアン(0.0, 1.0, 1.0)
    float4 dstCol;
    dstCol.rgb = 1.0f - srcCol.rgb;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}