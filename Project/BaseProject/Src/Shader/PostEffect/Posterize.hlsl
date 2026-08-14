#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ: ポスタリゼーションエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_levels; // x: 色段階数（階調レベル数）
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
    
    // --- 色を段階的に量子化 ---
    // アルゴリズム:
    // 1. 元の色（0.0～1.0）にレベル数を掛ける（例: レベル8なら 0.0～8.0の範囲）
    // 2. floor関数で整数化（例: 5.7 → 5）
    // 3. レベル数で割って元の範囲に戻す（例: 5/8 = 0.625）
    // 結果: 連続的な色が離散的な段階に変換される
    float4 dstCol;
    dstCol.rgb = floor(srcCol.rgb * g_levels.x) / g_levels.x;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}