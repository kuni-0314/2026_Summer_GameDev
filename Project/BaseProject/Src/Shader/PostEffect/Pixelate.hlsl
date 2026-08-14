#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ピクセル化エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_pixelSize; // x: ピクセルサイズ（大きいほど粗いピクセル）
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // ピクセルサイズを取得
    float pixelSize = g_pixelSize.x;
    
    // --- UV座標をピクセル単位に量子化 ---
    // 1. UV座標をピクセルサイズで割る（グリッドに分割）
    // 2. floor関数で整数化（同じグリッド内は同じ値に）
    // 3. ピクセルサイズを掛けて元のUV範囲に戻す
    float2 pixelatedUV = floor(PSInput.uv / pixelSize) * pixelSize;
    
    // ピクセルの中心からサンプリング（より正確な色取得）
    pixelatedUV += pixelSize * 0.5f;
    
    // 量子化されたUV座標でテクスチャをサンプリング
    // 同じグリッド内の全ピクセルが同じ色になる
    float4 srcCol = tex.Sample(texSampler, pixelatedUV);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}