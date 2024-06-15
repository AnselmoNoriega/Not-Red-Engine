cbuffer MaterialBuffer : register(b0)
{
    float3 lightDir;
    float4 lightColor;
}

Texture2D water : register(t0);
Texture2D targets : register(t1);
Texture2D depth : register(t2);
Texture2D waterDepth : register(t3);
Texture2D foamTexture : register(t4);

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

cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 objectDist = depth.Sample(textureSampler, input.texCoord);
    float4 waterDist = waterDepth.Sample(textureSampler, input.texCoord);
   
    if (objectDist.x < waterDist.x)
    {
        float3 waterDirection = water.Sample(textureSampler, input.texCoord).xyz;
        float2 offset = waterDirection.xz * 0.1f;
        
        float dif = max(dot(waterDirection, -lightDir), 0.0f);
        float ambient = 0.2;
        
        float2 refractedUV = input.texCoord + offset;
        float4 objectDist2 = depth.Sample(textureSampler, refractedUV);
        float4 waterDist2 = waterDepth.Sample(textureSampler, refractedUV);
        float4 color = float4(0.5, 0.9, 1, 1);
        float blendFactor = 0.5;
        
        if (objectDist2.x > waterDist2.x)
        {
            float4 ObjectColor = targets.Sample(textureSampler, input.texCoord);
            color = lerp(color, ObjectColor, blendFactor);
            float4 col = float4(lightColor.xyz * (dif + ambient), 1);
            col = lerp(col, float4(1, 1, 1, 1), 0.1);
            return color * col;
        }
        else
        {
            float4 ObjectColor = targets.Sample(textureSampler, refractedUV);
    
            color = lerp(color, ObjectColor, blendFactor);
            if (objectDist.a != 0.0f && waterDist.x - objectDist.x <= 0.01)
            {
                color = foamTexture.Sample(textureSampler, input.texCoord);
            }
            float4 col = float4(lightColor.xyz * (dif + ambient), 1);
            col = lerp(col, float4(1, 1, 1, 1), 0.1);
            return color * col;
        }
    }
    else
    {
        float4 ObjectColor = targets.Sample(textureSampler, input.texCoord);
        return ObjectColor;
    }
    
}