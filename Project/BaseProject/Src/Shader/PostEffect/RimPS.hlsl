// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    //float3 g_light_dir;
    //float3 g_camera_pos;
    //float4 g_diff_color;
    //float4 g_specular_color;

}


float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color = diffuseMapTexture.Sample(
        diffuseMapSampler,
        PSInput.uv
    );

    if (color.a < 0.01f)
    {
        discard;
    }

    // グレースケール
    float gray = dot(
        color.rgb,
        float3(0.3f, 0.6f, 0.12f)
    );

    color.rgb = float3(gray, gray, gray);

    // マテリアルカラー
    color *= PSInput.diffuse;

    return color;
   

}
