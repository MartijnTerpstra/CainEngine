
struct VsIn
{
    float2 Position : POSITION;
};

struct VsOut
{
    float4 Position : SV_Position;
};

VsOut VS_main(VsIn input)
{
    VsOut output;

    output.Position = float4(input.Position, 0, 1);
}

float4 PS_main(VsOut input) : COLOR
{
    return float4(1, 1, 1, 1);
}