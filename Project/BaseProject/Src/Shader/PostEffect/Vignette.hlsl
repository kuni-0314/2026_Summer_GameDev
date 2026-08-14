#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ビネットエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 強度（減光の急峻さ）, y: 範囲（減光が始まる距離）
}
 
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
    // UV座標を中心(0.5, 0.5)を原点とする座標系に変換
    float2 center = PSInput.uv - 0.5f;
    
    // 中心からの距離を計算（ユークリッド距離）
    // length関数は sqrt(x*x + y*y) と等価
    float dist = length(center);
    
    // --- ビネット効果を適用 ---
    // smoothstep関数: 指定範囲で滑らかに0から1に変化
    // dist が g_param.y - g_param.x より小さい: vignette = 1.0（減光なし）
    // dist が g_param.y より大きい: vignette = 0.0（完全に暗い）
    // その間は滑らかに補間される（エルミート補間）
    float vignette = smoothstep(g_param.y, g_param.y - g_param.x, dist);
    
    // 元の色にビネット係数を乗算して暗くする
    float4 dstCol = srcCol;
    dstCol.rgb *= vignette;
    
    return dstCol;
}