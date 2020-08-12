#pragma once
#include <SimpleMath.h>
#include <string>
#include "DXDeviceManager.h"

using namespace DirectX::SimpleMath;

struct PointLightData
{
	Vector3 position;
	Vector3 color;
	Vector3 attenuation;
};

class PointLight
{
public:
	PointLight();
	PointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initRadius);
	~PointLight();

	const Vector3 GetPosition() { return data.position; }
	const Vector3& GetAttenuation() { return data.attenuation; }		// CHange this!!
	const Vector3 GetColor() { return data.color; }
	const std::string& GetID() const { return id; }
	bool ShouldUpdate() { return shouldUpdate; }

	void SetPosition(float x, float y, float z) { data.position = Vector3(x, y, z); }
	void SetAttenuation(float constant, float linear, float quadratic) { data.attenuation = Vector3(constant, linear, quadratic); }				// Change this!!
	void SetColor(float r, float g, float b) { data.color = Vector3(r, g, b); }
	void SetUpdateState(bool state) { shouldUpdate = state; }



private:

	friend class LightManager;
	friend class Graphics;
	const PointLightData& GetPointLightData() { return data; }

	std::string id;
	PointLightData data;
	bool shouldUpdate;



};

