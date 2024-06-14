cbuffer Transform : register(b0)
{
    matrix viewProjection;
    float3 camPos;
    float nearPlane;
    float farPlane;
}

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float depth : TEXCOORD0;
};

VS_OUTPUT VS(float3 position : POSITION)
{
    float4 viewPosition = mul(float4(position, 1.0f), viewProjection);
    float linearDepth = viewPosition.z / viewPosition.w;
    float normalizedDepth = (linearDepth - nearPlane) / (farPlane - nearPlane);
    
    VS_OUTPUT output;
    output.depth = saturate(normalizedDepth);
    output.position = mul(float4(position, 1.0f), viewProjection);
    output.color = float4(position - camPos, 1.0f);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.depth, input.depth, input.depth, 1.0f);
} 