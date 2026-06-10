#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: ドットマトリクスエフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: ドットサイズ（グリッド分割数）, y: ドット間隔（ドットの大きさ調整）
}

// ドットマトリクス効果の定数
static const float3 LUMINANCE_WEIGHTS = float3(0.299f, 0.587f, 0.114f); // ITU-R BT.601
static const float DOT_SIZE_MULTIPLIER = 0.5f; // ドットサイズの倍率
static const float DOT_SMOOTHNESS = 0.1f; // ドットの境界のぼかし
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // --- UV座標をグリッドに分割 ---
    // ドットサイズに応じて画面を格子状に分割
    float2 gridUV = PSInput.uv * g_param.x;
    
    // グリッドのセル番号（整数部分）
    float2 gridCell = floor(gridUV);
    
    // セル内のローカル座標（小数部分、0.0～1.0）
    float2 gridLocal = frac(gridUV);
    
    // --- セルの中心からサンプリング ---
    // 各セルを代表する色として中心の色を使用
    float2 sampleUV = (gridCell + 0.5f) / g_param.x;
    float4 srcCol = tex.Sample(texSampler, sampleUV);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- ドットの形状を生成 ---
    // セルの中心を原点とする座標系に変換
    float2 dotCenter = gridLocal - 0.5f;
    
    // 中心からの距離を計算（円形のドット）
    float dotDist = length(dotCenter);
    
    // --- 輝度に応じてドットサイズを変化させる ---
    // 明るい部分: 大きなドット（濃い印刷）
    // 暗い部分: 小さなドット（薄い印刷）
    // これにより新聞や雑誌の網点印刷を再現
    float luminance = dot(srcCol.rgb, LUMINANCE_WEIGHTS);
    float dotSize = luminance * g_param.y * DOT_SIZE_MULTIPLIER;
    
    // --- ドットマスクを生成 ---
    // smoothstep関数で滑らかな円形マスクを生成
    // dotDist < dotSize: dot = 1.0（ドット内）
    // dotDist > dotSize + DOT_SMOOTHNESS: dot = 0.0（ドット外）
    float dot = smoothstep(dotSize + DOT_SMOOTHNESS, dotSize, dotDist);
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    // ドットマスクを適用（ドット内のみ色を表示）
    dstCol.rgb = srcCol.rgb * dot;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}