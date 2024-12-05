cbuffer TransformBuffer : register(b0)
{
    float4x4 modelTransform;
    float4x4 viewMatrix;
    float4x4 viewProjectionMatrix;
};

struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 linearDepth : TEXCOORD0;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    
    float4 worldPosition = mul(modelTransform, float4(input.position, 1.0f));
    float4 viewPosition = mul(viewMatrix, worldPosition);
    output.linearDepth = viewPosition.xyz;
    output.position = mul(viewProjectionMatrix, worldPosition);

    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    float3 depth = input.linearDepth / 100.0f;
    return float4(depth, 1.0);
}