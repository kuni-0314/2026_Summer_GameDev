#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 波紋エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 強度（振幅）, z: 波の数（周波数）
}

// 定数
static const float2 CENTER = float2(0.5f, 0.5f); // 波紋の中心点
static const float TWO_PI = 6.28318530718f; // 円周率の2倍
static const float OFFSET_SCALE = 0.01f; // オフセット量のスケール
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- 中心からの方向と距離を計算 ---
    float2 toCenter = PSInput.uv - CENTER;
    float dist = length(toCenter);
    
    // --- 波紋の計算 ---
    // sin関数で同心円状の波を生成
    // (dist - g_param.x * 0.5f): 時間経過で波が外側に広がる効果
    // g_param.z: 波の密度（周波数）
    float ripple = sin((dist - g_param.x * 0.5f) * g_param.z * TWO_PI);
    
    // --- UV座標を波紋に沿って歪ませる ---
    // 中心からの方向ベクトルを正規化（長さ1に）
    // 波紋の強さ（ripple）に応じてオフセットを計算
    float2 offset = normalize(toCenter) * ripple * g_param.y * OFFSET_SCALE;
    
    // 歪んだUV座標でテクスチャをサンプリング
    float2 uv = PSInput.uv + offset;
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}