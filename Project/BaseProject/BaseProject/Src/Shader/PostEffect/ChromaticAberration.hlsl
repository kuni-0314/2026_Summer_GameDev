#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 色収差エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_intensity; // x: 色収差の強度（中心からの距離に応じてRGBがずれる）
}

// 色収差の定数
static const float OFFSET_MULTIPLIER = 0.01f; // オフセット量のスケール（画面の1%単位）
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 画面中心から現在のピクセルへの方向ベクトル
    // 中心から離れるほど色収差が大きくなる
    float2 direction = PSInput.uv - 0.5f;
    
    // 色収差のオフセット量を計算
    // 強度と方向ベクトルを掛け合わせる
    float offset = g_intensity.x * OFFSET_MULTIPLIER;
    float2 offsetVec = direction * offset;
    
    // --- RGB各チャンネルを異なる位置からサンプリング ---
    // レンズの色収差を模倣：波長の違いにより焦点位置がずれる現象
    
    // Rチャンネル: 中心から外側にずれた位置（長波長は外側に屈折）
    float4 rSample = tex.Sample(texSampler, PSInput.uv + offsetVec);
    
    // Gチャンネル: 元の位置（基準）
    float4 gSample = tex.Sample(texSampler, PSInput.uv);
    
    // Bチャンネル: 中心に向かってずれた位置（短波長は内側に屈折）
    float4 bSample = tex.Sample(texSampler, PSInput.uv - offsetVec);
    
    // 完全に透明なピクセルは破棄
    if (gSample.a < 0.01f)
    {
        discard;
    }
    
    // RGB各チャンネルを組み合わせて色収差効果を生成
    // アルファ値はGチャンネル（中心）の値を使用
    return float4(rSample.r, gSample.g, bSample.b, gSample.a);
}