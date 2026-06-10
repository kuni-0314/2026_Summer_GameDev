#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 吹雪エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 密度（雪の細かさ）, z: 速度（降る速さ）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f);
static const float NOISE_MULTIPLIER = 43758.5453123f;

// 吹雪効果の定数
static const int SNOW_LAYERS = 3; // 雪のレイヤー数（奥行き感を出すため）
static const float LAYER_OFFSET = 0.5f; // レイヤー間のオフセット
static const float LAYER_UV_OFFSET = 0.3f; // レイヤーごとのUVオフセット
static const float FLAKE_THRESHOLD = 0.95f; // 雪片の出現閾値（5%の確率）
static const float FLAKE_MAX_RADIUS = 0.3f; // 雪片の最大半径
static const float LAYER_FADE = 0.3f; // レイヤーごとの明るさ減衰
 
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
    
    // --- 複数レイヤーの雪を生成 ---
    // 異なる速度と密度のレイヤーを重ねることで、奥行き感と複雑さを表現
    float snow = 0.0f;
    
    // [unroll] 属性でループ展開（固定3回の軽量処理）
    [unroll]
    for (int i = 0; i < SNOW_LAYERS; i++)
    {
        // レイヤーごとのオフセット
        float layer = float(i) * LAYER_OFFSET;
        
        // --- レイヤーごとのUV座標を計算 ---
        // 密度を変えることで雪片のサイズを変化
        // 時間を加算することで降雪アニメーション
        float2 snowUV = PSInput.uv;
        snowUV.y += g_param.x * g_param.z; // Y方向に移動（降雪）
        
        // レイヤーごとに異なる密度とオフセット
        float2 layerUV = snowUV * g_param.y * (1.0f + layer);
        layerUV = frac(layerUV + layer * LAYER_UV_OFFSET);
        
        // グリッドのセルID（各セルに1つの雪片）
        float2 cellID = floor(layerUV * g_param.y * (1.0f + layer));
        
        // --- 各セルに雪片を配置するか判定 ---
        // 時間とレイヤーで異なるシード値を使ってランダムに配置
        float flake = random(cellID + float2(0.0f, floor(g_param.x * g_param.z + layer)));
        
        // セル内のローカル座標
        float2 cellUV = frac(layerUV * g_param.y * (1.0f + layer));
        
        // セル中心からの距離
        float dist = length(cellUV - 0.5f);
        
        // --- 雪片の形状と明るさを計算 ---
        // step関数: flake >= FLAKE_THRESHOLD なら雪片を配置
        // smoothstep関数: 円形の雪片を生成（中心が明るく、外側が暗い）
        // (1.0f - layer * LAYER_FADE): 手前のレイヤーほど明るく
        snow += step(FLAKE_THRESHOLD, flake) *
                smoothstep(FLAKE_MAX_RADIUS, 0.0f, dist) *
                (1.0f - layer * LAYER_FADE);
    }
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    
    // 元の色に雪（白）を加算
    // 雪は光源なので加算合成が自然
    dstCol.rgb = srcCol.rgb + snow;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}