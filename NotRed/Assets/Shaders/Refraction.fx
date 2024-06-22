
Texture2D water : register(t0);
Texture2D waterNormal : register(t1);
Texture2D waterDepth : register(t2);
Texture2D targets : register(t3);
Texture2D targetsDepth : register(t4);
Texture2D foamTexture : register(t5);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

double UnpackDepth(double4 packedDepth)
{
    double depth = packedDepth.x;// + packedDepth.y / 256.0 + packedDepth.z / 65536.0;
    return depth;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    double wDepth = UnpackDepth(waterDepth.Sample(textureSampler, input.texCoord));
    double tDepth = UnpackDepth(targetsDepth.Sample(textureSampler, input.texCoord));
    
    if (wDepth > tDepth)
    {
        float2 wNormal = waterNormal.Sample(textureSampler, input.texCoord).rb * 0.1f * wDepth; //check with b-g
        
        double wDistortedDepth = UnpackDepth(waterDepth.Sample(textureSampler, input.texCoord + wNormal));
        double tDistortedDepth = UnpackDepth(targetsDepth.Sample(textureSampler, input.texCoord + wNormal));
        
        float4 targetsColor;
        if (wDistortedDepth < tDistortedDepth)
        {
            targetsColor = targets.Sample(textureSampler, input.texCoord);
        }
        else
        {
            targetsColor = targets.Sample(textureSampler, input.texCoord + wNormal);
        }
        
        float4 waterColor = water.Sample(textureSampler, input.texCoord);
        float blendStrength = (waterColor.x + waterColor.y + waterColor.z) / 3;
        return lerp(targetsColor, waterColor, blendStrength);
    }
    else
    {
        return targets.Sample(textureSampler, input.texCoord);
    }

}