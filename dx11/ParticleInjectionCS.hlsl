#define INJECTION_COUNT 8

struct Particle
{
    float3 pos;
    float lifetime;
    float3 color;
    float3 velocity;
};

AppendStructuredBuffer<Particle> bufferA : register(u0);


[numthreads(INJECTION_COUNT, 4, 2)]
void CSMAIN( uint3 DTid : SV_DispatchThreadID )
{
    float dtidX = (float) DTid.x;
    float dtidY = (float) DTid.y;
    
    Particle p;
    p.color = float3(dtidX / INJECTION_COUNT, 1.f, dtidY / INJECTION_COUNT);
    p.lifetime = (dtidX + 3.f) * 4.f;
    p.pos = float3(dtidX * 0.5f, dtidY * 0.5f, 0.f) + float3(0.f, 4.f, 0.f);
    p.velocity = float3((dtidX + 0.5f) * 0.6f, (dtidX + 0.5f) * 0.4f, (dtidX + 0.5f) * 0.9f);
    
    bufferA.Append(p);

}