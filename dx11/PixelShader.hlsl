struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D diffuseTexture : register(t0);
SamplerState defaultSampler : register(s0);

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    //return float4(input.uv.xy, 0.f, 1.f);
    return diffuseTexture.Sample(defaultSampler, input.uv);

}