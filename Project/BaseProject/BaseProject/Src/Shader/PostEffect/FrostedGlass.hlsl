#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: すりガラスエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間（ランダムパターン用）, y: フロスト強度（ぼかしの度合い）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f);
static const float NOISE_MULTIPLIER = 43758.5453123f;

// フロストガラス効果の定数
static const float RANDOM_OFFSET_SCALE = 0.05f; // ランダムオフセットのスケール
static const int BLUR_SAMPLES = 6; // ブラーのサンプル数
static const float INV_BLUR_SAMPLES = 1.0f / float(BLUR_SAMPLES); // サンプル数の逆数
static const float TWO_PI = 6.28318530718f; // 円周率の2倍
static const float BLUR_RADIUS = 0.01f; // ブラーの半径
static const float BLUR_MIX = 0.5f; // ブラーの混合率
static const float BRIGHTNESS_REDUCTION = 0.9f; // 明るさの低減率（すりガラスの透過性）
 
// 疑似乱数生成関数（0.0～1.0の範囲）
float random(float2 st)
{
    return frac(sin(dot(st, NOISE_SEED)) * NOISE_MULTIPLIER);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標をコピー
    float2 uv = PSInput.uv;
    
    // --- ランダムなオフセットでランダムサンプリング ---
    // すりガラスの微細な凹凸を模倣
    // 時間で変化するシード値を使って異なるランダムパターンを生成
    float2 randomOffset = float2(
        random(uv + g_param.x * 0.1f), // X方向のランダム値
        random(uv + g_param.x * 0.1f + 0.5f) // Y方向のランダム値（異なるシード）
    ) - 0.5f; // -0.5～0.5の範囲に変換（中心を0に）
    
    // フロスト強度に応じてオフセット量を調整
    randomOffset *= g_param.y * RANDOM_OFFSET_SCALE;
    uv += randomOffset;
    
    // オフセットされた位置から色をサンプリング
    float4 col = tex.Sample(texSampler, uv);
    
    // --- 周辺ぼかし効果 ---
    // 円形パターンで周囲をサンプリングしてぼかす
    float4 blur = float4(0, 0, 0, 0);
    
    // [unroll] 属性でループ展開を促進（固定回数の軽量処理）
    [unroll]
    for (int i = 0; i < BLUR_SAMPLES; i++)
    {
        // 円周上に均等に配置されたサンプリング点を計算
        float angle = float(i) * INV_BLUR_SAMPLES * TWO_PI;
        
        // 極座標から直交座標への変換
        float2 offset = float2(cos(angle), sin(angle)) * g_param.y * BLUR_RADIUS;
        
        // サンプリングして累積
        blur += tex.Sample(texSampler, uv + offset);
    }
    
    // 累積した色を平均化
    blur *= INV_BLUR_SAMPLES;
    
    // --- 元の色とぼかしを混合 ---
    // すりガラスは完全にぼけるわけではなく、元の画像も見える
    float4 result = lerp(col, blur, BLUR_MIX);
    
    // 完全に透明なピクセルは処理をスキップ
    if (result.a < 0.01f)
    {
        discard;
    }
    
    // --- 明るさを少し下げる ---
    // すりガラスは光を散乱させるため、若干暗く見える
    result.rgb *= BRIGHTNESS_REDUCTION;
    
    return result;
}