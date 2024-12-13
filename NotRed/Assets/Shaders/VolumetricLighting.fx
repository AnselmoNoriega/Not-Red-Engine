
cbuffer ViewBuffer : register(b0)
{
    matrix viewMatrix;
    matrix viewProjection;
    float3 camPos;
}

cbuffer LightBuffer : register(b1)
{
    matrix lwvp;
    matrix lightViewProj;
    matrix lightView;
    float3 lightPos;
    float3 lightColor;
}

cbuffer MatrixBuffer : register(b2)
{
    matrix lightMatrix;
    matrix geoMatrix;
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
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
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
    float3 cameraDepthViewPos = geometryPositionTetxure.Sample(textureSampler, texCoord).rgb * 100.0f;

    // Reconstruct world position from depth
    float4 cameraDepthViewPos4 = float4(cameraDepthViewPos, 1.0f);
    float4 worldPos = mul(Inverse(viewMatrix), cameraDepthViewPos4);

    // Transform world position into the light's view space
    float4 lightViewPos = mul(lightView, worldPos);

    // Normalize light view position
    float3 lightDepth = lightViewPos.xyz / lightViewPos.w; // Perspective division
  
    float2 lightUV = lightViewPos.xy / lightViewPos.w * 0.5f + 0.5f;

    // Sample the light's depth texture
    float3 sampledLightDepth = lightViewTarget.Sample(textureSampler, lightUV).rgb * 100.0f;

    // Compare depths (light hitting point in view of light)
    float lightHit = (lightDepth.z <= sampledLightDepth.z + 0.001) ? 1.0f : 0.0f;

    // Return color based on light hitting
    return float4(lightHit, lightHit, lightHit, 1.0f); 
    return color;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    // Sample necessary textures
    float3 lightDepth = lightGeometryTexture.Sample(textureSampler, input.texCoord).xyz;
    float3 lightDepthIn = lightInGeometryTexture.Sample(textureSampler, input.texCoord).xyz;
    float3 geometryPos = geometryPositionTetxure.Sample(textureSampler, input.texCoord).xyz;

    if (length(lightDepth) <= 0.001)
    {
        return geometryTexture.Sample(textureSampler, input.texCoord); // No volumetric effect if light depth is invalid
    }

    // Transform positions to world space
    float3 lightGeomPos = GetWorldPosition(lightDepth * 100.0f, Inverse(viewMatrix), Inverse(lightMatrix));
    float3 lightGeomInPos = GetWorldPosition(lightDepthIn * 100.0f, Inverse(viewMatrix), Inverse(lightMatrix));
    float3 worldGeoPos = GetWorldPosition(geometryPos * 100.0f, Inverse(viewMatrix), Inverse(geoMatrix));
    
    float3 camViewDir = normalize(camPos - lightGeomPos);

    // Raymarch settings
    float mainRayLength = length(lightGeomInPos - lightGeomPos);
    float lightRayLength = length(lightPos - lightGeomPos);
    
    int numStepsMain = int(mainRayLength * 2.0f); // Number of steps for the main raymarch
    int numStepsLight = int(lightRayLength * 2.0f); // Number of steps for the light raymarch
    float lightAbsorptionCoefficient = 0.5f;

    float accumulatedLight = 0.0f;

    // Calculate the main ray step
    float3 mainDirection = normalize(lightGeomInPos - lightGeomPos);
    float mainStepSize = length(mainDirection) / numStepsMain;
    
    // Initialize overall transmittance
    float transmittance = 1.0f;
    
    // Raymarch from geometry to light
    [loop]
    for (int i = 0; i < numStepsMain; i++)
    {
        float3 rayPos = lightGeomPos + mainDirection * (i * mainStepSize);

        //======================== Check Light Occlusion ========================
        float4 projectedPos = mul(lightViewProj, float4(rayPos, 1.0f));
        float2 lightTexCoords = projectedPos.xyz / projectedPos.w;
        
        /*float3 ndcPos = lightClipPos.xyz / lightClipPos.w;
        float2 lightTexCoords = ndcPos.xy * 0.5 + 0.5;*/

        if (lightTexCoords.x >= 0.0 && lightTexCoords.x <= 1.0 &&
            lightTexCoords.y >= 0.0 && lightTexCoords.y <= 1.0)
        {
            float depthFromLightView = length(lightViewTarget.Sample(textureSampler, lightTexCoords).rgb);

            if (projectedPos.z > depthFromLightView + 0.005)
            {
                // Light is occluded; no contribution from this step
                transmittance *= 0.5; // Gradually reduce intensity due to occlusion
                continue;
            }
        }
        //=======================================================================

        // Sample density at the current position
        float density = GetProceduralDensity(rayPos);

        // Calculate scattering for this position
        float3 lightDir = normalize(lightPos - rayPos);
        float3 scattering = ComputeScattering(lightDir, camViewDir, density);

        // Attenuate light due to density
        transmittance *= exp(-density * lightAbsorptionCoefficient);

        // Early exit if light intensity drops below visibility
        if (transmittance < 0.01f)
            break;

        // Accumulate light considering scattering and transmittance
        accumulatedLight += 10 * (transmittance * scattering * mainStepSize) / length(lightPos - rayPos);
    }

    // Apply distance attenuation to the accumulated light
    float distanceToLight = length(lightGeomPos - lightPos);
    float distanceAttenuation = 1.0f / (1.0f + distanceToLight * 0.1f);
    accumulatedLight *= distanceAttenuation;

    // Blend accumulated light with the base geometry texture
    float4 baseColor = geometryTexture.Sample(textureSampler, input.texCoord);
    baseColor = LitTexture(baseColor, input.texCoord);
    float alpha = saturate(accumulatedLight);// / (5 / (length(lightGeomInPos - lightGeomPos) / 2)); // Scale blending factor
    float3 finalColor = lerp(baseColor.rgb, lightColor, alpha * 2);

    return float4(finalColor, 1.0f);
}