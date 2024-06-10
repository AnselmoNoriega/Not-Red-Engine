
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
    matrix lwvp;
    matrix lfwvp;
    matrix world;
    float3 viewPos;
}
cbuffer SettingsBuffer : register(b1)
{
    bool useDiffuseMap;
    bool useSpecMap;
    bool useShadowMap;
    float depthBias;
}
cbuffer LightBuffer : register(b2)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}
cbuffer MaterialBuffer : register(b3)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

Texture2D diffuseMap : register(t0);
Texture2D shadowMap : register(t1);
Texture2D specMap : register(t2);
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
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 lightFarNDCPosition : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float3 localPos = input.position;
    localPos.yz += sin(input.position.x * 2) * 0.5;
    localPos.x += sin(input.position.z * 2) * 0.5;
    VS_OUTPUT output;
    
    float3 worldPos = mul(float4(localPos, 1.0f), world).xyz;
    output.position = mul(float4(localPos, 1.0f), wvp);
    output.worldNormal = mul(input.noraml, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPos - worldPos);
    
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(localPos, 1.0f), lwvp);
        output.lightFarNDCPosition = mul(float4(localPos, 1.0f), lfwvp);
    }
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    float3 n = normalize(input.worldNormal);
        
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    float4 emissive = materialEmissive;
        
    float4 ambient = lightAmbient * materialAmbient;
    
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;
    
    float4 diffuseMapColor = (useDiffuseMap) ? diffuseMap.Sample(textureSampler, input.texCoord) : 1.0f;
    float4 specMapColor = (useSpecMap) ? specMap.Sample(textureSampler, input.texCoord).r : 1.0f;
    
    finalColor = (ambient + diffuse + emissive) * diffuseMapColor + (specular * specMapColor);
        
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - (shadowUV.y + 1.0f) * 0.5f;
        if (saturate(u) == u && saturate(v) == v)
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;
            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (ambient + materialEmissive) * diffuseMapColor;
            }
        }
        else
        {
            actualDepth = 1.0f - (input.lightFarNDCPosition.z / input.lightFarNDCPosition.w);
            shadowUV = input.lightFarNDCPosition.xy / input.lightFarNDCPosition.w;
            u = (shadowUV.x + 1.0f) * 0.5f;
            v = 1.0f - (shadowUV.y + 1.0f) * 0.5f;
            if (saturate(u) == u && saturate(v) == v)
            {
                float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
                float savedDepth = savedColor.r;
                if (savedDepth > actualDepth + depthBias)
                {
                    finalColor = (ambient + materialEmissive) * diffuseMapColor;
                }
            }
        }
    }
    return finalColor;
}

[maxvertexcount(3)]
void GS(triangle VS_OUTPUT input[3], inout TriangleStream<VS_OUTPUT> output)
{
    for (int i = 0; i < 3; ++i)
    {
        output.Append(input[i]);
    }
}