Texture2D albedo : register(t0);
SamplerState samAnis : register(s0);

struct Pointlight
{
	float3 color;
	float falloffLinear;
	float3 direction;
	float directionBias;
	float3 position;
	float falloffExponential;
};

cbuffer PointLights : register(b0)
{
	uint4 pointLightCount;
	Pointlight pointLights[16];
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
	for(uint i = 0; i < pointLightCount.x; i++)
	{
		float3 light = pointLights[i].position.xyz - input.wpos;
		float dist = length(light);
		light /= dist;
		float ndl = saturate(dot(input.nor, light));
		if(ndl > 0.0f)
		{
			float att = 1.0f / (pointLights[i].falloffExponential * (dist * dist)) - pointLights[i].falloffLinear * dist;
			float3 pldir = normalize(float3(0.0f, 1.0f, 1.0f));
			float diratt = clamp(dot(pointLights[i].direction, light) + 2.0f + pointLights[i].directionBias, 0.0f, 1.0f);
			att *= diratt;

			if(att > 0)
			{
				diffuseContribution += att * ndl * pointLights[i].color;							
				float3 V = normalize(eyepos.xyz - input.wpos.xyz);
				float3 H = normalize(light + V);
				float power = pow(abs(dot(input.nor, H)), 16.0f);
				if(power > 0.0f)
				{
					specularContribution += power * ndl * 0.000001f;
				}
			}
		}
	}
	return float4(texColor.xyz * (diffuseContribution + specularContribution + float3(0.1f, 0.1f, 0.1f)), 1.0f);
}