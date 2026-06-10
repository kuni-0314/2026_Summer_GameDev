#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: エンボスエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_texelSize; // x: 1/width（テクセルの幅）, y: 1/height（テクセルの高さ）
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // テクセルサイズを取得
    float2 texel = g_texelSize.xy;
    
    // --- 対角のピクセルをサンプリング ---
    // エンボス効果は対角の輝度差を利用して立体感を表現
    float4 tl = tex.Sample(texSampler, PSInput.uv + float2(-texel.x, -texel.y)); // 左上
    float4 br = tex.Sample(texSampler, PSInput.uv + float2(texel.x, texel.y)); // 右下
    
    // 中心ピクセル
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- エンボス計算 ---
    // 左上と右下の色の差を計算し、0.5を加算して中間グレーを基準にする
    // 差が正: 明るく（光が当たっている）
    // 差が負: 暗く（影になっている）
    // 0.5を加算することで、差がない部分は中間グレー（浮き彫りなし）になる
    float3 diff = (tl.rgb - br.rgb) + 0.5f;
    
    float4 dstCol;
    dstCol.rgb = diff;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}