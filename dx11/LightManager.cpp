#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::LightManager(ComPtr<ID3D11Buffer> structuredBuffer, unsigned int numLights) :
	lightsBuffer(structuredBuffer),
	numLights(numLights)
{
	pointLightData.reserve(numLights);
}

LightManager::~LightManager()
{
}

void LightManager::AddLight(std::shared_ptr<PointLight> newLight)
{
	PointLightHash hash = { newLight };

	pointLightData.push_back(newLight->GetPointLightData());
	pointLights.insert( { newLight->GetID(), hash } );
}

bool LightManager::RemoveLight(const std::string& identifier)
{
	if (pointLights.erase(identifier) == 0)
	{
		return false;
	}

	pointLightData.pop_back();
	return true;
}

void LightManager::UpdateLightData()
{
	// Fill the data, order doesn't matter (we are simply accumulating light result in shader)
	// Assumes that pointLightData size == pointLights size
	unsigned int i = 0;
	for (const auto& light : pointLights)
	{
		pointLightData[i++] = light.second.light->GetPointLightData();
	}

}


