cbuffer Transform : register(b0)
{
    matrix wvp;
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
    
    // NDC space, 0-1 in the Z direction
    output.position = mul(float4(position, 1.0f), wvp);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float linearDepth = input.position.z / input.position.w;
    
    input.depth = saturate(linearDepth);
    
    return float4(input.depth, input.depth, input.depth, 1.0f);
} 