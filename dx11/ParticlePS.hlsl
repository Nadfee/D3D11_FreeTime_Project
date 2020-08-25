struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    return float4(input.color, 0.7f);       // playing wtih blending
}