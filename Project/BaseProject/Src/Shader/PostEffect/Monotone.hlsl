#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 人間の視覚に基づいた輝度係数（ITU-R BT.601標準）
// 人間の目は緑に最も敏感で、青に最も鈍感
static const float3 LUMINANCE_WEIGHTS = float3(0.299f, 0.587f, 0.114f);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- グレースケール変換 ---
    // 方法1: 単純平均（高速だが視覚的に不正確）
    float gray = (srcCol.r + srcCol.g + srcCol.b) * 0.333333f;
    
    // 方法2: 輝度係数を使った変換（推奨）
    // 人間の視覚特性に基づいた重み付け平均
    // ドット積を使用して効率的に計算
    //float gray = dot(srcCol.rgb, LUMINANCE_WEIGHTS);
    
    // グレースケール値をRGB全チャンネルに設定
    float4 dstCol;
    dstCol.rgb = float3(gray, gray, gray);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}