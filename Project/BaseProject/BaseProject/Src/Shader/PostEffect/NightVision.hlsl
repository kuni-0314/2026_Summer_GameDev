#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 暗視エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間（スキャンラインのアニメーション用）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f);
static const float NOISE_MULTIPLIER = 43758.5453123f;

// 暗視効果の定数
static const float3 LUMINANCE_WEIGHTS = float3(0.299f, 0.587f, 0.114f); // ITU-R BT.601
static const float3 NIGHT_VISION_TINT = float3(0.1f, 1.5f, 0.1f); // 緑色の色調（G成分を強調）
static const float VIGNETTE_INNER = 0.5f; // ビネット効果の内側の半径
static const float VIGNETTE_OUTER = 0.7f; // ビネット効果の外側の半径
static const float SCANLINE_FREQUENCY = 300.0f; // スキャンラインの周波数
static const float SCANLINE_SPEED = 10.0f; // スキャンラインの移動速度
static const float SCANLINE_INTENSITY = 0.05f; // スキャンラインの強度
static const float NOISE_INTENSITY = 0.1f; // ノイズの強度
 
// 疑似乱数生成関数（0.0～1.0の範囲）
float random(float2 st)
{
    return frac(sin(dot(st, NOISE_SEED)) * NOISE_MULTIPLIER);
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
    
    // --- 輝度計算 ---
    // グレースケールに変換して明るさの情報を取得
    float luminance = dot(srcCol.rgb, LUMINANCE_WEIGHTS);
    
    // --- 緑色に変換（暗視ゴーグルの特徴的な色） ---
    // 輝度に緑色の色調を乗算
    // Gチャンネルを1.5倍に強調し、R/Bチャンネルを抑制
    float3 nightVision = luminance * NIGHT_VISION_TINT;
    
    // --- ビネット効果（円形の減光） ---
    // 暗視ゴーグルのレンズを通して見ているような効果
    float2 center = PSInput.uv - 0.5f;
    float dist = length(center);
    
    // smoothstep関数で滑らかな円形のマスクを生成
    float vignette = smoothstep(VIGNETTE_OUTER, VIGNETTE_INNER, dist);
    
    // --- スキャンライン（水平走査線） ---
    // 電子機器のディスプレイを模倣
    // 時間で動くことでアニメーション効果
    float scanline = sin(PSInput.uv.y * SCANLINE_FREQUENCY + g_param.x * SCANLINE_SPEED);
    scanline = scanline * SCANLINE_INTENSITY + 0.95f;
    
    // --- ノイズ（電子ノイズ） ---
    // 暗視装置の増幅ノイズを再現
    float noise = random(float2(PSInput.uv.y * g_param.x, PSInput.uv.x));
    noise = noise * NOISE_INTENSITY + 0.9f;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    dstCol.rgb = nightVision * vignette * scanline * noise;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}