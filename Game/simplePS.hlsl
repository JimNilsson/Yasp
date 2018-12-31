Texture2D albedo : register(t0);
SamplerState samAnis : register(s0);

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main( VS_OUT input ) : SV_TARGET
{
	return albedo.Sample(samAnis, input.tex);
}