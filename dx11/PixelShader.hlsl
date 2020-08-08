struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    return float4(input.uv.xy, 0.f, 1.f);

}