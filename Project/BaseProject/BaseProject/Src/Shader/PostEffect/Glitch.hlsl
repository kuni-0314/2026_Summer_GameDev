#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: グリッチエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_time; // x: 時間(アニメーション用、グリッチのランダム性に影響)
}

// グリッチ効果の定数
static const float2 HASH_VEC = float2(12.345f, 67.890f); // 疑似乱数生成用のハッシュベクトル
static const float AMPLIFY = 12345.67890f; // 乱数の増幅係数
static const float LINE_COUNT = 20.0f; // 画面を分割する水平ライン数
static const float GLITCH_PROBABILITY = 0.05f; // グリッチが発生する確率（5%）
static const float GLITCH_INTENSITY = 2.0f; // 横ずれの強度
static const float RGB_SPLIT_THRESHOLD = 0.8f; // RGB分離が発生する閾値（80%以上）
static const float RGB_SPLIT_OFFSET = 0.01f; // RGB分離のオフセット量（画面の1%）
static const float TIME_SCALE = 10.0f; // 時間のスケール（アニメーション速度）
 
// 疑似乱数生成関数（0.0～1.0の範囲）
// ハッシュ関数を使用して座標から決定的な乱数を生成
float random(float2 st)
{
    // ドット積でハッシュ値を計算
    float hash = dot(st, HASH_VEC);
    
    // sin関数を使って値を分散させ、frac関数で小数部分のみ取得
    // この手法は古典的だが高速で、シェーダーでよく使われる
    return frac(sin(hash) * AMPLIFY);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標をコピー（後で歪ませる）
    float2 uv = PSInput.uv;
    
    // --- 水平方向のグリッチ（ライン単位で横ずれ） ---
    
    // 画面をLINE_COUNT本の水平ラインに分割
    // floor関数で離散的なライン番号を取得
    float glitchLine = floor(uv.y * LINE_COUNT);
    
    // 各ラインごとに時間依存の乱数を生成
    // 時間を量子化（floor）することで、一定期間同じグリッチパターンを保持
    float glitchRandom = random(float2(glitchLine, floor(g_time.x * TIME_SCALE)));
    
    // 確率的に横ずれを適用
    // step関数で条件分岐を回避: glitchRandom < GLITCH_PROBABILITY なら 1.0、そうでなければ 0.0
    // 従来の if 文による分岐は GPU のパイプライン効率を低下させる可能性がある
    uv.x += glitchRandom * GLITCH_INTENSITY * step(glitchRandom, GLITCH_PROBABILITY);
    
    // 歪んだUV座標でテクスチャをサンプリング
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは処理をスキップ（早期リターン最適化）
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- RGB色収差効果（RGB分離） ---
    
    // 高い乱数値（80%以上）の場合のみRGB分離を発生させる
    // step関数: glitchRandom >= RGB_SPLIT_THRESHOLD なら 1.0、そうでなければ 0.0
    float rgbSplitMask = step(RGB_SPLIT_THRESHOLD, glitchRandom);
    float offset = RGB_SPLIT_OFFSET * rgbSplitMask;
    
    // オフセットベクトルを事前計算（水平方向のみ）
    float2 offsetVec = float2(offset, 0.0f);
    
    // RGB分離エフェクトの適用
    // offset > 0 の場合のみ追加サンプリングを実行（条件分岐だが、高頻度ではないため許容）
    float4 dstCol = srcCol;
    if (offset > 0.0f)
    {
        // Rチャンネル: 右方向にずらす
        dstCol.r = tex.Sample(texSampler, uv + offsetVec).r;
        
        // Bチャンネル: 左方向にずらす
        dstCol.b = tex.Sample(texSampler, uv - offsetVec).b;
        
        // Gチャンネル: 元の位置のまま（srcCol.gを使用）
    }
    
    return dstCol;
}