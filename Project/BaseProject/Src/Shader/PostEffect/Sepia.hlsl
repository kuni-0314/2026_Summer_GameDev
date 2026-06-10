#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// セピア調変換の標準的な変換行列
// 古い写真のような茶褐色の色調を再現
static const float3 SEPIA_R = float3(0.393f, 0.769f, 0.189f); // 新しいRチャンネルの係数
static const float3 SEPIA_G = float3(0.349f, 0.686f, 0.168f); // 新しいGチャンネルの係数
static const float3 SEPIA_B = float3(0.272f, 0.534f, 0.131f); // 新しいBチャンネルの係数

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元の色をサンプリング
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 完全に透明なピクセルは処理をスキップ
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    
    // --- セピア調変換 ---
    // RGB各チャンネルを線形変換して温かみのある茶褐色に変換
    // 各チャンネルは元のRGB値の重み付け和として計算される
    // ドット積を使用して効率的に計算（3回の乗算と2回の加算に最適化される）
    float4 dstCol;
    dstCol.r = dot(srcCol.rgb, SEPIA_R); // 赤みを強調
    dstCol.g = dot(srcCol.rgb, SEPIA_G); // 中間トーン
    dstCol.b = dot(srcCol.rgb, SEPIA_B); // 青みを抑制
    
    // アルファ値は元のまま保持
    dstCol.a = srcCol.a;
    
    return dstCol;
}