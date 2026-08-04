#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 画面振動エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param1; // x: 時間, y: 振幅X（横揺れの強さ）, z: 振幅Y（縦揺れの強さ）, w: 周波数（揺れの速さ）
    float4 g_param2; // x: Y方向のランダムシードオフセット
}

// 疑似乱数生成の定数
static const float RANDOM_SEED = 43758.5453123f;
 
// 疑似乱数生成関数（0.0～1.0の範囲）
float random(float seed)
{
    // シンプルなハッシュ関数ベースの乱数生成
    return frac(sin(seed) * RANDOM_SEED);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- ランダムな揺れを生成 ---
    // 時間と周波数からシード値を計算
    float timeSeed = g_param1.x * g_param1.w;
    
    // X方向のランダムな揺れ（-0.5～0.5の範囲）
    float shakeX = (random(timeSeed) - 0.5f) * g_param1.y;
    
    // Y方向のランダムな揺れ（異なるシード値を使用）
    float shakeY = (random(timeSeed + g_param2.x) - 0.5f) * g_param1.z;
    
    // --- UV座標をオフセット ---
    // 揺れの量だけUV座標をずらすことで、画面全体が揺れる効果
    float2 uv = PSInput.uv + float2(shakeX, shakeY);
    
    // オフセットされたUV座標でテクスチャをサンプリング
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // 元の色をそのまま返す（UV座標がずれているだけ）
    return srcCol;
}