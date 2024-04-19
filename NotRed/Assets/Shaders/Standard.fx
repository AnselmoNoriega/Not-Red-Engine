
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
}
cbuffer SettingsBuffer : register(b1)
{
    bool useDiffuseMap;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 noraml : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    if (useDiffuseMap)
    {
        return textureMap.Sample(textureSampler, input.texCoord);
    }
    
    return float4(1.0f, 0.5, 0.5f, 1.0f);
}