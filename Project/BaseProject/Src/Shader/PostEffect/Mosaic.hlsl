#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_mosaicDivision; // x: 横の分割数, y: 縦の分割数
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標をブロック単位に量子化
    float2 blockSize = 1.0f / g_mosaicDivision.xy;
    float2 blockUV = floor(PSInput.uv / blockSize) * blockSize;
    
    // ブロック内の複数点をサンプリングして平均化
    float4 avgColor = float4(0, 0, 0, 0);
    const int samples = 4; // 4x4
    
    for (int y = 0; y < samples; y++)
    {
        for (int x = 0; x < samples; x++)
        {
            float2 offset = float2(x, y) / float(samples) * blockSize;
            avgColor += tex.Sample(texSampler, blockUV + offset);
        }
    }
    
    float4 srcCol = avgColor / float(samples * samples);
    
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}