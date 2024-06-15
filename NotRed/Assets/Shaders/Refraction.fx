cbuffer MaterialBuffer : register(b0)
{
    float3 lightDir;
}

Texture2D water : register(t0);
Texture2D targets : register(t1);
Texture2D depth : register(t2);
Texture2D waterDepth : register(t3);

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
    float4 objectDis = depth.Sample(textureSampler, input.texCoord);
    float4 waterDis = waterDepth.Sample(textureSampler, input.texCoord);
   
    if (objectDis.x < waterDis.x)
    {
        float4 waterDirection = water.Sample(textureSampler, input.texCoord);
        float2 offset = waterDirection.xz * 0.1f;
        
        float2 refractedUV = input.texCoord + offset;
        float4 objectDis2 = depth.Sample(textureSampler, refractedUV);
        float4 waterDis2 = waterDepth.Sample(textureSampler, refractedUV);
        float4 color = float4(0.5, 0.9, 1, 1);
        float blendFactor = 0.5;
        if (objectDis2.x > waterDis2.x)
        {
            float4 ObjectColor = targets.Sample(textureSampler, input.texCoord);
            color = lerp(color, ObjectColor, blendFactor);
            return color;
        }
        else
        {
            float4 ObjectColor = targets.Sample(textureSampler, refractedUV);
    
            color = lerp(color, ObjectColor, blendFactor);
            if (objectDis.a != 0.0f && waterDis.x - objectDis.x <= 0.01)
            {
                color = lerp(color, float4(1, 0, 0, 1), blendFactor);
            }
            return color;
        }
    }
    else
    {
        float4 ObjectColor = targets.Sample(textureSampler, input.texCoord);
        return ObjectColor;
    }
    //if(finalColor.a < 1.0f)
    //{
    //    float2 offset = finalColor.xz * 0.1f;
    //
    //    float2 refractedUV = input.texCoord + offset;
    //    
    //    color = textureMap0.Sample(textureSampler, refractedUV);
    //    
    //    float4 blueColor = float4(0.5, 0.9, 1, 1);
    //
    //    float blendFactor = 0.5;
    //    color = lerp(color, finalColor, blendFactor);
    //}
    
}