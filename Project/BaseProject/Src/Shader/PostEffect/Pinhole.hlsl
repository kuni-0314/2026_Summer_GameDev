#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ピンホールカメラエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 穴のサイズ（0.0～1.0）, y: 歪みの強度（樽型歪みの度合い）
}

// ピンホール効果の定数
static const float2 CENTER = float2(0.5f, 0.5f); // 画面中心（穴の位置）
static const float MASK_SMOOTHNESS = 0.1f; // マスクのぼかし幅
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 画面中心からのベクトルを取得
    float2 toCenter = PSInput.uv - CENTER;
    
    // 中心からの距離を計算
    // length() = sqrt(x*x + y*y)
    float dist = length(toCenter);
    
    // 歪み係数を計算
    //float factor = pow(dist, g_param.y - 1.0f);
    float factor = exp2(log2(dist) * g_param.y);
    
    // 歪み後のUVを計算
    float2 uv = CENTER + toCenter * factor;
    
    // 歪んだUV座標でテクスチャをサンプリング
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 円形マスク（ビネット効果） ---
    // ピンホールの穴を通して見ているような効果
    // smoothstep関数で滑らかな円形のマスクを生成
    // 
    // g_param.x: 穴のサイズ（外側の半径）
    // g_param.x + MASK_SMOOTHNESS: ぼかしの外側
    // dist が穴のサイズより小さい: mask = 1.0（完全に表示）
    // dist が穴のサイズより大きい: mask = 0.0（完全に暗い）
    float mask = smoothstep(g_param.x + MASK_SMOOTHNESS, g_param.x, dist);
    
    // マスクを適用して周辺を暗くする
    float4 dstCol;
    dstCol.rgb = srcCol.rgb * mask;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}