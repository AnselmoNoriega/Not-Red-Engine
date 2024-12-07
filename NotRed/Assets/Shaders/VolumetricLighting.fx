
cbuffer TranformBuffer : register(b0)
{
    matrix world;
    float3 viewDir;
    matrix lightViewProj;
    matrix viewMatrix;
}

Texture2D geometryTexture : register(t0);
Texture2D geometryPositionTetxure : register(t1);
Texture2D lightGeometryTexture : register(t2);
Texture2D lightInGeometryTexture : register(t3);
Texture2D lightViewTarget : register(t4);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
    float3 fragPos : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.viewDir = viewDir;
    output.fragPos = mul(float4(input.position, 1.0f), world).xyz;
    output.texCoord = input.texCoord;
    
    return output;
}

matrix Inverse(matrix m)
{
    matrix adjugate;
    float det;

    // Calculate the determinant
    det = m._11 * (m._22 * (m._33 * m._44 - m._43 * m._34) - m._32 * (m._23 * m._44 - m._43 * m._24) + m._42 * (m._23 * m._34 - m._33 * m._24))
        - m._21 * (m._12 * (m._33 * m._44 - m._43 * m._34) - m._32 * (m._13 * m._44 - m._43 * m._14) + m._42 * (m._13 * m._34 - m._33 * m._14))
        + m._31 * (m._12 * (m._23 * m._44 - m._43 * m._24) - m._22 * (m._13 * m._44 - m._43 * m._14) + m._42 * (m._13 * m._24 - m._23 * m._14))
        - m._41 * (m._12 * (m._23 * m._34 - m._33 * m._24) - m._22 * (m._13 * m._34 - m._33 * m._14) + m._32 * (m._13 * m._24 - m._23 * m._14));

    if (abs(det) < 1e-6) // Check for singularity
    { 
        return adjugate; // Non-invertible matrix
    }

    // Adjugate matrix computation
    adjugate._11 = m._22 * (m._33 * m._44 - m._43 * m._34) - m._32 * (m._23 * m._44 - m._43 * m._24) + m._42 * (m._23 * m._34 - m._33 * m._24);
    adjugate._12 = -(m._21 * (m._33 * m._44 - m._43 * m._34) - m._31 * (m._23 * m._44 - m._43 * m._24) + m._41 * (m._23 * m._34 - m._33 * m._24));
    adjugate._13 = m._21 * (m._32 * m._44 - m._42 * m._34) - m._31 * (m._22 * m._44 - m._42 * m._24) + m._41 * (m._22 * m._34 - m._32 * m._24);
    adjugate._14 = -(m._21 * (m._32 * m._43 - m._42 * m._33) - m._31 * (m._22 * m._43 - m._42 * m._23) + m._41 * (m._22 * m._33 - m._32 * m._23));

    adjugate._21 = -(m._12 * (m._33 * m._44 - m._43 * m._34) - m._32 * (m._13 * m._44 - m._43 * m._14) + m._42 * (m._13 * m._34 - m._33 * m._14));
    adjugate._22 = m._11 * (m._33 * m._44 - m._43 * m._34) - m._31 * (m._13 * m._44 - m._43 * m._14) + m._41 * (m._13 * m._34 - m._33 * m._14);
    adjugate._23 = -(m._11 * (m._32 * m._44 - m._42 * m._34) - m._31 * (m._12 * m._44 - m._42 * m._14) + m._41 * (m._12 * m._34 - m._32 * m._14));
    adjugate._24 = m._11 * (m._32 * m._43 - m._42 * m._33) - m._31 * (m._12 * m._43 - m._42 * m._13) + m._41 * (m._12 * m._33 - m._32 * m._13);

    adjugate._31 = m._12 * (m._23 * m._44 - m._43 * m._24) - m._22 * (m._13 * m._44 - m._43 * m._14) + m._42 * (m._13 * m._24 - m._23 * m._14);
    adjugate._32 = -(m._11 * (m._23 * m._44 - m._43 * m._24) - m._21 * (m._13 * m._44 - m._43 * m._14) + m._41 * (m._13 * m._24 - m._23 * m._14));
    adjugate._33 = m._11 * (m._22 * m._44 - m._42 * m._24) - m._21 * (m._12 * m._44 - m._42 * m._14) + m._41 * (m._12 * m._24 - m._22 * m._14);
    adjugate._34 = -(m._11 * (m._22 * m._43 - m._42 * m._23) - m._21 * (m._12 * m._43 - m._42 * m._13) + m._41 * (m._12 * m._23 - m._22 * m._13));

    adjugate._41 = -(m._12 * (m._23 * m._34 - m._33 * m._24) - m._22 * (m._13 * m._34 - m._33 * m._14) + m._32 * (m._13 * m._24 - m._23 * m._14));
    adjugate._42 = m._11 * (m._23 * m._34 - m._33 * m._24) - m._21 * (m._13 * m._34 - m._33 * m._14) + m._31 * (m._13 * m._24 - m._23 * m._14);
    adjugate._43 = -(m._11 * (m._22 * m._34 - m._32 * m._24) - m._21 * (m._12 * m._34 - m._32 * m._14) + m._31 * (m._12 * m._24 - m._22 * m._14));
    adjugate._44 = m._11 * (m._22 * m._33 - m._32 * m._23) - m._21 * (m._12 * m._33 - m._32 * m._13) + m._31 * (m._12 * m._23 - m._22 * m._13);

    // Divide by determinant to get the inverse
    return adjugate / det;
}

