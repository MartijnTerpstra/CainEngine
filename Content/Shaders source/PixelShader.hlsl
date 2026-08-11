#include "Shared.hlsli"

Texture2D g_texture : register(t0);

float4 main(VSout input) : SV_TARGET
{
    return float4(input.uv, 1, 1); //g_texture.Sample(g_engineSampler, input.uv);

}
