#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_levels; // x: 色段階数
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // 色を段階的に丸める
    float4 dstCol;
    dstCol.rgb = floor(srcCol.rgb * g_levels.x) / g_levels.x;
    dstCol.a = srcCol.a;
    
    return dstCol;
}