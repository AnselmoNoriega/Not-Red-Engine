
cbuffer TranformBuffer : register(b0)
{
    matrix wvp;
    matrix world;
    float3 viewDir;
    matrix lightViewProj;
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

float4x4 Inverse(float4x4 m)
{
    float4x4 result;
    
    // Compute the determinant of the matrix
    float det = dot(m._m00, cross(m._m10, m._m20))
              - dot(m._m01, cross(m._m11, m._m21))
              + dot(m._m02, cross(m._m12, m._m22));
    
    if (det == 0.0)
    {
        return result; // non-invertible
    }
    
    // Compute the adjugate matrix (Cofactor method)
    
    result._m00 = dot(m._m11, cross(m._m21, m._m31)) - dot(m._m12, cross(m._m22, m._m32)) + dot(m._m13, cross(m._m23, m._m33));
    result._m01 = -(dot(m._m10, cross(m._m20, m._m30)) - dot(m._m12, cross(m._m22, m._m32)) + dot(m._m13, cross(m._m23, m._m33)));
    result._m02 = dot(m._m10, cross(m._m20, m._m30)) - dot(m._m11, cross(m._m21, m._m31)) + dot(m._m13, cross(m._m23, m._m33));
    result._m03 = -(dot(m._m10, cross(m._m20, m._m30)) - dot(m._m11, cross(m._m21, m._m31)) + dot(m._m12, cross(m._m22, m._m32)));

    result._m10 = -(dot(m._m01, cross(m._m21, m._m31)) - dot(m._m02, cross(m._m22, m._m32)) + dot(m._m03, cross(m._m23, m._m33)));
    result._m11 = dot(m._m00, cross(m._m20, m._m30)) - dot(m._m02, cross(m._m22, m._m32)) + dot(m._m03, cross(m._m23, m._m33));
    result._m12 = -(dot(m._m00, cross(m._m10, m._m30)) - dot(m._m02, cross(m._m12, m._m32)) + dot(m._m03, cross(m._m13, m._m33)));
    result._m13 = dot(m._m00, cross(m._m10, m._m30)) - dot(m._m01, cross(m._m11, m._m31)) + dot(m._m03, cross(m._m13, m._m33));

    result._m20 = dot(m._m01, cross(m._m11, m._m21)) - dot(m._m02, cross(m._m12, m._m22)) + dot(m._m03, cross(m._m13, m._m23));
    result._m21 = -(dot(m._m00, cross(m._m10, m._m20)) - dot(m._m02, cross(m._m12, m._m22)) + dot(m._m03, cross(m._m13, m._m23)));
    result._m22 = dot(m._m00, cross(m._m10, m._m20)) - dot(m._m01, cross(m._m11, m._m21)) + dot(m._m03, cross(m._m13, m._m23));
    result._m23 = -(dot(m._m00, cross(m._m10, m._m20)) - dot(m._m01, cross(m._m11, m._m21)) + dot(m._m02, cross(m._m12, m._m22)));

    result._m30 = -(dot(m._m01, cross(m._m11, m._m21)) - dot(m._m02, cross(m._m12, m._m22)) + dot(m._m03, cross(m._m13, m._m23)));
    result._m31 = dot(m._m00, cross(m._m10, m._m20)) - dot(m._m01, cross(m._m11, m._m21)) + dot(m._m02, cross(m._m12, m._m22));
    result._m32 = -(dot(m._m00, cross(m._m10, m._m20)) - dot(m._m01, cross(m._m11, m._m21)) + dot(m._m02, cross(m._m12, m._m22)));
    result._m33 = dot(m._m00, cross(m._m10, m._m20)) - dot(m._m01, cross(m._m11, m._m21)) + dot(m._m02, cross(m._m12, m._m22));

    // Divide by the determinant to get the final inverse matrix
    result = result / det;

    return result;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.viewDir = viewDir;
    output.fragPos = mul(float4(input.position, 1.0f), world).xyz;
    output.texCoord = input.texCoord;
    
    //output.geometryPositionDepth = geometryPositionTetxure.Sample(textureSampler, input.texCoord).r;
    //output.lightGeometryDepth = lightGeometryTexture.Sample(textureSampler, input.texCoord).r;
    //output.lightInGeometryDepth = lightInGeometryTexture.Sample(textureSampler, input.texCoord).r;
    //output.lightViewDepth = lightViewTarget.Sample(textureSampler, input.texCoord).r;
    
    return output;
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
    float3 conePosition = float3(0.0, 0.0, 0.0);
    float coneHeight = 10.0;
    float coneRadius = 5.0;
    float4 coneColor = (0.0, 1.0, 0.0, 1.0);
    
    float4 baseColor = geometryTexture.Sample(textureSampler, input.texCoord);

    // Calculate cone properties in view space
    /*float3 pixelPos = float3(input.texCoord * 2.0f - 1.0f, 0.0f); // Normalized device coordinates
    float3 coneDir = normalize(conePosition - pixelPos); // Direction to cone center

    float heightFactor = saturate((coneHeight - length(pixelPos.xy - conePosition.xy)) / coneHeight);
    float radiusFactor = saturate(1.0f - length(pixelPos.xy - conePosition.xy) / coneRadius);

    // Check if the pixel lies within the cone
    if (heightFactor > 0.0f && radiusFactor > 0.0f)
    {
        float blendFactor = heightFactor * radiusFactor;
        baseColor = lerp(baseColor, coneColor, blendFactor); // Blend cone color with base color
    }
    */
    return baseColor;
    /*
    float depth = lightGeometryTexture.Sample(textureSampler, input.texCoord).r;
    float4 clipPos = float4(input.texCoord.x, input.texCoord.y, depth, 1.0f);
    float4 viewPos = mul(Inverse(lightViewProj), clipPos);
    float3 worldPos = viewPos.xyz / viewPos.w;
    
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
    */
}