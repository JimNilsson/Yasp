Texture2D albedo : register(t0);
SamplerState samAnis : register(s0);

cbuffer ObjectBuffer : register(b0)
{
	float4 pointlight;
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
	float3 light = pointlight.xyz - input.wpos;
	float distance = length(light);
	light /= distance;
	float ndl = saturate(dot(input.nor, light));
	if(ndl > 0.0f)
	{
		float divby = (distance / pointlight.w) + 1.0f;
		float att = (5.0f / (divby * divby)) - 5.0f / 4.0f;
		diffuseContribution = att * ndl;
		
		float3 V = normalize(eyepos.xyz - input.wpos.xyz);
		float3 H = normalize(light + V);
		
		float power = pow(abs(dot(input.nor, H)), 6.0f);
		if(power > 0.0f)
		{
			specularContribution = power * ndl;
		}
		
	}
	
	//return saturate(float4(input.nor, 1.0f));
	//return float4(input.wpos, 1.0f);
	//return float4(ndl, ndl, ndl, 1.0f);
	return float4(texColor.xyz * (diffuseContribution + specularContribution + float3(0.1f, 0.1f, 0.1f)), 1.0f);
}