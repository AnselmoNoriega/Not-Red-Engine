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
    float depth : TEXCOORD0;
};

VS_OUTPUT VS(float3 position : POSITION)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(position, 1.0f), viewProjection);
    float linearDepth = output.position.z / output.position.w;
    float normalizedDepth = (linearDepth - nearPlane) / (farPlane - nearPlane);
    
    output.depth = saturate(0);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.depth, input.depth, input.depth, 1.0f);
} 