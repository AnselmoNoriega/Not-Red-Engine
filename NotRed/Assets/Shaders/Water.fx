
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
    matrix lwvp;
    matrix lfwvp;
    matrix world;
    float3 viewPos;
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
}
cbuffer WaterBuffer : register(b4)
{
    float waveHeight;
    float waveStrength;
    float waveMovementTime;
}

Texture2D diffuseMap : register(t0);
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
    float4 position : POSITION;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float2 texCoord : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 lightFarNDCPosition : TEXCOORD4;
};

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 localPos = input.position;
    localPos.yz += sin((input.position.x * waveStrength) + waveMovementTime) * waveHeight;
    localPos.x += cos((input.position.z * waveStrength) + waveMovementTime) * waveHeight;
    
    float3 worldPos = mul(float4(localPos, 1.0f), world).xyz;
    output.position = mul(float4(localPos, 1.0f), wvp);
    output.worldNormal = mul(input.noraml, (float3x3) world);
    output.worldTangent = mul(input.tangent, (float3x3) world);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPos - worldPos);
    
    return output;
}

[maxvertexcount(3)]
void GS(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> output)
{
    float3 AB = input[1].position - input[0].position;
    float3 AC = input[2].position - input[0].position;
    
    for (int i = 0; i < 3; ++i)
    {
        GS_OUTPUT outputData;
        outputData.position = input[i].position;
        outputData.normal = normalize(cross(AB, AC));
        output.Append(outputData);
    }
    output.RestartStrip();
}

float4 PS(GS_OUTPUT gsInput) : SV_Target
{
    float4 color;
    color.xyz = gsInput.normal;
    color.a = 0.5f;
    return color;
}