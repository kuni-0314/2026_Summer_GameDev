#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: デジタルレインエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 速度, z: 列の密度（列数）
}

// 疑似乱数生成の定数
static const float2 NOISE_SEED = float2(12.9898f, 78.233f);
static const float NOISE_MULTIPLIER = 43758.5453123f;

// エフェクトパラメータ
static const float CHARACTER_DENSITY = 30.0f; // 縦方向の文字密度
static const float CHARACTER_THRESHOLD = 0.5f; // 文字の出現閾値
static const float TRAIL_DECAY = 5.0f; // 文字の軌跡の減衰速度
static const float EFFECT_INTENSITY = 0.7f; // エフェクトの合成強度
 
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
    
    // --- 縦のライン（列）を生成 ---
    // 画面を垂直方向に分割し、各列が独立して動く
    float column = floor(PSInput.uv.x * g_param.z);
    
    // 各列ごとにランダムな速度を設定
    float speed = random(float2(column, 0.0f)) * g_param.y;
    
    // 時間経過によるオフセット（縦スクロール）
    float offset = frac(g_param.x * speed);
    
    // --- 文字の位置を計算 ---
    // 縦方向にスクロールする文字の位置
    float charPos = frac(PSInput.uv.y + offset);
    
    // 文字の行番号を計算（CHARACTER_DENSITY本の行に分割）
    float charRow = floor((PSInput.uv.y + offset) * CHARACTER_DENSITY);
    
    // --- ランダムな文字パターンを生成 ---
    // 列と行の組み合わせから文字の有無を決定
    // step関数: random値が閾値以上なら文字を表示（1.0）、そうでなければ非表示（0.0）
    float char = step(CHARACTER_THRESHOLD, random(float2(column, charRow)));
    
    // --- 文字の軌跡（トレイル）を生成 ---
    // 先頭は明るく、後ろに行くほど指数関数的に暗くなる
    // exp関数で滑らかな減衰を実現
    float trail = exp(-charPos * TRAIL_DECAY);
    
    // --- 緑色のマトリックス風カラーを生成 ---
    // Gチャンネルのみに値を設定（緑色）
    // trailとcharを掛け合わせて、文字がある場所だけ明るくする
    float3 matrixColor = float3(0.0f, trail * char, 0.0f);
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    
    // 元の画像とマトリックスエフェクトをブレンド
    // lerp関数で線形補間: trail * char が大きいほどマトリックス色が強くなる
    dstCol.rgb = lerp(srcCol.rgb, matrixColor, trail * char * EFFECT_INTENSITY);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}