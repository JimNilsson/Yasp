struct VS_OUT
{
	float4 pos : SV_POSITION;
};

float4 main( VS_OUT input ) : SV_TARGET
{
	return float4(normalize(input.pos.xyz), 1.0f);
}