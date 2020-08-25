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

// quad
static const float4 quad[4] =
{
    float4(-1, 1, 0, 0),
    float4(1, 1, 0, 0),
    float4(-1, -1, 0, 0),
    float4(1, -1, 0, 0),
};

// cube
static const float4 CUBE[36] =
{

float4(0.5f, -0.5f, -0.5f, 0.5f),
float4(-0.5f, -0.5f, -0.5f, 0.5f),
float4(-0.5f, 0.5f, -0.5f, 0.5f),

float4(-0.5f, 0.5f, -0.5f, 0.5f),
float4(0.5f, 0.5f, -0.5f, 0.5f),
float4(0.5f, -0.5f, -0.5f, 0.5f),

float4(-0.5f, -0.5f, 0.5f, 0.5f),
float4(0.5f, -0.5f, 0.5f, 0.5f),
float4(0.5f, 0.5f, 0.5f, 0.5f),

float4(0.5f, 0.5f, 0.5f, 0.5f),
float4(-0.5f, 0.5f, 0.5f, 0.5f),
float4(-0.5f, -0.5f, 0.5f, 0.5f),

float4(-0.5f, -0.5f, -0.5f, 0.5f),
float4(-0.5f, -0.5f, 0.5f, 0.5f),
float4(-0.5f, 0.5f, 0.5f, 0.5f),
    
float4(-0.5f, 0.5f, 0.5f, 0.5f),
float4(-0.5f, 0.5f, -0.5f, 0.5f),
float4(-0.5f, -0.5f, -0.5f, 0.5f),

float4(0.5f, -0.5f, 0.5f, 0.5f),
float4(0.5f, -0.5f, -0.5f, 0.5f),
float4(0.5f, 0.5f, -0.5f, 0.5f),

float4(0.5f, 0.5f, -0.5f, 0.5f),
float4(0.5f, 0.5f, 0.5f, 0.5f),
float4(0.5f, -0.5f, 0.5f, 0.5f),

float4(0.5f, 0.5f, -0.5f, 0.5f),
float4(-0.5f, 0.5f, -0.5f, 0.5f),
float4(-0.5f, 0.5f, 0.5f, 0.5f),

float4(-0.5f, 0.5f, 0.5f, 0.5f),
float4(0.5f, 0.5f, 0.5f, 0.5f),
float4(0.5f, 0.5f, -0.5f, 0.5f),

float4(0.5f, -0.5f, 0.5f, 0.5f),
float4(-0.5f, -0.5f, 0.5f, 0.5f),
float4(-0.5f, -0.5f, -0.5f, 0.5f),
    
float4(-0.5f, -0.5f, -0.5f, 0.5f),
float4(0.5f, -0.5f, -0.5f, 0.5f),
float4(0.5f, -0.5f, 0.5f, 0.5f)
};

static const float4 NEWSTUFF[8] =
{
    float4(-1, 1, -1.f, 0),
    float4(1, 1, -1.f, 0),
    float4(-1, -1, -1.f, 0),
    float4(1, -1, -1.f, 0),
    
    float4(-1, 1, 7.f, 0),
    float4(1, 1, 7.f, 0),
    float4(-1, -1, 7.f, 0),
    float4(1, -1, 7.f, 0),
    
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
   
    float dist = length(input[0].pos - float3(0.f, 0.f, 0.f));
    if (dist <= 0)
        dist = 0.f;
    
    float3 color = input[0].color;
    if (input[0].color.g != 1.f)
        color = (float4(0.2f, 0.2f, 0.7f, 0.f) * dist + float4(1.f, 0.3f, 0.2f, 0.f) * (4.f - dist)).xyz;

    //Quad

    for (uint i = 0; i < 4; i++)
    {
        GS_OUT element = (GS_OUT) 0;
        
        float4 worldPos = mul(worldMat, float4(input[0].pos, 1.f));
        float4 viewPos = mul(viewMat, worldPos);
        float4 quadCorner = viewPos + 0.04f * quad[i];
        
  
        quadCorner = mul(projMat, quadCorner);
        
        element.pos = quadCorner;
        element.color = color;
	
        output.Append(element);
    }
    output.RestartStrip();
    

    
    ////Cube
    //for (uint i = 0; i < 36; i++)
    //{
    //    if (i % 3 == 0)
    //        output.RestartStrip();
        
    //    GS_OUT element = (GS_OUT) 0;
        
    //    float4 worldPos = mul(worldMat, float4(input[0].pos, 1.f));
    //    float4 viewPos = mul(viewMat, worldPos) + 0.05f * float4(CUBE[i].xyz, 1.f);
    //    float4 quadCorner = viewPos;
        
  
    //    quadCorner = mul(projMat, quadCorner);
        
    //    element.pos = quadCorner;
    //    element.color = color;
	
    //    output.Append(element);

    //}
    
    //output.RestartStrip();

}