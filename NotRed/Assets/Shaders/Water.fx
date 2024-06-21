
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
    float3 basePosition : TEXCOORD0;
    float3 newPosition : TEXCOORD1;
};

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL0;
    float3 basePosition : TEXCOORD0;
    float3 newPosition : TEXCOORD1;
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
    
    VS_OUTPUT output;
    output.basePosition = p;
    
    for (int i = 0; i < 4; ++i)
    {
        p += GerstnerWave(wavePattern[i], input.position * waveStrength);
    }
    
    output.newPosition = p;
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
        outputData.basePosition = input[i].basePosition;
        outputData.newPosition = input[i].newPosition;
        output.Append(outputData);
    }
    output.RestartStrip();
}

float4 PS(GS_OUTPUT gsInput) : SV_Target
{
    float3 lighdir = normalize(float3(1.0f, -1.0f, 1.0f));
    float4 lighCol = float4(1, 0.97, 0.97, 1.0f);
    
    float dif = max(dot(gsInput.normal, -lighdir), 0.0f);
    float ambient = 0.4;
    
    float4 col = float4(lighCol.xyz * (dif + ambient), 1);
    
    float4 color = float4(0.305, 0.862, 0.854, 1);
    
    float4 distance = float4(gsInput.newPosition - gsInput.basePosition, 1);
    color = lerp(color, float4(1, 1, 1, 1), ((distance.r + distance.g) / 2) / 2);
    color.a = 1;
    
    return color * col;
}