#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// ========================================
// 定数バッファ: 渦巻きエフェクトのパラメータ
// ========================================
cbuffer cbParam : register(b4)
{
    float4 g_param;
    // x: 時間 (time) - アニメーション用の経過時間
    // y: 強度 (strength) - 渦巻きの回転の強さ
    // z: 半径 (radius) - 渦巻きの影響範囲
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 中心点の設定
    float2 center = float2(0.5f, 0.5f);
    
    // 現在のピクセルから中心へのベクトルを計算
    float2 toCenter = PSInput.uv - center;
    
    // 中心からの距離を計算
    // length()関数で中心からの距離を取得
    // 距離が大きいほど渦巻きの効果が強くなる
    float dist = length(toCenter);
    
    // 回転角度の計算
    // 渦巻きの角度を計算:
    // - dist * g_param.z: 距離に応じた基本角度(外側ほど大きく回転)
    // - g_param.x: 時間による回転のオフセット(アニメーション用)
    float angle = dist * g_param.z + g_param.x;
    
    // 強度(g_param.y)を掛けて最終的な回転角度を決定
    // sin/cosで回転行列を作成
    float s = sin(angle * g_param.y);
    float c = cos(angle * g_param.y);
    
    // 回転行列の作成と適用
    // 2D回転行列:
    // | cos(θ)  -sin(θ) |
    // | sin(θ)   cos(θ) |
    float2x2 rotation = float2x2(
        c, -s, // 1行目: cosθ, -sinθ
        s, c // 2行目: sinθ,  cosθ
    );
    
    // 中心からのベクトルに回転行列を適用
    // これにより、ピクセルが中心を軸に回転する
    float2 rotated = mul(toCenter, rotation);
    
    // 回転後のUV座標を計算
    // 回転後の相対座標に中心座標を足して、
    // 元のUV座標系に戻す
    float2 uv = center + rotated;
    
    // テクスチャからカラーをサンプリング
    // 回転後のUV座標を使ってテクスチャから色を取得
    // これにより、画像が渦巻き状に歪む効果が得られる
    float4 srcCol = tex.Sample(texSampler, uv);
    
    
    // 透明ピクセルの除外
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    return srcCol;
}

// ========================================
// 【処理の全体像】
// ========================================
// 1. 各ピクセルについて、画面中心からの距離を計算
// 2. 距離に応じて回転角度を決定(外側ほど大きく回転)
// 3. その角度でピクセル座標を回転
// 4. 回転後の座標からテクスチャをサンプリング
// → 結果として画面が渦巻き状に歪む
//
// 【パラメータの使い方】
// g_param.x (時間): 大きくすると全体が回転
// g_param.y (強度): 大きくすると渦巻きが強くなる
// g_param.z (半径): 大きくすると渦巻きの範囲が広がる
// ========================================