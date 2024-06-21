
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
}

cbuffer WaterBuffer : register(b1)
{
    float4 wavePattern[4];
    float waveMovementTime;
    float waveStrength;
}

Texture2D normals : register(t0);
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
    VS_OUTPUT output;

    float3 p = input.position;
    for (int i = 0; i < 4; ++i)
    {
        p += GerstnerWave(wavePattern[i], input.position * waveStrength);
    }
    
    output.position = mul(float4(p, 1.0f), wvp);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return normals.Sample(textureSampler, input.texCoord * 10);
}