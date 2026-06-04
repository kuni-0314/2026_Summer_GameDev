#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_texelSize; // x: 1/width, y: 1/height
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 texel = g_texelSize.xy;
    
    // 周囲のピクセルをサンプリング
    float4 tl = tex.Sample(texSampler, PSInput.uv + float2(-texel.x, -texel.y));
    float4 br = tex.Sample(texSampler, PSInput.uv + float2(texel.x, texel.y));
    
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // エンボス計算
    float3 diff = (tl.rgb - br.rgb) + 0.5f;
    
    float4 dstCol;
    dstCol.rgb = diff;
    dstCol.a = srcCol.a;
    
    return dstCol;
}