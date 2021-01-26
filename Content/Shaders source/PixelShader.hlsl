#include "Shared.hlsli"

Texture2D g_texture : register(t0);

float4 main(VSout input) : SV_TARGET
{
    return g_texture.Sample(g_engineSampler, input.uv);
}
