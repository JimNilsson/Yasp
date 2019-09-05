struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float3 wpos : POSITION;
	float2 tex : TEXCOORD;
};

cbuffer ObjectBuffer : register(b0)
{
	float4x4 gWVP;
}

cbuffer WorldBuffer : register(b1)
{
	float4x4 gWorld;
}


VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), gWVP);
	output.wpos = mul(float4(input.pos, 1.0f), gWorld);
	output.nor = mul(float4(input.nor, 0.0f), gWorld);
	output.tex = input.tex;
	
	return output;
}