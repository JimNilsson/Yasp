Texture2D albedo : register(t0);
SamplerState samAnis : register(s0);

cbuffer ObjectBuffer : register(b0)
{
	float4 pointlight;
}

cbuffer PointLights : register(b2)
{
	uint4 pointlightCount;
	float4 pointlights[16];
}

cbuffer EyePos : register(b1)
{
	float4 eyepos;
}

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float3 wpos : POSITION;
	float2 tex : TEXCOORD;
};


float4 main( VS_OUT input ) : SV_TARGET
{
	float4 texColor = albedo.Sample(samAnis, input.tex);
	float3 diffuseContribution = float3(0,0,0);
	float3 specularContribution = float3(0,0,0);
	for(uint i = 0; i < pointlightCount.x; i++)
	{
		float3 light = pointlights[i].xyz - input.wpos;
		float distance = length(light);
		light /= distance;
		float ndl = saturate(dot(input.nor, light));
		if(ndl > 0.0f)
		{
			float divby = (distance / pointlight.w) + 1.0f;
			float att = (5.0f / (divby * divby)) - 5.0f / 4.0f;
			diffuseContribution += att * ndl;
			
			float3 V = normalize(eyepos.xyz - input.wpos.xyz);
			float3 H = normalize(light + V);
			
			float power = pow(abs(dot(input.nor, H)), 16.0f);
			if(power > 0.0f)
			{
				specularContribution += power * ndl;
			}
			
		}
	}
	float3 sunlight = float3(1.0f,1.0f,1.0f);
	sunlight = normalize(sunlight);
	diffuseContribution += dot(input.nor, sunlight) * 2.0f * texColor;
	
	float4 dicks = float4(texColor.xyz * (diffuseContribution + specularContribution + float3(0.1f, 0.1f, 0.1f)), 1.0f);
	
	//return saturate(float4(input.nor, dicks.w + dicks.z * 0.000001f));
	//return float4(input.wpos, 1.0f);
	//return float4(ndl, ndl, ndl, 1.0f);
	//return float4(dot(input.nor, sunlight), dot(input.nor, sunlight),dot(input.nor, sunlight), 1.0f);
	
	//return float4(dot(input.nor, sunlight) + dicks.z - 1.0f, dot(input.nor, sunlight),dot(input.nor, sunlight), 1.0f);
	return float4(texColor.xyz * (diffuseContribution + specularContribution + float3(0.1f, 0.1f, 0.1f)), 1.0f);
}