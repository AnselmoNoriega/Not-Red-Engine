
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float param0;
    float param1;
    float param2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
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
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    switch (mode)
    {
        case 0:
        {
            finalColor = textureMap0.Sample(textureSampler, input.texCoord);
            break;
        }
        case 1:
        {
            finalColor = textureMap0.Sample(textureSampler, input.texCoord);
            finalColor = (finalColor.x + finalColor.y + finalColor.z) / 3;
            break;
        }
        case 2:
        {
            finalColor = 1.0f - textureMap0.Sample(textureSampler, input.texCoord);
            break;
        }
        case 3:
        {
            float2 texCoord = input.texCoord;
            texCoord.x *= param0;
            texCoord.y *= param1;
                finalColor = textureMap0.Sample(textureSampler, texCoord);
            break;
        }
        case 4:
        {
            float u = input.texCoord.x;
            float v = input.texCoord.y;
            finalColor =
            textureMap0.Sample(textureSampler, float2(u, v)) +
            textureMap0.Sample(textureSampler, float2(u + param0, v)) +
            textureMap0.Sample(textureSampler, float2(u - param0, v)) +
            textureMap0.Sample(textureSampler, float2(u, v + param1)) +
            textureMap0.Sample(textureSampler, float2(u, v - param1)) +
            textureMap0.Sample(textureSampler, float2(u + param0, v + param1)) +
            textureMap0.Sample(textureSampler, float2(u + param0, v - param1)) +
            textureMap0.Sample(textureSampler, float2(u - param0, v + param1)) +
            textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
            finalColor *= 0.111111f;

        }
        break;
        case 5:
        {
            float4 finalColor1 = textureMap0.Sample(textureSampler, input.texCoord);
            float4 finalColor2 = textureMap1.Sample(textureSampler, float2(input.texCoord.x + param0, input.texCoord.y));
            finalColor = (finalColor1 + finalColor2) / 2;
        }
        break;
        case 7:
        {
                float2 distortion = float2(param0, -param1);
                float4 redChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
                float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
                float4 blueChannel = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
                finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
            }
        break;
    }
    
    return finalColor;
}