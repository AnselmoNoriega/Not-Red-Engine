
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

float4 PSMain(float3 fragPos, float3 viewDir) : SV_Target
{
    // Initialize light intensity and position
    float3 lightPos = float3(0, 10, 0);
    float3 lightColor = float3(1, 1, 0.8);
    float3 lightDir = normalize(lightPos - fragPos);

    // Raymarch through the volume
    float3 rayPos = fragPos; // Starting point of the ray
    float3 rayStep = viewDir * stepSize; // Increment along the ray direction
    float accumulatedLight = 0.0;

    for (int i = 0; i < numSteps; ++i)
    {
        // Sample the dust density from the 3D noise texture
        float density = SampleDensity(rayPos); // 3D noise texture lookup

        // Compute scattering at the current position
        float3 scattering = ComputeScattering(lightDir, viewDir, density);

        // Accumulate light contribution and attenuate
        accumulatedLight += dot(lightColor, scattering) * density * stepSize;

        // Move to the next position along the ray
        rayPos += rayStep;
    }

    // Apply accumulated light and attenuate by distance
    float distanceAttenuation = 1.0 / (1.0 + length(fragPos - lightPos) * 0.1);
    accumulatedLight *= distanceAttenuation;

    // Output the final color
    return float4(accumulatedLight, accumulatedLight, accumulatedLight, 1.0);
}