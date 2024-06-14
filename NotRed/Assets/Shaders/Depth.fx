cbuffer Transform : register(b0)
{
    matrix wvp;
    float3 camPos;
}

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT VS(float3 position : POSITION)
{
    VS_OUTPUT output;
    output.position = mul(float4(position, 1.0f), wvp);
    output.color = float4(position - camPos, 1.0f);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}