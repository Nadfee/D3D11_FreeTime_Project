struct GS_IN
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

// clockwise [0, 4]
static const float4 quad[4] =
{
    float4(-1, 1, 0, 0),
    float4(1, 1, 0, 0),
    float4(-1, -1, 0, 0),
    float4(1, -1, 0, 0),
};

cbuffer WorldMatBuffer : register(b0)
{
    Matrix worldMat;
}

cbuffer ViewMatBuffer : register(b1)
{
    Matrix viewMat;
}

cbuffer ProjMatBuffer : register(b2)
{
    Matrix projMat;
}


[maxvertexcount(4)]
void GSMAIN( point GS_IN input[1],  inout TriangleStream< GS_OUT > output  )
{
    
    //float dist = length(input[0].pos - float3(0.f, 0.f, 0.f));
    //if (dist < 8.f)
    //    dist -= 7.f;
    
    //dist = saturate(dist);
    
    //float4 color = float4(0.2f, 0.2f, 1.0f, 0.0f) * (dist) + float4(1.0f, 0.2f, 0.2f, 0.0f) * (1.0f - dist);
   
    
	for (uint i = 0; i < 4; i++)
	{
        GS_OUT element = (GS_OUT)0;
        
        //float4 viewPos = mul(worldMat, float4(input[0].pos, 1.f)); 
        float4 viewPos = mul(viewMat, float4(input[0].pos, 1.f)); // Temporary, not using world matrix because its not set up
        float4 quadCorner = viewPos + 0.02f * quad[i];
        
  
        quadCorner = mul(projMat, quadCorner);
        
        element.pos = quadCorner;
        element.color = input[0].color;
	
		output.Append(element);
	}
    output.RestartStrip();

}