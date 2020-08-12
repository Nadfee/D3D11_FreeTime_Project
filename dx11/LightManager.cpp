#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::LightManager(ComPtr<ID3D11Buffer> structuredBuffer, unsigned int maxLights) :
	lightsBuffer(structuredBuffer),
	maxLights(maxLights)
{
	pointLightData.reserve(maxLights);
}

LightManager::~LightManager()
{
}

void LightManager::AddLight(std::shared_ptr<PointLight> newLight)
{
	PointLightHash hash = { newLight };

	pointLightsDataOffsets.insert({ newLight->GetID(), pointLightData.size() });		// Index location stored!
	pointLightData.push_back(newLight->GetPointLightData());

	pointLights.insert( { newLight->GetID(), hash } );
}

bool LightManager::RemoveLight(const std::string& identifier)
{
	if (pointLights.erase(identifier) == 0)
	{
		return false;
	}

	// Here we want to swap current data with last data and pop and fix the offset of the swapped thing.
	unsigned int idxToDelete = pointLightsDataOffsets.find(identifier)->second;
	
	// Duplicate data onto the deletion spot and remove the duplicated old one
	pointLightData[idxToDelete] = pointLightData.back();


	// Update the offset for the swapped data
	for (auto& pair : pointLightsDataOffsets)
	{
		// If we find the one that was responsible for the last data offset
		if (pair.second == pointLightData.size() - 1)
		{
			// Give it the new offset!
			pair.second = idxToDelete;
			break;
		}
	}

	// Finally erase
	pointLightData.pop_back();
	pointLightsDataOffsets.erase(identifier);

	//pointLightData.pop_back();
	return true;
}

void LightManager::UpdateLightData()
{
	// Fill the data, order doesn't matter (we don't need to identify lights in shader)
	// Assumes that pointLightData size == pointLights size
	// Add and Remove (especially) ensures that only data that should be sent are sent!
	unsigned int i = 0;
	for (const auto& pair : pointLights)
	{
		OutputDebugStringW(L"Updating\n");
		if (pair.second.light->ShouldUpdate())
		{
			pointLightData[i++] = pair.second.light->GetPointLightData();
		}
		else
		{
			pointLightData[i++] = { Vector3(0.f), Vector3(0.f), Vector3(0.f, -1.f, 0.f)};		// Radius still sent, temp so that we can still loop in HLSL
		}
	}

}


