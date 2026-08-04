#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 波形エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間（波のアニメーション）, y: 波の強度（振幅）, z: 波の周波数（波の細かさ）
}

// 波形エフェクトの定数
static const float VERTICAL_WAVE_PHASE = 0.7f; // 縦方向の波の位相オフセット（水平と異なる動きを作る）
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標をコピー（歪ませる）
    float2 uv = PSInput.uv;
    
    // --- 水平方向の波 ---
    // sin関数を使用して正弦波を生成
    // uv.y * g_param.z: Y座標に周波数を掛けて波の細かさを調整
    // g_param.x: 時間を加算してアニメーション
    // g_param.y: 振幅（波の強さ）
    uv.x += sin(uv.y * g_param.z + g_param.x) * g_param.y;
    
    // --- 垂直方向の波 ---
    // cos関数を使用（sinと異なる波形パターン）
    // g_param.x * VERTICAL_WAVE_PHASE: 異なる速度で動かすことで複雑な歪みを生成
    uv.y += cos(uv.x * g_param.z + g_param.x * VERTICAL_WAVE_PHASE) * g_param.y;
    
    // 歪んだUV座標でテクスチャをサンプリング
    float4 srcCol = tex.Sample(texSampler, uv);
    
    // 完全に透明なピクセルは破棄
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}