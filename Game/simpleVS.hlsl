struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 wpos : POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};


cbuffer PerFrameBuffer : register(b0)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
	float4x4 gInvView;
	float4x4 gInvViewProj;
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 gWVP;
	float4x4 gWorldViewInvTrp;
	float4x4 gWorld;
}


VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos =  mul(float4(input.pos, 1.0f), gWVP);
	output.wpos = mul(float4(input.pos, 1.0f), gWorld);
	output.tex = input.tex;
	output.nor = mul(float4(input.nor,1.0f), gWorldViewInvTrp);
	
	return output;
}