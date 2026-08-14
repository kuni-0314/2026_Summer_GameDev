#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 古いフィルムエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間（ノイズと傷のアニメーション用）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f); // ハッシュ用シード値
static const float NOISE_MULTIPLIER = 43758.5453123f; // 乱数の増幅係数

// セピア調フィルタの変換行列（標準的なセピア変換係数）
static const float3 SEPIA_R = float3(0.393f, 0.769f, 0.189f); // Rチャンネル変換
static const float3 SEPIA_G = float3(0.349f, 0.686f, 0.168f); // Gチャンネル変換
static const float3 SEPIA_B = float3(0.272f, 0.534f, 0.131f); // Bチャンネル変換

// エフェクトパラメータ
static const float VIGNETTE_STRENGTH = 1.5f; // ビネット効果の強度（周辺減光）
static const float NOISE_INTENSITY = 0.1f; // グレインノイズの強度
static const float SCRATCH_THRESHOLD = 0.995f; // 傷が発生する閾値（0.5%の確率）
static const float SCRATCH_INTENSITY = 0.5f; // 傷の明るさ
 
// 疑似乱数生成関数（0.0～1.0の範囲）
// 古典的なハッシュ関数ベースの乱数生成
float random(float2 st)
{
    // ドット積とsin関数を組み合わせてハッシュ値を生成
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
    
    // --- セピア調変換 ---
    // RGB各チャンネルを線形変換してセピア色に
    // ドット積を使用して効率的に計算
    float3 sepia;
    sepia.r = dot(srcCol.rgb, SEPIA_R); // 新しいRチャンネル
    sepia.g = dot(srcCol.rgb, SEPIA_G); // 新しいGチャンネル
    sepia.b = dot(srcCol.rgb, SEPIA_B); // 新しいBチャンネル
    
    // --- ビネット効果（周辺減光） ---
    // 画面中心から離れるほど暗くなる効果
    float2 center = PSInput.uv - 0.5f; // 中心を原点とした座標
    
    // 中心からの距離の二乗を計算（dot積で効率的に）
    // 距離が大きいほど暗くなる（1.0から減算）
    float vignette = 1.0f - dot(center, center) * VIGNETTE_STRENGTH;
    
    // --- グレインノイズ（フィルムの粒子感） ---
    // 時間とY座標から乱数を生成し、微細なノイズを追加
    // 0.9～1.0の範囲で明るさを変動させる
    float noise = random(float2(PSInput.uv.y, g_param.x)) * NOISE_INTENSITY + 0.9f;
    
    // --- 傷エフェクト ---
    // 稀に縦線の傷を追加
    float scratchRandom = random(float2(g_param.x, PSInput.uv.x));
    
    // step関数で条件分岐を回避
    // scratchRandom > SCRATCH_THRESHOLD の場合のみ傷を追加
    float scratchMask = step(SCRATCH_THRESHOLD, scratchRandom);
    float scratch = random(float2(PSInput.uv.y, g_param.x)) * SCRATCH_INTENSITY * scratchMask;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    // セピア色にビネット、ノイズを乗算し、傷を加算
    dstCol.rgb = sepia * vignette * noise + scratch;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}