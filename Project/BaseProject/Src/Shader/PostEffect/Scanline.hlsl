#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 係数, y: 強度
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // 走査線パターン
    float scanline = sin(PSInput.uv.y * g_param.x * 3.14159f * 2.0f) * 0.5f + 0.5f; // (0～1の範囲に変換))
    scanline = lerp(1.0f, scanline, g_param.y);
    
    float4 dstCol = srcCol;
    dstCol.rgb *= scanline;
    
    return dstCol;
}