#include "../Common/Pixel/PixelShader2DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // グレースケール変換
    float4 dstCol;
    float gray = (srcCol.r + srcCol.g + srcCol.b) / 3.0f;
    dstCol.rgb = float3(gray, gray, gray);
    dstCol.a = srcCol.a;
    
    return dstCol;
}