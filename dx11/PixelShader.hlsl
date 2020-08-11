struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
};

struct PointLight
{
    float3 lightPosition;
    float3 lightColor;
    float radius;
};

Texture2D diffuseTexture : register(t0);
StructuredBuffer<PointLight> lightBuffer : register(t1);
SamplerState defaultSampler : register(s0);

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    
    return float4(lightBuffer[0].lightColor, 1.f);
    
    //return float4(normalize(input.nor.xyz), 1.f);
	//return float4(input.uv.xy, 0.f, 1.f);
    return diffuseTexture.Sample(defaultSampler, input.uv);
    


}