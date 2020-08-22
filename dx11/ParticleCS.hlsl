struct Particle
{
    float3 pos;
    float padding1;
    float3 color;
    float padding2;
};

cbuffer ParticleCountBuffer : register(b0)
{
    uint4 numParticles; // X is num particles!
}

cbuffer SimulationBuffer : register(b1)
{
    float wave;
}

ConsumeStructuredBuffer<Particle> bufferA : register(u0);
AppendStructuredBuffer<Particle> bufferB : register(u1);

#define MaxParticleCount 512

[numthreads(MaxParticleCount, 1, 1)]
void CSMAIN( uint3 DTid : SV_DispatchThreadID )
{       
    
    // Check for if this thread should run or not.
    uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;
    
    if (myID < numParticles.x)
    {
            
        Particle p = bufferA.Consume();
        // p.pos.y = wave + DTid.x;     // cool effect
        p.pos.y = wave;
        p.color.r = wave * 0.5f + 0.5f;
        bufferB.Append(p);
   
    }


}