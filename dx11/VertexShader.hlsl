cbuffer WorldMatrix : register(b0)
{
    Matrix worldMatrix;
}

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;

};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};


VS_OUT VSMAIN( VS_IN input )
{
	VS_OUT output = (VS_OUT)0;
    output.pos = mul(float4(input.pos, 1.f), worldMatrix);
    output.uv = input.uv;
	
	return output;
}