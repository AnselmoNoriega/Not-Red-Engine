
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

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 basePosition : POSITION0;
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

VS_OUTPUT VS(float3 pos : POSITION)
{
    VS_OUTPUT output;
    output.basePosition = mul(float4(pos, 1.0f), wvp);

    float3 p = pos;
    for (int i = 0; i < 4; ++i)
    {
        p += GerstnerWave(wavePattern[i], pos * waveStrength);
    }

    output.position = mul(float4(p, 1.0f), wvp);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color;
    color.xyz = 0.5;
    color.a = 1.0f;
    return color;
}