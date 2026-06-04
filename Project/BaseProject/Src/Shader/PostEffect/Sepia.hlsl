#include "../Common/Pixel/PixelShader2DHeader.hlsli"

static const float3 SEPIA_R = float3(0.393, 0.769, 0.189);
static const float3 SEPIA_G = float3(0.349, 0.686, 0.168);
static const float3 SEPIA_B = float3(0.272, 0.534, 0.131);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // ƒZƒsƒA’²•ÏŠ·
    float4 dstCol;
    dstCol.r = dot(srcCol.rgb, SEPIA_R);
    dstCol.g = dot(srcCol.rgb, SEPIA_G);
    dstCol.b = dot(srcCol.rgb, SEPIA_B);
    dstCol.a = srcCol.a;
    
    return dstCol;
}