#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 液体歪みエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間, y: 歪み強度, z: 波の密度
}

// 液体歪みの定数
static const float WAVE1_FREQ_X = 5.0f; // 波1のX方向周波数
static const float WAVE1_FREQ_Y = 3.0f; // 波1のY方向周波数
static const float WAVE1_SPEED_X = 2.0f; // 波1のX方向速度
static const float WAVE1_SPEED_Y = 1.5f; // 波1のY方向速度
static const float WAVE2_FREQ_X = 4.0f; // 波2のX方向周波数
static const float WAVE2_FREQ_Y = 7.0f; // 波2のY方向周波数
static const float WAVE2_SPEED_X = 1.8f; // 波2のX方向速度
static const float WAVE2_SPEED_Y = 2.2f; // 波2のY方向速度
static const float DISTORTION_SCALE = 0.02f; // 歪みのスケール
static const float REFRACT_MULTIPLIER = 2.0f; // 屈折オフセットの倍率
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標をコピー（歪ませる）
    float2 uv = PSInput.uv;
    
    // --- 複雑な波を組み合わせて液体のような歪み ---
    // 2つの異なる周波数と速度の波を重ね合わせることで、
    // 水や液体の自然な動きを再現
    
    // 波1: sin × cos の組み合わせ（X-Y方向の相互作用）
    float wave1 = sin(uv.x * g_param.z * WAVE1_FREQ_X + g_param.x * WAVE1_SPEED_X) *
                  cos(uv.y * g_param.z * WAVE1_FREQ_Y + g_param.x * WAVE1_SPEED_Y);
    
    // 波2: sin × cos の組み合わせ（異なるパターン、逆方向に動く）
    float wave2 = sin(uv.y * g_param.z * WAVE2_FREQ_Y - g_param.x * WAVE2_SPEED_X) *
                  cos(uv.x * g_param.z * WAVE2_FREQ_X - g_param.x * WAVE2_SPEED_Y);
    
    // 2つの波を合成してUV座標を歪ませる
    float2 distortion = float2(wave1, wave2) * g_param.y * DISTORTION_SCALE;
    uv += distortion;
    
    // --- 液体による色の屈折 ---
    // 屈折率の違いによるRGB分離を模倣
    // 歪みの大きさに応じて屈折オフセットを計算
    float refractOffset = length(distortion) * REFRACT_MULTIPLIER;
    
    // RGB各チャンネルを微妙に異なる位置からサンプリング
    float r = tex.Sample(texSampler, uv + float2(refractOffset, 0.0f)).r;
    float g = tex.Sample(texSampler, uv).g;
    float b = tex.Sample(texSampler, uv - float2(refractOffset, 0.0f)).b;
    
    // 歪んだUV座標でアルファ値を取得
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // 屈折したRGB値を組み合わせる
    float4 dstCol;
    dstCol.rgb = float3(r, g, b);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}