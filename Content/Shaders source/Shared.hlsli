

struct VSout
{
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float4 pos : SV_POSITION;
};

SamplerState g_engineSampler : register(s0);
SamplerState g_nearestClampSampler : register(s1);
SamplerState g_linearClampSampler : register(s2);