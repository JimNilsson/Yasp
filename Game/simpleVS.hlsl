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

cbuffer EntityBuffer : register(b0)
{
	float4x4 WorldViewProjectionMatrix;
}

cbuffer WorldBuffer : register(b1)
{
	float4x4 WorldMatrix;
	float4x4 RotationMatrix;
}


VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), WorldViewProjectionMatrix);
	output.wpos = mul(float4(input.pos, 1.0f), WorldMatrix);
	output.nor = mul(float4(input.nor, 0.0f), RotationMatrix);
	output.tex = input.tex;
	
	return output;
}