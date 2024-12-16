
cbuffer PerFrame : register(b0)
{
    matrix viewMatrix;
    
    matrix invViewMatrix;
    matrix invProjectionMatrix;
    matrix invModelMatrix;
    
    float3 cameraPosition;
}

cbuffer RayMarching : register(b1)
{
    float stepSize;
    float densityMultiplier;
    float lightIntensity;
}

cbuffer LightData : register(b2)
{
    float3 lightColor;
    float padding;
    float3 moveDirection;
}

Texture2D baseColorTexture : register(t0); // Object's base color
Texture2D depthTexture : register(t1); // Object's depths
Texture2D normalsTexture : register(t2); // Object's normals
Texture2D lightInGeometryTexture : register(t3); // Back-face depth
Texture2D lightGeometryTexture : register(t4); // Front-face depth

SamplerState samplerState : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    
    return output;
}

// Functions for 3D noise =================================================
// ========================================================================
// ========================================================================
float3 FadeCurve(float3 t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float Hash(float3 p) // Point
{
    p = frac(p * 0.3183099 + 0.1);
    p *= 17.0;
    return frac(p.x * p.y * p.z * (p.x + p.y + p.z));
}

float Noise3D(float3 p)
{
    float3 i = floor(p);
    float3 f = frac(p);

    float3 u = FadeCurve(f);

    return lerp(
        lerp(
            lerp(Hash(i + float3(0.0, 0.0, 0.0)), Hash(i + float3(1.0, 0.0, 0.0)), u.x),
            lerp(Hash(i + float3(0.0, 1.0, 0.0)), Hash(i + float3(1.0, 1.0, 0.0)), u.x),
            u.y),
        lerp(
            lerp(Hash(i + float3(0.0, 0.0, 1.0)), Hash(i + float3(1.0, 0.0, 1.0)), u.x),
            lerp(Hash(i + float3(0.0, 1.0, 1.0)), Hash(i + float3(1.0, 1.0, 1.0)), u.x),
            u.y),
        u.z);
}
// ========================================================================
// ========================================================================

// Functions for dust density =============================================
// ========================================================================
// ========================================================================
float SampleDensity(float inputDensity)
{
    // Smooth adjustment curve
    float density = pow(inputDensity, 2.0);

    // Additional noise for subtle variation
    float3 noiseCoord = float3(inputDensity * 12.34, inputDensity * 56.78, inputDensity * 90.12);
    float extraNoise = frac(sin(dot(noiseCoord, float3(12.9898, 78.233, 37.719))) * 43758.5453);
    density += extraNoise * 0.05; // Add a small noise variation

    return saturate(density);
}

float FractalNoise3D(float3 p, int octaves, float lacunarity, float gain)
{
    float frequency = 1.0;
    float amplitude = 1.0;
    float total = 0.0;
    float maxValue = 0.0; // Used for normalization

    for (int i = 0; i < octaves; ++i)
    {
        total += Noise3D(p * frequency) * amplitude;
        maxValue += amplitude;

        frequency *= lacunarity;
        amplitude *= gain;
    }

    return total / maxValue; // Normalize the result
}

float Turbulence3D(float3 p, int octaves, float lacunarity, float gain)
{
    float frequency = 1.0;
    float amplitude = 1.0;
    float total = 0.0;

    for (int i = 0; i < octaves; ++i)
    {
        total += abs(Noise3D(p * frequency) * 2.0 - 1.0) * amplitude;
        frequency *= lacunarity;
        amplitude *= gain;
    }

    return total;
}
// ========================================================================
// ========================================================================

// Function for light scattering ==========================================
// ========================================================================
// ========================================================================
float ComputeScattering(float3 lightDir, float3 viewDir, float density)
{
    // Henyey-Greenstein phase function for anisotropic scattering
    float g = 0.5; // Anisotropy factor (-1: backward, 0: isotropic, 1: forward)
    float phase = (1.0 - g * g) / pow(max(1.0e-6, 1.0 + g * g - 2.0 * g * dot(lightDir, viewDir)), 1.5);

    // Scattering contribution is proportional to density and phase
    float scattering = density * phase;
    
    return saturate(scattering);
}
// ========================================================================
// ========================================================================

float CloudNoise(float3 p)
{
    // Combine fractal noise and turbulence for cloud-like patterns
    float fractal = FractalNoise3D(p, 5, 2.0, 0.5); // 5 octaves
    float turbulent = Turbulence3D(p, 4, 2.0, 0.6); // 4 octaves

    return lerp(fractal, turbulent, 0.5); // Blend fractal and turbulence
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 frontDepthEncoded = lightGeometryTexture.Sample(samplerState, input.texCoord);
    float4 backDepthEncoded = lightInGeometryTexture.Sample(samplerState, input.texCoord);
    float4 depthEncoded = depthTexture.Sample(samplerState, input.texCoord);
    
    // Get volume thickness from vewPoint
    float inOutDistance = length(frontDepthEncoded.xyz - backDepthEncoded.xyz);
    
    // Decode to get view-space positions
    float3 entryViewPos = frontDepthEncoded.xyz * 100.0f;
    float3 exitViewPos = backDepthEncoded.xyz * 100.0f;

    // Ensure entry and exit positions are valid
    bool objectOutsideVolume = length(depthEncoded.xyz) + 0.001 < length(frontDepthEncoded.xyz) && depthEncoded.a > 0.001;
    if (backDepthEncoded.a < 0.001 || objectOutsideVolume)
    {
        return baseColorTexture.Sample(samplerState, input.texCoord);
    }

    // Transform to world-space
    float3 entryWorldPos = mul(float4(entryViewPos, 1.0f), invViewMatrix).xyz;
    float3 exitWorldPos = mul(float4(exitViewPos, 1.0f), invViewMatrix).xyz;

    // Perform ray marching
    float3 rayDir = normalize(exitWorldPos - entryWorldPos);
    float rayLength = length(exitWorldPos - entryWorldPos);
    float3 currentPos = entryWorldPos;
    float scattering = 0.0;

    for (float t = 0.0f; t < rayLength; t += stepSize)
    {
        float density = CloudNoise(currentPos + moveDirection);
        scattering += ComputeScattering(-rayDir, rayDir, density) * density * lightIntensity * stepSize;
        currentPos += rayDir * stepSize;
    }
    
    // Decrese light strength depending on suroundings
    float objInterferingValue = 1;
    if (backDepthEncoded.a > 0.001 && depthEncoded.a > 0.001)
    {
        objInterferingValue = 1.5;
    }
    
    // Combine scattering with base color
    float4 baseColor = baseColorTexture.Sample(samplerState, input.texCoord);
    float lightEffect = exp(-densityMultiplier * scattering);
    return lerp(baseColor, float4(lightColor, 1.0), (1.0 - lightEffect) * inOutDistance * 10 / objInterferingValue);
}