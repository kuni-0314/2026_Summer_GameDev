#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 深度フォグエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: フォグの開始距離, y: フォグ範囲, z: フォグの濃度（0.0～1.0）
}

// 深度フォグの定数
static const float2 CENTER = float2(0.5f, 0.5f); // 画面中心
static const float3 FOG_COLOR = float3(0.8f, 0.8f, 0.9f); // フォグの色（薄い青みがかった灰色）
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 画面中心からの距離を計算 ---
    // 2D画面での疑似的な深度として、中心からの距離を使用
    // 中心が近景、外側が遠景という仮定
    float dist = length(PSInput.uv - CENTER);
    
    // --- フォグの強度を計算 ---
    // smoothstep関数で滑らかなフォグの濃淡を生成
    // 
    // g_param.x: フォグの開始距離（この距離まではフォグなし）
    // g_param.x + g_param.y: フォグの終了距離（この距離以降は最大濃度）
    // その間: 滑らかに補間
    float fogAmount = smoothstep(g_param.x, g_param.x + g_param.y, dist);
    
    // フォグの濃度パラメータを適用
    fogAmount *= g_param.z;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    
    // 元の色とフォグの色を線形補間
    // fogAmount = 0.0: 元の色のまま（フォグなし）
    // fogAmount = 1.0: フォグの色に完全に置き換わる
    dstCol.rgb = lerp(srcCol.rgb, FOG_COLOR, fogAmount);
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}