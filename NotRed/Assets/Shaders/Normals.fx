cbuffer TransformBuffer : register(b0)
{
    float4x4 modelTransform;
    float4x4 viewMatrix;
    float4x4 viewProjectionMatrix;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : TEXCOORD0;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    
    float4 worldPosition = mul(modelTransform, float4(input.position, 1.0f));
    
    output.color = input.normal;
    output.position = mul(viewProjectionMatrix, worldPosition);

    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    return float4(input.color, 1.0);
}