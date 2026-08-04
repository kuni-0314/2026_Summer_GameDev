#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: CRTエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 歪曲強度, z: 走査線強度, w: 縁の丸み（0.0～1.0）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f);
static const float NOISE_MULTIPLIER = 43758.5453123f;

// CRT効果の定数
static const float ZOOM = 0.9f; // ズームアウト係数（画面を少し縮小）
static const float CURVATURE_Y_FACTOR = 0.8f; // Y方向の歪曲係数（X方向より弱め）
static const float SCANLINE_FREQUENCY = 600.0f; // 走査線の周波数
static const float RGB_SPLIT_OFFSET = 0.002f; // RGB色収差のオフセット量
static const float VIGNETTE_SCALE = 1.2f; // ビネット効果のスケール
static const float VIGNETTE_STRENGTH = 0.25f; // ビネット効果の強度
static const float VIGNETTE_POWER = 0.5f; // ビネット効果の曲線
static const float FLICKER_FREQUENCY = 50.0f; // フリッカーの周波数
static const float FLICKER_INTENSITY = 0.02f; // フリッカーの強度
static const float NOISE_INTENSITY = 0.03f; // ホワイトノイズの強度
static const float BRIGHTNESS = 0.95f; // 全体的な明るさ
 
// 疑似乱数生成関数（0.0～1.0の範囲）
float random(float2 st)
{
    return frac(sin(dot(st, NOISE_SEED)) * NOISE_MULTIPLIER);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- 中心を原点とする座標系に変換 ---
    float2 centered = PSInput.uv - 0.5f;
    
    // --- ズームアウト効果 ---
    // 画面を少し縮小してCRTの黒縁を表現
    centered /= ZOOM;
    
    // --- CRTの歪曲（樽型歪み） ---
    // ブラウン管の湾曲した画面を再現
    float2 distortion = centered;
    
    // Y座標に基づいてX方向を歪ませる
    float curvatureX = abs(centered.y) * abs(centered.y) * g_param.y;
    
    // X座標に基づいてY方向を歪ませる（弱め）
    float curvatureY = abs(centered.x) * abs(centered.x) * g_param.y * CURVATURE_Y_FACTOR;
    
    distortion.x *= 1.0f + curvatureX;
    distortion.y *= 1.0f + curvatureY;
    
    // 中心を(0.5, 0.5)に戻す
    float2 distortedUV = distortion + 0.5f;
    
    // --- 画面外チェック（縁の丸み対応） ---
    // w パラメータで縁の丸みを調整
    float margin = lerp(0.0f, 0.15f, g_param.w);
    
    if (distortedUV.x < -margin || distortedUV.x > 1.0f + margin ||
        distortedUV.y < -margin || distortedUV.y > 1.0f + margin)
    {
        // 画面外は黒で塗りつぶす
        return float4(0, 0, 0, 1);
    }
    
    // 歪んだUV座標でテクスチャをサンプリング
    float4 srcCol = tex.Sample(texSampler, distortedUV);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 走査線（水平の縞模様） ---
    // sin関数で周期的な明暗パターンを生成
    float scanline = sin(distortedUV.y * SCANLINE_FREQUENCY) * 0.5f + 0.5f;
    scanline = lerp(1.0f, scanline, g_param.z);
    
    // --- RGB色収差（ブラウン管の色ずれ） ---
    // RGB各チャンネルを微妙にずらして表示
    float r = tex.Sample(texSampler, distortedUV + float2(RGB_SPLIT_OFFSET, 0.0f)).r;
    float b = tex.Sample(texSampler, distortedUV - float2(RGB_SPLIT_OFFSET, 0.0f)).b;
    float3 chromatic = float3(r, srcCol.g, b);
    
    // --- ビネット（周辺減光） ---
    float2 vignetteUV = centered * VIGNETTE_SCALE;
    //float vignette = 1.0f - dot(vignetteUV, vignetteUV) * VIGNETTE_STRENGTH;
    //vignette = pow(saturate(vignette), VIGNETTE_POWER);
    float r2 = dot(vignetteUV, vignetteUV);
    float vignette = 1.0f - r2 * VIGNETTE_STRENGTH;
    vignette = saturate(vignette);
    
    // --- 画面の角を丸める ---
    // w パラメータで丸みの強さを調整
    float cornerSize = lerp(2.5f, 1.2f, g_param.w);
    float2 corner = abs(centered) * cornerSize;
    float cornerSmoothStart = lerp(1.0f, 0.9f, g_param.w);
    float cornerSmoothEnd = lerp(1.1f, 1.0f, g_param.w);
    float cornerMask = 1.0f - smoothstep(cornerSmoothStart, cornerSmoothEnd, max(corner.x, corner.y));
    
    // --- フリッカー（微細な明滅） ---
    // 古いモニターの不安定な映像を再現
    float flicker = sin(g_param.x * FLICKER_FREQUENCY) * FLICKER_INTENSITY + 0.98f;
    
    // --- ホワイトノイズ（ランダムな粒子） ---
    float noise = random(distortedUV * g_param.x) * NOISE_INTENSITY;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    dstCol.rgb = chromatic * scanline * vignette * cornerMask * flicker * BRIGHTNESS + noise;
    dstCol.a = srcCol.a;
    
    return dstCol;
}