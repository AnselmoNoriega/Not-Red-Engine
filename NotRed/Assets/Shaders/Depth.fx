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
    float4 depth : TEXCOORD0;
};

VS_OUTPUT VS(float3 position : POSITION)
{
    VS_OUTPUT output;
    
    // NDC space, 0-1 in the Z direction
    output.position = mul(float4(position, 1.0f), wvp);
    return output;
}

float4 PackDepth(double depth)
{
    float4 packedDepth;
    packedDepth.x = frac(depth);
    packedDepth.y = frac(depth * 256.0);
    packedDepth.z = frac(depth * 65536.0);
    packedDepth.a = 1;
    return packedDepth;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    double linearDepth = input.position.z / input.position.w;
    linearDepth = saturate(linearDepth);
    input.depth = PackDepth(linearDepth);
    
    return input.depth;
} 