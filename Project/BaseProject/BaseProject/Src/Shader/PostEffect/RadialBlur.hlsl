#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 放射状ブラーのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: ブラーの強度（0.0～1.0、中心からの距離に応じてブラーが強くなる）
}

// 放射状ブラーの定数
static const float2 CENTER = float2(0.5f, 0.5f); // ブラーの中心点（画面中央）
static const int SAMPLES = 10; // サンプリング回数（多いほど滑らか）
static const float INV_SAMPLES = 1.0f / float(SAMPLES); // サンプル数の逆数（除算を乗算に変換）
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 画面中心から現在のピクセルへの方向ベクトルを計算
    // この方向に沿ってサンプリングを行う
    float2 dir = PSInput.uv - CENTER;
    
    // ブラー効果の累積値
    float4 col = float4(0, 0, 0, 0);
    
    // --- 放射状サンプリング ---
    // 中心から外側に向かって複数点をサンプリングし、平均を取る
    // これにより「動き」や「速度感」を表現
    
    // [unroll] 属性を使用してループを展開
    // サンプル数が固定（10回）かつ軽量処理なので、展開による最適化
    [unroll]
    for (int i = 0; i < SAMPLES; i++)
    {
        // サンプリング位置のスケールを計算
        // i=0: scale=1.0 (元の位置)
        // i=9: scale=1.0-強度 (中心寄り)
        // 徐々に中心に近づくようにサンプリング
        float scale = 1.0f - g_param.x * (float(i) * INV_SAMPLES);
        
        // スケールを適用した位置でサンプリング
        float2 uv = CENTER + dir * scale;
        col += tex.Sample(texSampler, uv);
    }
    
    // 累積したカラーを平均化
    // 除算の代わりに事前計算した逆数を乗算（高速化）
    col *= INV_SAMPLES;
    
    // 完全に透明なピクセルは破棄
    if (col.a < 0.01f)
    {
        discard;
    }
    
    return col;
}