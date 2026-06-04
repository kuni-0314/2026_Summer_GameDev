#include "../Common/Pixel/PixelShader2DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // F‚ð”½“]
    float4 dstCol;
    dstCol.rgb = 1.0f - srcCol.rgb;
    dstCol.a = srcCol.a;
    
    return dstCol;
}