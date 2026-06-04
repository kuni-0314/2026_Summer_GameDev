#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 強度, y: 範囲
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // 中心からの距離を計算
    float2 center = PSInput.uv - 0.5f;
    float dist = length(center);
    
    // ビネット効果を適用
    float vignette = smoothstep(g_param.y, g_param.y - g_param.x, dist);
    
    float4 dstCol = srcCol;
    dstCol.rgb *= vignette;
    
    return dstCol;
}