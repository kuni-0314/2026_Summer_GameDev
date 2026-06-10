#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ストロボエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 周波数（点滅速度）, z: 強度（効果の強さ）
}

// ストロボ効果の定数
static const float STROBE_SCALE = 10.0f; // ストロボの時間スケール
static const float COLOR_PHASE_OFFSET_G = 0.33f; // G成分の位相オフセット
static const float COLOR_PHASE_OFFSET_B = 0.66f; // B成分の位相オフセット
static const float TWO_PI = 6.28318530718f; // 円周率の2倍
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 不規則な点滅パターンを生成 ---
    // sin関数とハッシュ関数を組み合わせて、予測不可能な点滅を作る
    // frac関数とsin関数で疑似ランダムな値を生成
    float strobeRandom = frac(sin(g_param.x * g_param.y * STROBE_SCALE) * 43758.5453123f);
    
    // step関数で二値化（オン/オフの切り替え）
    // strobeRandom >= 0.5: strobe = 1.0（点灯）
    // strobeRandom < 0.5: strobe = 0.0（消灯）
    float strobe = step(0.5f, strobeRandom);
    
    // --- 色の変化（レインボーエフェクト） ---
    // 時間と周波数に応じて色相を回転
    float colorShift = frac(g_param.x * g_param.y);
    
    // sin関数で RGB 各チャンネルを位相をずらして変調
    // 結果として虹色のように色が変化する
    float3 strobeColor = float3(
        sin(colorShift * TWO_PI) * 0.5f + 0.5f, // R成分（位相0）
        sin((colorShift + COLOR_PHASE_OFFSET_G) * TWO_PI) * 0.5f + 0.5f, // G成分（位相120度）
        sin((colorShift + COLOR_PHASE_OFFSET_B) * TWO_PI) * 0.5f + 0.5f // B成分（位相240度）
    );
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    
    // 元の色とストロボ色を混合
    // strobe * g_param.z: 点灯時のみ効果を適用、強度で調整
    dstCol.rgb = lerp(srcCol.rgb, strobeColor, strobe * g_param.z);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}