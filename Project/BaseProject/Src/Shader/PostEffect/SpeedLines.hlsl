#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// 定数バッファ: 集中線エフェクトのパラメータ
cbuffer cbParam : register(b4)
{
    float4 g_param; // x: 時間（アニメーション用）, y: ライン密度（本数）, z: スピード（回転速度）
}

// 集中線効果の定数
static const float2 CENTER = float2(0.5f, 0.5f); // 集中線の中心点
static const float FADE_INNER = 0.2f; // フェードの開始距離
static const float FADE_OUTER = 0.8f; // フェードの終了距離
static const float LINE_INTENSITY = 0.7f; // 集中線の強度
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- 中心からの方向と距離を計算 ---
    float2 toCenter = PSInput.uv - CENTER;
    
    // atan2関数で角度を計算（-π ～ π の範囲）
    // この角度を使って放射状のパターンを生成
    float angle = atan2(toCenter.y, toCenter.x);
    
    // 中心からの距離
    float dist = length(toCenter);
    
    // --- 放射状のラインパターンを生成 ---
    // sin関数で周期的なパターンを作成
    // angle * g_param.y: 角度にライン密度を掛けて本数を調整
    // g_param.x * g_param.z: 時間とスピードで回転アニメーション
    float linePattern = sin(angle * g_param.y - g_param.x * g_param.z);
    
    // step関数で二値化（白黒のはっきりしたライン）
    // linePattern >= 0.0 なら 1.0（白いライン）
    // linePattern < 0.0 なら 0.0（ライン間の隙間）
    linePattern = step(0.0f, linePattern);
    
    // --- 中心からの距離で効果を調整 ---
    // 中心付近は集中線を表示せず、離れるほど濃くなる
    // smoothstep関数で滑らかなグラデーションを生成
    // 
    // FADE_INNER（0.2）より近い: fade = 0.0（集中線なし）
    // FADE_OUTER（0.8）より遠い: fade = 1.0（集中線最大）
    // その間: 滑らかに補間
    float fade = smoothstep(FADE_INNER, FADE_OUTER, dist);
    
    // ラインパターンにフェードと強度を適用
    float speedLine = linePattern * fade * LINE_INTENSITY;
    
    // --- 最終的な色の合成 ---
    float4 dstCol;
    
    // 集中線は元の画像を暗くし、ライン部分を白くする効果
    // (1.0f - speedLine): ライン部分以外を暗くする（反転）
    // + speedLine: ライン部分を明るくする
    dstCol.rgb = srcCol.rgb * (1.0f - speedLine) + speedLine;
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}