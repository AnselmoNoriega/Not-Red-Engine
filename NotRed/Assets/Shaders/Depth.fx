cbuffer CameraBuffer : register(b0)
{
    matrix wvp;
};

struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    output.position = mul(float4(input.position, 1.0), wvp);
    return output;
}