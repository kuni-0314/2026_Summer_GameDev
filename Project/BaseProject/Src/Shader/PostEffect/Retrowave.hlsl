#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: レトロウェーブエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 強度（グロー効果の強さ）
}

// レトロウェーブの定数
static const float3 CYBER_PINK_TINT = float3(1.0f, 0.3f, 0.8f); // サイバーピンク/パープル色
static const float3 LUMINANCE_WEIGHTS = float3(0.299f, 0.587f, 0.114f); // ITU-R BT.601 輝度係数
//static const float GLOW_POWER = 4.0f; // グロー効果の指数（大きいほど明るい部分が強調される）
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- サイバーパンクカラーグレーディング ---
    // 80年代のシンセウェーブ/レトロウェーブの特徴的なピンク/パープル色調
    
    // 輝度を計算（人間の視覚特性に基づく重み付け）
    float luminance = dot(srcCol.rgb, LUMINANCE_WEIGHTS);
    
    // 明るい部分を強調してグロー効果を生成
    // pow関数で非線形に輝度を増幅（明るい部分だけが大きく増幅される）
    // サイバーピンク色を掛け合わせて特徴的な色調に
    //float3 glow = pow(luminance, GLOW_POWER) * CYBER_PINK_TINT;
    float l2 = luminance * luminance;
    float glowIntensity = l2 * l2;
    float3 glow = glowIntensity * CYBER_PINK_TINT;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    // 元の色にグロー効果を加算合成
    // g_param.x で効果の強度を調整
    dstCol.rgb = lerp(srcCol.rgb, srcCol.rgb + glow, g_param.x);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}