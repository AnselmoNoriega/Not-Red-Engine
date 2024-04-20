
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewPos;
}
cbuffer SettingsBuffer : register(b1)
{
    bool useDiffuseMap;
}
cbuffer LightBuffer : register(b2)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
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
    float3 worldNormal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    float3 worldPos = mul(float4(input.position, 1.0f), world);
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.noraml, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPos - worldPos);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    float4 ambient = lightAmbient;
    
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse;
    
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, 10);
    float4 specular = s * lightSpecular;
    
    float4 diffuseMapColor = (useDiffuseMap) ? textureMap.Sample(textureSampler, input.texCoord) : 1.0f;
    
    float4 finalColor = (ambient + diffuse) * diffuseMapColor + specular;
    
    return finalColor;
}