float3 GetWorldPosition(float3 viewSpacePosition, matrix invViewMatrix, matrix invModelMatrix)
{
    // Sample the view-space position from the texture
    //float3 viewSpacePosition = linearDepthTexture.Sample(samplerState, screenUV).xyz;

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
    float phase = (1.0 - g * g) / pow(1.0 + g * g - 2.0 * g * dot(lightDir, viewDir), 1.5);

    // Scattering contribution is proportional to density and phase
    float scattering = density * phase;
    
    return saturate(scattering);
}
// ========================================================================
// ========================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 depth = lightGeometryTexture.Sample(textureSampler, input.texCoord);
    if (length(depth) <= 0.001)
    {
        return geometryTexture.Sample(textureSampler, input.texCoord);
    }
    
    float3 lightPos = GetWorldPosition(depth * 100.0f, Inverse(camViewMatrix), Inverse(modelMatrix));
    
    // input.position * invScreenMat * invCameraProj * invCameraView = positionInWorld
    // positionWorld = mul(mul(mul(input.position, invScreenMat), invCameraProj), invCameraView)
    // pass in screenToWorldMat = invScreenMat * invCameraProj * invCameraView; (transpose)
    // pixelInWorld = mul(input.position, screenToWorldMat)
    
    
    /*
    float4 clipPos = float4(input.texCoord.x, input.texCoord.y, depth, 1.0f);
    float4 viewPos = mul(Inverse(lightViewProj), clipPos);
    float3 worldPos2 = viewPos.xyz / viewPos.w;
    */
    
    // Initialize light
    float3 lightPos = float3(0, 10, 0); // Light source position
    float3 lightColor = float3(1, 1, 0.8); // Light color
    float3 lightDir = normalize(lightPos - input.fragPos); // Direction from light to point
    
    int numSteps = 10;
    float stepSize = 0.1;

    // Raymarch from light to fragment position
    float3 lightRayPos = lightPos; // Starting point of the light ray
    float3 lightRayStep = normalize(input.fragPos - lightPos) * stepSize; // Increment along light direction
    float lightTransmittance = 1.0; // Accumulated attenuation along the light ray
    float lightAbsorptionCoefficient = 0.5; // Accumulated attenuation along the light ray

    for (int i = 0; i < numSteps; ++i)
    {
        // Sample density along the light ray
        float density = GetProceduralDensity(lightRayPos);

        // Attenuate light based on density
        lightTransmittance *= exp(-density * lightAbsorptionCoefficient);

        // Early exit if light is fully absorbed
        if (lightTransmittance < 0.01)
            break;

        // Move to the next position along the light ray
        lightRayPos += lightRayStep;

        // Stop if the light ray reaches the fragment position
        if (length(lightRayPos - input.fragPos) < stepSize)
            break;
    }

    // Raymarch through the volume from the fragment's perspective
    float3 rayPos = input.fragPos; // Starting point of the view ray
    float3 rayStep = input.viewDir * stepSize; // Increment along view direction
    float accumulatedLight = 0.0; // Accumulated light contribution

    for (int i = 0; i < numSteps; ++i)
    {
        // Sample the dust density from the 3D noise texture
        float density = GetProceduralDensity(rayPos);

        // Compute scattering at the current position
        float3 scattering = ComputeScattering(lightDir, input.viewDir, density);

        // Combine scattering with light attenuation
        accumulatedLight += dot(lightColor * lightTransmittance, scattering) * density * stepSize;

        // Move to the next position along the ray
        rayPos += rayStep;
    }

    // Apply distance attenuation
    float distanceAttenuation = 1.0 / (1.0 + length(input.fragPos - lightPos) * 0.1);
    accumulatedLight *= distanceAttenuation;

    // Output the final color
    return float4(accumulatedLight, accumulatedLight, accumulatedLight, 1.0);
}