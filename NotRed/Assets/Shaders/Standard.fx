
cbuffer TransformBuffer : register(b0)
{
    matrix wvp;
    matrix lwvp;
    matrix world;
    float3 viewPosition;
}
cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useDiffuseMap;
    bool useNormalMap;
    bool useSpecMap;
    bool useBumpMap;
    bool useShadowMap;
    bool useSkinning;
    float bumpWeight;
    float depthBias;
}

cbuffer BoneTransformBuffer : register(b4)
{
    matrix boneTransforms[256];
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specMap : register(t2);
Texture2D bumpMap : register(t3);
Texture2D shadowMap : register(t4);
SamplerState textureSampler : register(s0);

static matrix Identity =
{
    1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
    if (length(weights) <= 0.0f)
    {
        return Identity;
    }

    matrix transform = boneTransforms[indices[0]] * weights[0];
    for (int i = 1; i < 4; i++)
    {
        transform += boneTransforms[indices[i]] * weights[i];
    }

    return transform;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTagent : TANGENT;
    float2 texCoord : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
    matrix toWorld = world;
    matrix toNDC = wvp;
    matrix toLightNDC = lwvp;

    if (useSkinning)
    {
        matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
        toWorld = mul(boneTransform, toWorld);
        toNDC = mul(boneTransform, toNDC);
        toLightNDC = mul(boneTransform, toLightNDC);
    }

    float3 localPosition = input.position;
    if (useBumpMap)
    {
        float4 bumpMapColor = bumpMap.SampleLevel(textureSampler, input.texCoord, 0.0f).r;
        float bumpColor = (2.0f * bumpMapColor.r) - 1.0f;
        localPosition += (input.normal * bumpColor * bumpWeight);
    }
	
    VS_OUTPUT output;
    output.position = mul(float4(localPosition, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.worldTagent = mul(input.tangent, (float3x3) toWorld);
    output.texCoord = input.texCoord;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(localPosition, 1.0f), world).xyz);
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(localPosition, 1.0f), lwvp);
    }

	
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
	
    if (useNormalMap)
    {
        float3 t = normalize(input.worldTagent);
        float3 b = normalize(cross(n, t));
        float3x3 tbnw = float3x3(t, b, n);
        float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
        float3 unpackedNormalMap = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
        n = normalize(mul(unpackedNormalMap, tbnw));
    }

    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);

    float4 ambient = lightAmbient * materialAmbient;

    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;
	
    float4 emissive = materialEmissive;
	
    float4 diffuseMapColor = (useDiffuseMap) ? diffuseMap.Sample(textureSampler, input.texCoord) : 1.0f;
    float4 specMapColor = (useSpecMap) ? specMap.Sample(textureSampler, input.texCoord).r : 1.0f;
	
    float4 finalColor = (ambient + diffuse + emissive) * diffuseMapColor + (specular * specMapColor);
	
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = (1.0f - (shadowUV.y + 1.0f)) * 0.5f;

        if (saturate(u) == u && saturate(v) == v)
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;

            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (ambient + materialEmissive) * diffuseMapColor;
            }
        }
    }


    return finalColor;
}