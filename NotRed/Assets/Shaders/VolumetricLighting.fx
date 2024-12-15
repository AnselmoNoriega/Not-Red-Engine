
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
}

Texture2D baseColorTexture : register(t0); // Object's base color
Texture2D depthTexture : register(t1); // Object's depths
Texture2D normalsTexture : register(t2); // Object's normals
Texture2D lightGeometryTexture : register(t3); // Front-face depth
Texture2D lightInGeometryTexture : register(t4); // Back-face depth

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

float3 GetWorldPosition(float3 viewSpacePosition, matrix invViewMatrix, matrix invModelMatrix)
{
    // Convert to world-space position
    float4 worldSpacePosition = mul(float4(viewSpacePosition, 1.0), invViewMatrix);

    // Convert to object-space position (original position)
    float4 originalPosition = mul(worldSpacePosition, invModelMatrix);

    return originalPosition.xyz;
}

// Functions for 3D noise =================================================
// ========================================================================
// ========================================================================
float3 FadeCurve(float3 t)
{
    return t * t * (3.0 - 2.0 * t); // Smooth fade curve
}

float Hash(float3 p) // Point
{
    return frac(sin(dot(p, float3(12.9898, 78.233, 45.543))) * 43758.5453);
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

float GetProceduralDensity(float3 coords)
{
    float noiseValue = Noise3D(coords * 0.1); // Scale the coordinates for the noise
    return SampleDensity(noiseValue); // Apply the density function
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
float4 LitTexture(float4 color, float2 texCoord)
{
    return color;
}

float3 ReconstructViewSpacePosition(float2 uv, float depth, matrix invProjectionMatrix)
{
    // Clip-space position
    float4 clipSpacePosition = float4(uv * 2.0f - 1.0f, depth, 1.0f);

    // View-space position
    float4 viewSpacePosition = mul(clipSpacePosition, invProjectionMatrix);
    return viewSpacePosition.xyz / viewSpacePosition.w;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 frontDepthEncoded = lightGeometryTexture.Sample(samplerState, input.texCoord);
    float4 backDepthEncoded = lightInGeometryTexture.Sample(samplerState, input.texCoord);

    // Decode to get view-space positions
    float3 entryViewPos = frontDepthEncoded.xyz * 100.0f;
    float3 exitViewPos = backDepthEncoded.xyz * 100.0f;

    // Ensure entry and exit positions are valid
    if (backDepthEncoded.a < 0.001)
    {
        return baseColorTexture.Sample(samplerState, input.texCoord);
    }
    if (frontDepthEncoded.a < 0.001)
    {
        entryViewPos = float3(0, 0, 0);
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
        float density = GetProceduralDensity(currentPos);
        scattering += ComputeScattering(-rayDir, rayDir, density) * density * lightIntensity * stepSize;
        currentPos += rayDir * stepSize;
    }

    // Combine scattering with base color
    float4 baseColor = baseColorTexture.Sample(samplerState, input.texCoord);
    float lightEffect = exp(-densityMultiplier * scattering);
    return lerp(baseColor, float4(lightColor, 1.0), 1.0 - lightEffect);
}