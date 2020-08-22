// Immediate Buffer!

//static const float4 CUBE[36] =
//{
 
//float4(-0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, -0.5, 0.5, 0.5),
 
//float4(-0.5, -0.5, -0.5, 0.5),
 
//float4(0.5, 0.5, 0.5, 0.5),
 
//float4(0.5, -0.5, 0.5, 0.5),
 
//float4(-0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, 0.5, -0.5, 0.5),
 
//float4(0.5, -0.5, -0.5, 0.5),
 
//float4(0.5, -0.5, 0.5, 0.5),
 
//float4(-0.5, 0.5, -0.5, 0.5),
 
//float4(-0.5, -0.5, -0.5, 0.5),
 
//float4(0.5, -0.5, -0.5, 0.5),
 
//float4(-0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, -0.5, -0.5, 0.5),
 
//float4(0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, 0.5, -0.5, 0.5),
 
//float4(-0.5, 0.5, -0.5, 0.5),
 
//float4(-0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, -0.5, -0.5, 0.5),
 
//float4(-0.5, 0.5, 0.5, 0.5),
 
//float4(0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, 0.5, 0.5, 0.5),
 
//float4(0.5, 0.5, -0.5, 0.5),
 
//float4(0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, 0.5, -0.5, 0.5),
 
//float4(-0.5, 0.5, -0.5, 0.5),
 
//float4(0.5, -0.5, -0.5, 0.5),
 
//float4(-0.5, -0.5, -0.5, 0.5),
 
//float4(-0.5, -0.5, 0.5, 0.5),
 
//float4(0.5, -0.5, -0.5, 0.5),
 
//float4(0.5, 0.5, -0.5, 0.5),
 
//float4(0.5, 0.5, 0.5, 0.5),
 
//float4(-0.5, 0.5, -0.5, 0.5),
 
//};

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
    
    // https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-intro#3d-textures
    //output.wsPos = mul(projMatrix, CUBE[vID]).xyz; // This does change! (Wrong version) - 
    // You can try seeing the different results with "otherskybox" skybox textures
    
    // "Texture cubes are addressed from the shader with a 3D vector pointing out from the center of the texture cube."
    // Meaning local space!
    output.wsPos = CUBE[vID];                    // This works as the "UV" for the cube. We make sure the vertices have 
    
    
    output.pos = mul(viewMatrix, float4(CUBE[vID].xyz, 0.f));   // Make sure to take camera rotation into account (look at correct side of cube)
    output.pos = mul(projMatrix, output.pos);                   // Take perspective into account
    //output.pos = CUBE[vID];
    output.pos.z = output.pos.w;
   
    return output;
}