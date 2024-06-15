
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
    matrix lwvp;
    matrix lfwvp;
    matrix world;
    float3 viewPos;
}
cbuffer MaterialBuffer : register(b3)
{
    float4 materialAmbient;
    float4 materialDiffuse;
}
cbuffer WaterBuffer : register(b4)
{
    float4 wavePattern[4];
    float waveMovementTime;
    float waveStrength;
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
};

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

float3 GerstnerWave(float4 wave, float3 p)
{
    float steepness = wave.z;
    float wavelength = wave.w;
    float k = 2 * 3.14159265 / wavelength;
    float c = sqrt(9.8 / k);
    float2 d = normalize(wave.xy);
    float f = k * (dot(d, p.xz) - c * waveMovementTime);
    float a = steepness / k;
    
    return float3(d.x * (a * cos(f)), a * sin(f), d.y * (a * cos(f)));
}

VS_OUTPUT VS(VS_INPUT input)
{
    float3 p = input.position;
    for (int i = 0; i < 4; ++i)
    {
        p += GerstnerWave(wavePattern[i], input.position * waveStrength);
    }
    
    VS_OUTPUT output;
    output.position = mul(float4(p, 1.0f), wvp);
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
    float ratio = 1.00 / 1.33;
    float3 I = normalize(gsInput.position.xyz - viewPos);
    float3 R = refract(I, gsInput.normal, ratio);
    
    float4 color;
    //color = vec4(texture(skybox, R).rgb, 1.0);
    
    color.xyz = gsInput.normal;
    color.a = 0.5f;
    return color;
}