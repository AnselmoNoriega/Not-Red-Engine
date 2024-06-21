cbuffer RefractionHelper : register(b0)
{
    float3 lightDir;
    float4 lightColor;
    float time;
};

Texture2D water : register(t0);
Texture2D waterNormal : register(t1);
Texture2D waterDepth : register(t2);
Texture2D targets : register(t3);
Texture2D targetsDepth : register(t4);
Texture2D foamTexture : register(t5);

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
    float4 wDepth = waterDepth.Sample(textureSampler, input.texCoord);
    float4 tDepth = targetsDepth.Sample(textureSampler, input.texCoord);
    
    if (wDepth.x > tDepth.x)
    {
        float2 wNormal = waterNormal.Sample(textureSampler, input.texCoord).rb * 0.1f * wDepth.x; //check with b-g
        
        float4 wDistortedDepth = waterDepth.Sample(textureSampler, input.texCoord + wNormal);
        float4 tDistortedDepth = targetsDepth.Sample(textureSampler, input.texCoord + wNormal);
        
        float4 targetsColor;
        if (wDistortedDepth.x < tDistortedDepth.x)
        {
            targetsColor = targets.Sample(textureSampler, input.texCoord);
        }
        else
        {
            targetsColor = targets.Sample(textureSampler, input.texCoord + wNormal);
        }
        
        float4 waterColor = water.Sample(textureSampler, input.texCoord);
        return lerp(waterColor, targetsColor, 0.5);
    }
    else
    {
        return targets.Sample(textureSampler, input.texCoord);
    }

}