#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_intensity; // x: 強度
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 direction = PSInput.uv - 0.5f;
    float offset = g_intensity.x * 0.01f;
    
    // RGB各チャンネルを異なる位置からサンプリング
    float r = tex.Sample(texSampler, PSInput.uv + direction * offset).r;
    float g = tex.Sample(texSampler, PSInput.uv).g;
    float b = tex.Sample(texSampler, PSInput.uv - direction * offset).b;
    float a = tex.Sample(texSampler, PSInput.uv).a;
    
    if (a < 0.01f)
    {
        discard;
    }
    
    return float4(r, g, b, a);
}