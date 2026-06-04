#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbParam : register(b4)
{
    float4 g_time; // x: 時間(アニメーション用)
}
 
// 疑似乱数生成(0.0～1.0の範囲)
float random(float2 st)
{
    // ハッシュ用の定数ベクトル
    static const float2 HASH_VEC = float2(12.345f, 67.890f);
    // 増幅用の定数
    static const float AMPLIFY = 12345.67890f;
    // 数に深い意味は無い
    
    float hash = dot(st, HASH_VEC);
    return frac(sin(hash) * AMPLIFY);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    
    // ランダムに横ずれを発生
    float glitchLine = floor(uv.y * 20.0f); // 20本のラインに分割
    float glitchRandom = random(float2(glitchLine, floor(g_time.x * 10.0f)));
    
    if (glitchRandom < 0.05f)  // 0.0～0.05 の範囲(5%)
    {
        uv.x += glitchRandom * 2.0f;
    }
    
    float4 srcCol = tex.Sample(texSampler, uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // RGBずらし（色収差のような効果）
    float offset = 0.01f * step(0.8f, glitchRandom); // glitchRandomが0.8以上のときに画面の1%だけずらす
    float r = tex.Sample(texSampler, uv + float2(offset, 0.0f)).r;
    float b = tex.Sample(texSampler, uv - float2(offset, 0.0f)).b;
    
    float4 dstCol = srcCol;
    dstCol.r = r;
    dstCol.b = b;
    
    return dstCol;
}