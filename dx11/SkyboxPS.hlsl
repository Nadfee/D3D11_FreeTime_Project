
struct PS_IN
{
    float4 pos : SV_Position;
    float3 wsPos : POSITION;
};

TextureCube textureCube : register(t0);
SamplerState defaultSampler : register(s0);


float4 PSMAIN(PS_IN input) : SV_TARGET
{
    //return float4(1.f, 1.f, 1.f, 1.f);
    return textureCube.Sample(defaultSampler, input.wsPos);
}