struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

cbuffer ObjectBuffer : register(b0)
{
	float4x4 gWVP;
}


VS_OUT main( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;

	output.pos =  mul(float4(input.pos, 1.0f), gWVP);
	
	return output;
}