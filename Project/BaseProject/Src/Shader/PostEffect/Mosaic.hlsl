#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: モザイクエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_mosaicDivision; // x: 横の分割数（ブロック数）, y: 縦の分割数（ブロック数）
}

// サンプリング定数
static const int SAMPLES_PER_AXIS = 4; // 各軸のサンプル数（4x4=16サンプル）
static const float INV_TOTAL_SAMPLES = 1.0f / float(SAMPLES_PER_AXIS * SAMPLES_PER_AXIS); // 総サンプル数の逆数
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- UV座標をブロック単位に量子化 ---
    // 各ブロックのサイズを計算（画面全体を分割数で割る）
    float2 blockSize = 1.0f / g_mosaicDivision.xy;
    
    // 現在のUVをブロックサイズで割り、floor関数で整数化してから再度ブロックサイズを掛ける
    // これにより、同じブロック内の全ピクセルが同じUV座標を参照する
    float2 blockUV = floor(PSInput.uv / blockSize) * blockSize;
    
    // --- ブロック内の複数点をサンプリングして平均化 ---
    // エイリアシングを軽減し、より滑らかなモザイク効果を実現
    float4 avgColor = float4(0, 0, 0, 0);
    
    // [unroll] 属性でループ展開を促進
    // 固定回数（4x4=16回）の軽量処理なので展開が有効
    [unroll]
    for (int y = 0; y < SAMPLES_PER_AXIS; y++)
    {
        [unroll]
        for (int x = 0; x < SAMPLES_PER_AXIS; x++)
        {
            // ブロック内の均等分散されたオフセットを計算
            float2 offset = float2(x, y) / float(SAMPLES_PER_AXIS) * blockSize;
            
            // サンプリングして累積
            avgColor += tex.Sample(texSampler, blockUV + offset);
        }
    }
    
    // 累積した色を平均化
    // 除算の代わりに事前計算した逆数を乗算（高速化）
    float4 srcCol = avgColor * INV_TOTAL_SAMPLES;
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}