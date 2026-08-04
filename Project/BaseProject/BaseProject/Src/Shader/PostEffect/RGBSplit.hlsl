#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: RGB分離エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_offset; // x: Rチャンネルのオフセット, y: Gチャンネルのオフセット, z: Bチャンネルのオフセット
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- RGB各チャンネルを異なる位置からサンプリング ---
    // アナログTV や VHS のノイズ、色ずれを再現
    
    // Rチャンネル: 右方向にオフセット
    float r = tex.Sample(texSampler, PSInput.uv + float2(g_offset.x, 0.0f)).r;
    
    // Gチャンネル: 下方向にオフセット
    float g = tex.Sample(texSampler, PSInput.uv + float2(0.0f, g_offset.y)).g;
    
    // Bチャンネル: 左方向にオフセット（Rと対称的に）
    float b = tex.Sample(texSampler, PSInput.uv + float2(-g_offset.z, 0.0f)).b;
    
    // アルファ値は元の位置から取得
    float a = tex.Sample(texSampler, PSInput.uv).a;
    
    // 完全に透明なピクセルは処理をスキップ
    if (a < 0.01f)
    {
        discard;
    }
    
    // RGB各チャンネルを組み合わせて色ずれ効果を生成
    return float4(r, g, b, a);
}