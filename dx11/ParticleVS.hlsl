struct Particle
{
    float3 pos;
    float padding1;
    float3 color;
    float padding2;
    
};

StructuredBuffer<Particle> funneledVertices : register(t5);

struct VS_OUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

VS_OUT VSMAIN( uint vID : SV_VertexID )
{
    VS_OUT output = (VS_OUT) 0;
    
    output.pos = funneledVertices[vID].pos;
    output.color = funneledVertices[vID].color;
  
	return output;
}