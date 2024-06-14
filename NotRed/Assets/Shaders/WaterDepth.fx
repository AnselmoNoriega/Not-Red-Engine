cbuffer Transform : register(b0)
{
    matrix viewProjection;
    float3 camPos;
    float nearPlane;
    float farPlane;
}

cbuffer WaterBuffer : register(b1)
{
    float4 wavePattern[3];
    float waveMovementTime;
    float waveStrength;
}

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float depth : TEXCOORD0;
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

VS_OUTPUT VS(float3 position : POSITION)
{
    float4 viewPosition = mul(float4(position, 1.0f), viewProjection);
    float linearDepth = viewPosition.z / viewPosition.w;
    float normalizedDepth = (linearDepth - nearPlane) / (farPlane - nearPlane);
    
    //Water shape
    float3 p = position;
    for (int i = 0; i < 3; ++i)
    {
        p += GerstnerWave(wavePattern[i], position * waveStrength);
    }
    
    VS_OUTPUT output;
    output.depth = saturate(normalizedDepth);
    output.position = mul(float4(p, 1.0f), viewProjection);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(input.depth, input.depth, input.depth, 1.0f);
}