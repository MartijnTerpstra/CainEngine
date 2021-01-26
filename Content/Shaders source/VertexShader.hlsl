#include "Shared.hlsli"

VSout main(float3 pos : POSITION, float2 uv : TEXCOORD0)
{
	VSout output;

	output.pos = float4(pos, 1);
	output.normal = float3(0, 0, 1);
	output.uv = uv;

	return output;
}