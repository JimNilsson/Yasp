struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer ObjectBuffer : register(b0)
{
	float4x4 gWVP;
}


VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos =  mul(float4(input.pos, 1.0f), gWVP);
	output.tex = input.tex;
	
	return output;
}