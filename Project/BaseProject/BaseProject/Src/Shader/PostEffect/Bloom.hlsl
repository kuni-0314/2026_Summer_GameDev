#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ブルームエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 閾値（この値以上の明るさのピクセルを抽出）, y: ブルーム強度（光の広がりの強さ）
}

// テクセルサイズ（1ピクセルのUV座標上でのサイズ）
// 定数化することでコンパイル時に最適化され、毎フレーム計算する必要がなくなる
static const float2 TEXEL_SIZE = float2(0.001f, 0.001f);

// サンプリング数の逆数（25サンプル = 5x5グリッド）
// 除算は乗算より遅いため、逆数を事前に計算して乗算に変換
static const float INV_SAMPLES = 1.0f / 25.0f;
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // アルファ値が非常に小さい場合はピクセルを破棄
    // 完全に透明なピクセルの処理をスキップして処理負荷を軽減
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // ブルーム効果の累積値
    float4 bloom = float4(0, 0, 0, 0);
    
    // 閾値をローカル変数に格納（レジスタアクセスを最適化）
    float threshold = g_param.x;
    
    // 5x5のガウシアンブラー風サンプリング
    // [unroll] 属性: コンパイラにループを展開させ、ループ制御のオーバーヘッドを削減
    // GPUは並列処理が得意なため、ループ展開により性能向上
    [unroll]
    for (int x = -2; x <= 2; x++)
    {
        [unroll]
        for (int y = -2; y <= 2; y++)
        {
            // 現在のピクセルからのオフセット量を計算
            float2 offset = float2(x, y) * TEXEL_SIZE;
            
            // オフセット位置のピクセルをサンプリング
            float4 sample = tex.Sample(texSampler, PSInput.uv + offset);
            
            // ピクセルの輝度を計算（RGB値の最大値を取得）
            // max3パターン: RGB各チャンネルの最大値が輝度の指標となる
            float brightness = max(sample.r, max(sample.g, sample.b));
            
            // 閾値以上の明るさを持つピクセルのみをブルームに加算
            // step(threshold, brightness): brightness >= threshold なら 1.0、それ以外は 0.0
            // この関数を使うことで条件分岐（if文）を回避し、GPU性能を向上
            // 条件分岐はGPUのパイプラインを停滞させる可能性があるため、数学的演算で代替
            bloom += sample * (brightness - threshold) * step(threshold, brightness);
        }
    }
    
    // 累積したブルーム値を平均化
    // 除算の代わりに事前計算した逆数を乗算（乗算は除算より高速）
    bloom *= INV_SAMPLES;
    
    // 元の色にブルーム効果を加算合成
    // g_param.y で光の広がりの強度を調整
    float4 dstCol = srcCol + bloom * g_param.y;
    
    // アルファ値は元の値を維持（ブルームは色のみに影響）
    dstCol.a = srcCol.a;
    
    return dstCol;
}