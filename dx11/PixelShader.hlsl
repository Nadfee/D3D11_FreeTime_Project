struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float2 uv : TEXCOORD;
    float3 nor : NORMAL;
};

struct PointLight
{
    float3 lightPosition;
    float3 lightColor;
    float3 attenuation;
};

Texture2D diffuseTexture : register(t0);
StructuredBuffer<PointLight> lightBuffer : register(t1);
SamplerState defaultSampler : register(s0);

float4 PSMAIN(PS_IN input) : SV_TARGET
{
    //return float4(input.worldPos, 1.f);

    
    float4 textureSample = diffuseTexture.Sample(defaultSampler, input.uv);
    //textureSample = float4(1.f, 1.f, 1.f, 1.f);
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    float3 normal = normalize(input.nor);
    
    finalColor += 0.10f * textureSample;     // ambient
    
    uint lightCount, size;
    lightBuffer.GetDimensions(lightCount, size);
    for (uint i = 0; i < lightCount; ++i)
    {
        if (length(lightBuffer[i].lightColor) == 0.f)
        {
            break;
        }
        
        // temporary, we set radius to -1 to indicate a light that shouldn't be calculated but exists
        if (lightBuffer[i].attenuation.x >= 0.f)
        {
            float attenuationFactor[3] = { lightBuffer[i].attenuation.x, lightBuffer[i].attenuation.y, lightBuffer[i].attenuation.z };
        
            float3 posToLight = lightBuffer[i].lightPosition - input.worldPos;
            float distanceToLight = length(posToLight);
            float3 posToLightDir = normalize(posToLight);
       
            float distFactor = saturate(-distanceToLight / lightBuffer[i].attenuation.y + 1.f);
        
            float diffuseFactor = saturate(dot(posToLightDir, normal));
        
            //finalColor += diffuseFactor * distFactor * textureSample;
        
            float4 diffuseColor = diffuseFactor * (textureSample + float4(lightBuffer[i].lightColor, 0.f));
            diffuseColor /= attenuationFactor[0] + (attenuationFactor[1] * distanceToLight) + (attenuationFactor[2] * distanceToLight * distanceToLight);
        
            finalColor += diffuseColor;
        
            //finalColor += diffuseFactor * (textureSample);
            //finalColor /= attenuationFactor[0] + (attenuationFactor[1] * distanceToLight) + (attenuationFactor[2] * distanceToLight * distanceToLight);

        }
        
    }
    
    
    //return float4(normalize(input.nor.xyz), 1.f);
	//return float4(input.uv.xy, 0.f, 1.f);
    // return diffuseTexture.Sample(defaultSampler, input.uv);
    return finalColor;
    


}