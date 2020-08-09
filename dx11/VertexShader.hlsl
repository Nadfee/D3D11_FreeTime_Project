cbuffer WorldMatrix : register(b0)
{
    Matrix worldMatrix;
}

cbuffer ViewMatrix : register(b1)
{
    Matrix viewMatrix;
}

cbuffer ProjectionMatrix : register(b2)
{
    Matrix projectionMatrix;
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
    output.pos = mul(worldMatrix, float4(input.pos, 1.f));
    output.pos = mul(viewMatrix, output.pos);
    output.pos = mul(projectionMatrix, output.pos);
    output.uv = input.uv;
	
	return output;
}