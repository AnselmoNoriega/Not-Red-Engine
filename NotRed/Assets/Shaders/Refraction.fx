cbuffer RefractionHelper : register(b0)
{
	float3 lightDir;
	float4 lightColor;
	float time;
};

Texture2D water : register(t0);
Texture2D targets : register(t1);
Texture2D depth : register(t2);
Texture2D waterDepth : register(t3);
Texture2D waterHeight : register(t4);
Texture2D foamTexture : register(t5);
Texture2D waterDistortion : register(t6);

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

float4 PS(VS_OUTPUT input) : SV_Target
{
	return waterHeight.Sample(textureSampler, input.texCoord);
	float3 lighdir = normalize(float3(1.0f, -1.0f, 1.0f));
	float4 lighCol = float4(1, 0.97, 0.97, 1.000000000f);

	float4 objectDist = depth.Sample(textureSampler, input.texCoord);
	float4 waterDist = waterDepth.Sample(textureSampler, input.texCoord);

	if (objectDist.x < waterDist.x)
	{
		float3 waterDirection = water.Sample(textureSampler, input.texCoord).xyz;
		float dif = max(dot(waterDirection, -lighdir), 0.0f);
		float ambient = 0.2;

		float movedCoord = float2(input.texCoord.y + time, input.texCoord.x + time);
		float2 distortion = waterDistortion.Sample(textureSampler, movedCoord).xy * 0.1;
		float2 refractedUV = input.texCoord + distortion;
		float4 objectDist2 = depth.Sample(textureSampler, refractedUV);
		float4 waterDist2 = waterDepth.Sample(textureSampler, refractedUV);
		float4 color = float4(0.305, 0.952, 0.984, 1);
		float blendFactor = 0.5;

		float4 ObjectColor;
		if (objectDist2.x > waterDist2.x)
		{
			ObjectColor = targets.Sample(textureSampler, input.texCoord);
		}
		else
		{
			ObjectColor = targets.Sample(textureSampler, refractedUV);
		}
			
		color = lerp(color, ObjectColor, blendFactor);
		if (objectDist.a != 0.0f && waterDist.x - objectDist.x <= 0.01)
		{
			color = foamTexture.Sample(textureSampler, input.texCoord);
		}
		float4 col = float4(lighCol.xyz * (dif + ambient), 1);
		col = lerp(col, float4(1, 1, 1, 1), 0.1);
		return color * col;
	}
	else
	{
		float4 ObjectColor = targets.Sample(textureSampler, input.texCoord);
		return ObjectColor;
	}

}