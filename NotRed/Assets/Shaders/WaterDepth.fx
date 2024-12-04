cbuffer TransformBuffer : register(b0)
{
    float4x4 modelTransform;
    float4x4 viewMatrix;
    float4x4 viewProjectionMatrix;
};

cbuffer WaterBuffer : register(b1)
{
    float4 wavePattern[4];
    float waveMovementTime;
    float waveStrength;
}

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float linearDepth : TEXCOORD0;
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
    //Water shape
    float3 p = position;
    for (int i = 0; i < 4; ++i)
    {
        p += GerstnerWave(wavePattern[i], position * waveStrength);
    }
    
    VS_OUTPUT output;
    
    float4 worldPosition = mul(modelTransform, float4(p, 1.0f));
    float4 viewPosition = mul(viewMatrix, worldPosition);
    output.linearDepth = viewPosition.z;
    output.position = mul(viewProjectionMatrix, worldPosition);
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float depth = 1.0 - saturate(input.linearDepth / 100.0f);
    return float4(depth, depth, depth, 1.0);
}