// Immediate Buffer!

static const float4 CUBE[36] =
{
 
float4(-0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, -0.5, 0.5, 0.5),
 
float4(-0.5, -0.5, -0.5, 0.5),
 
float4(0.5, 0.5, 0.5, 0.5),
 
float4(0.5, -0.5, 0.5, 0.5),
 
float4(-0.5, -0.5, 0.5, 0.5),
 
float4(0.5, 0.5, -0.5, 0.5),
 
float4(0.5, -0.5, -0.5, 0.5),
 
float4(0.5, -0.5, 0.5, 0.5),
 
float4(-0.5, 0.5, -0.5, 0.5),
 
float4(-0.5, -0.5, -0.5, 0.5),
 
float4(0.5, -0.5, -0.5, 0.5),
 
float4(-0.5, -0.5, 0.5, 0.5),
 
float4(0.5, -0.5, 0.5, 0.5),
 
float4(0.5, -0.5, -0.5, 0.5),
 
float4(0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, 0.5, -0.5, 0.5),
 
float4(-0.5, 0.5, -0.5, 0.5),
 
float4(-0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, -0.5, -0.5, 0.5),
 
float4(-0.5, 0.5, 0.5, 0.5),
 
float4(0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, -0.5, 0.5, 0.5),
 
float4(0.5, 0.5, 0.5, 0.5),
 
float4(0.5, 0.5, -0.5, 0.5),
 
float4(0.5, -0.5, 0.5, 0.5),
 
float4(0.5, 0.5, -0.5, 0.5),
 
float4(-0.5, 0.5, -0.5, 0.5),
 
float4(0.5, -0.5, -0.5, 0.5),
 
float4(-0.5, -0.5, -0.5, 0.5),
 
float4(-0.5, -0.5, 0.5, 0.5),
 
float4(0.5, -0.5, -0.5, 0.5),
 
float4(0.5, 0.5, -0.5, 0.5),
 
float4(0.5, 0.5, 0.5, 0.5),
 
float4(-0.5, 0.5, -0.5, 0.5),
 
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 wsPos : POSITION;
};

cbuffer CamViewMat : register(b0)
{
    Matrix viewMatrix;
}

cbuffer CamProjMat : register(b1)
{
    Matrix projMatrix;
}


VS_OUT VSMAIN(uint vID : SV_VertexID)
{
    VS_OUT output = (VS_OUT)0;
    
    output.wsPos = CUBE[vID];
    output.pos = mul(viewMatrix, float4(CUBE[vID].xyz, 0.f));
    output.pos = mul(projMatrix, output.pos);
    //output.pos = CUBE[vID];
    output.pos.z = output.pos.w;
   
    return output;
}