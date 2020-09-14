struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    return pow(float4(input.color, 1.f), float4(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f, 1.f));
    //return float4(input.color, 1.f);
    
}