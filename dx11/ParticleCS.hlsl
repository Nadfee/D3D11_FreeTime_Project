struct Particle
{
    float3 pos;
    float lifetime;
    float3 color;
    float3 velocity;
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

static const float G = 10.0f;
static const float m1 = 1.0f;
static const float m2 = 1000.0f;
static const float m1m2 = m1 * m2;
static const float eventHorizon = 0.4f;

#define MaxParticleCount 1024

[numthreads(MaxParticleCount, 1, 1)]
void CSMAIN( uint3 DTid : SV_DispatchThreadID )
{
//    // Check for if this thread should run or not.
//    uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;

    
//    if (myID < numParticles.x)
//    {
            
//        Particle p = bufferA.Consume();

//        if (p.lifetime > 0.f)
//        {
//            bufferB.Append(p);
            
//        }
   
//    }
    
    
    //// Check for if this thread should run or not.
    //uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;

    
    //if (myID < numParticles.x)
    //{
            
    //    Particle p = bufferA.Consume();

    //    p.pos += p.velocity * 0.0007f;
        
    //    //if (p.pos.y > 4.f)
    //    //{
    //    //    p.velocity.y = -p.velocity.y;
    //    //}
        
    //    //if (p.pos.y < -4.f)
    //    //{
    //    //    p.velocity.y = -p.velocity.y;
    //    //}
        
    //    //if (p.pos.z > 4.f)
    //    //{
    //    //    p.velocity.z = -p.velocity.z;
    //    //}
        
    //    //if (p.pos.z < -4.f)
    //    //{
    //    //    p.velocity.z = -p.velocity.z;
    //    //}
        
    //    //if (p.pos.x > 4.f)
    //    //{
    //    //    p.velocity.x = -p.velocity.x;
    //    //}
        
    //    //if (p.pos.x < -4.f)
    //    //{
    //    //    p.velocity.x = -p.velocity.x;
    //    //}
        
    //    if (p.pos.y > 1.f)
    //    {
    //        p.velocity.y = -p.velocity.y;
    //    }
        
    //    if (p.pos.y < -1.f)
    //    {
    //        p.velocity.y = -p.velocity.y;
    //    }
        
    //    if (p.pos.z > 1.f)
    //    {
    //        p.velocity.z = -p.velocity.z;
    //    }
        
    //    if (p.pos.z < -1.f)
    //    {
    //        p.velocity.z = -p.velocity.z;
    //    }
        
    //    if (p.pos.x > 1.f)
    //    {
    //        p.velocity.x = -p.velocity.x;
    //    }
        
    //    if (p.pos.x < -1.f)
    //    {
    //        p.velocity.x = -p.velocity.x;
    //    }

        
    //    p.lifetime -= deltaTime * 0.0000001f;
        
    //    if (p.lifetime > 0.f)
    //    {
    //        bufferB.Append(p);
            
    //    }
   
    //}
    
    
    //// Check for if this thread should run or not.
    //uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;
    
    //float counterIn = deltaTime + ((float) myID / (float) numParticles.x) * MaxParticleCount;
    
    //float counter2 = counterIn / 2.f;
    
    
    //if (myID < numParticles.x)
    //{
            
    //    Particle p = bufferA.Consume();
        
    //    float radius = p.lifetime * p.lifetime + length(p.color) * length(p.color);
        
    //    // p.pos.y = wave + DTid.x;     // cool effect
    //    //p.pos.y = cos(wave);
        
    //    // Spherical coord
    //    //p.pos.x = radius * cos(counterIn) * sin(counter2);
    //    //p.pos.z = (radius * sin(counterIn) * sin(counter2));
    //    //p.pos.y = -radius * cos(counterIn) + 2.f * sin(deltaTime) + 5.f;
        
    //    // Cool too
    //    //p.pos.x = radius * cos(counter2) * sin(counter2);
    //    //p.pos.z = (radius * sin(counterIn) * sin(counterIn));
    //    //p.pos.y = -radius * cos(counter2) + 2.f * sin(deltaTime) + 5.f;
        
    //    //p.pos.x = radius * cos(counter2) * sin(counter2);
    //    //p.pos.z = (radius * cos(counterIn) * sin(counterIn));
    //    //p.pos.y = -radius * cos(counter2) + 2.f * cos(deltaTime) + 5.f;
        
    //    p.pos.x = radius * cos(counterIn) * sin(counter2);
    //    p.pos.z = (radius * cos(counterIn) * sin(counter2));
    //    p.pos.y = -radius * cos(counterIn * 5.f) + 2.f * cos(deltaTime) + 14.f;
        
    //    p.pos += p.velocity;
    
    //    // Cool
    //    //p.pos.x = radius * cos(counterIn) * sin(counter2);
    //    //p.pos.z = (radius * sin(counter2) * sin(counter2));
    //    //p.pos.y = -radius * cos(counterIn) + 2.f * sin(deltaTime) + 5.f;

    //    //p.color.r = cos(deltaTime) * 0.5f + 0.5f;
    //    //p.color.g = (sin(deltaTime) * 0.5f + 0.5f) * 0.3f;
    //    //p.color.b = (cos(deltaTime) * 0.5f + 0.5f) * 0.7f;
    //    p.lifetime = clamp(p.lifetime - deltaTime * 0.0001f, 0, 5.f);

    //    if (p.lifetime > 0.f)
    //        bufferB.Append(p);
   
    //}
    
    // Book
    static const float G = 10.0f;
    static const float m1 = 1.0f;
    static const float m2 = 50.0f;
    static const float m1m2 = m1 * m2;
    static const float eventHorizon = 0.15f;
    
    float3 ConsumerLocation = float3(0.f, 0.f, 0.f);
    

    // Check for if this thread should run or not.
    uint myID = DTid.x + DTid.y * MaxParticleCount + DTid.z * MaxParticleCount * MaxParticleCount;
    
    if (myID < numParticles.x)
    {
        // Get the current particle
        Particle p = bufferA.Consume();
        
        // Calculate the current gravitational force applied to it
        float3 d = ConsumerLocation.xyz - p.pos;
        float r = length(d);


        float3 Force = (G * m1m2 / (r * r)) * normalize(d);
        // Calculate the new velocity, accounting for the acceleration from
        // the gravitational force over the current time step.
        p.velocity = p.velocity + (Force / m1) * deltaTime * 0.001f;
        
        // Calculate the new position, accounting for the new velocity value
        // over the current time step.
        p.pos += p.velocity * deltaTime * 0.001f;
        
        // Update the life time left for the particle,
        p.lifetime -= deltaTime * 0.0000001f;
        //Test to see how close the particle is to the black hole, and
        //don't pass it to the output list if it is too close,
        
        //float3 col = p.pos;
        //if (p.pos.x < 0.f)
        //{
        //    col.x = -p.pos.x;
        //}
        //if (p.pos.x < 0.f)
        //{
        //    col.y = -p.pos.y;
        //}
        //if (p.pos.x < 0.f)
        //{
        //    col.z = -p.pos.z;
        //}
        
        //p.color = col;
        
        if (r > eventHorizon)
        {
            if (p.lifetime > 0.f)
            {
                bufferB.Append(p);
            }
        }
    }
    
}