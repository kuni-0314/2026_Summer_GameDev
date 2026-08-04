#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ: フェードホワイトエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間 (time) - アニメーション用の経過時間
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // フェードホワイトの強度を計算
    // g_param.x は時間に応じて変化する値で、フェードの進行度を表す
    float fadeIntensity = smoothstep(0.0f, 1.0f, saturate(g_param.x)); // 0.0から1.0の範囲に制限
    
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // フェードホワイトの色を計算
    float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 finalColor = lerp(srcCol, white, fadeIntensity);
    
    
    return finalColor;
}