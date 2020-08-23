struct Particle
{
    float3 pos;
    float lifetime;
    float3 color;
    float padding2;
};

cbuffer ParticleCountBuffer : register(b0)
{
    uint4 numParticles; // X is num particles!
}

cbuffer SimulationBuffer : register(b1)
{
    float deltaTime;
}

ConsumeStructuredBuffer<Particle> bufferA : register(u0);
AppendStructuredBuffer<Particle> bufferB : register(u1);

#define MaxParticleCount 512

[numthreads(MaxParticleCount, 1, 1)]
void CSMAIN( uint3 DTid : SV_DispatchThreadID )
{       
    // Simulation idea! Black hole!
    
    // Sphere max radius
    // Distance from sphere center -> Dependent on particle lifetime (when dead should be in center)
    // Use spherical coordinates and make them accumulate towards different directions! (Random rotation)
    
    // Check for if this thread should run or not.
    uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;
    
    float counterIn = deltaTime + ((float) myID / (float) numParticles.x) * MaxParticleCount;
    
    

    float counter2 = counterIn / 2.f;
    
    
    if (myID < numParticles.x)
    {
            
        Particle p = bufferA.Consume();
        
        float radius = p.lifetime * p.lifetime;
        
        // p.pos.y = wave + DTid.x;     // cool effect
        //p.pos.y = cos(wave);
        
        // Spherical coord
        //p.pos.x = radius * cos(counterIn) * sin(counter2);
        //p.pos.z = (radius * sin(counterIn) * sin(counter2));
        //p.pos.y = -radius * cos(counterIn) + 2.f * sin(deltaTime) + 5.f;
        
        // Cool too
        //p.pos.x = radius * cos(counter2) * sin(counter2);
        //p.pos.z = (radius * sin(counterIn) * sin(counterIn));
        //p.pos.y = -radius * cos(counter2) + 2.f * sin(deltaTime) + 5.f;
        
        p.pos.x = radius * cos(counter2) * sin(counter2);
        p.pos.z = (radius * cos(counterIn) * sin(counterIn));
        p.pos.y = -radius * cos(counter2) + 2.f * cos(deltaTime) + 5.f;
    
        // Cool
        //p.pos.x = radius * cos(counterIn) * sin(counter2);
        //p.pos.z = (radius * sin(counter2) * sin(counter2));
        //p.pos.y = -radius * cos(counterIn) + 2.f * sin(deltaTime) + 5.f;

        //p.color.r = cos(deltaTime) * 0.5f + 0.5f;
        //p.color.g = (sin(deltaTime) * 0.5f + 0.5f) * 0.3f;
        //p.color.b = (cos(deltaTime) * 0.5f + 0.5f) * 0.7f;
        p.lifetime = clamp(p.lifetime - deltaTime * 0.0001f, 0, 5.f);

        if (p.lifetime != 0.f)
            bufferB.Append(p);
   
    }


}