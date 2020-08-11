#pragma once
#include "PointLight.h"
#include "DXDeviceManager.h"

#include <unordered_map>
#include <memory>
#include <vector>

using PointLightPtr = std::shared_ptr<PointLight>;

struct PointLightHash
{
	PointLightPtr light;

	bool operator==(PointLight const& other) const { return light->GetID() == other.GetID(); };
};

class LightManager
{
public:
	LightManager();
	LightManager(ComPtr<ID3D11Buffer> structuredBuffer, unsigned int numLights);
	~LightManager();
			
	void AddLight(std::shared_ptr<PointLight> newLight);
	bool RemoveLight(const std::string& identifier);

	unsigned int GetMaxLightsCount() { return numLights; }
	ComPtr<ID3D11Buffer> GetLightsBuffer() { return lightsBuffer; }
	const std::vector<PointLightData>& GetPointLightData() { return pointLightData; };
	

	void SetBufferView(ComPtr<ID3D11ShaderResourceView> bufferView) { lightsBufferView = bufferView; }
	const ComPtr<ID3D11ShaderResourceView>& GetBufferView() { return lightsBufferView; }

	void UpdateLightData();

private:

	// Data storage in Vector and Unordered Map used to re-arrange the data accordingly
	std::vector<PointLightData> pointLightData;
	std::unordered_map<std::string, unsigned int> pointLightsDataOffsets;

	std::unordered_map<std::string, PointLightHash> pointLights;

	ComPtr<ID3D11Buffer> lightsBuffer;
	ComPtr<ID3D11ShaderResourceView> lightsBufferView;

	unsigned int numLights;


};

