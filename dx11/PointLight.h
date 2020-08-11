#pragma once
#include <SimpleMath.h>
#include <string>
#include "DXDeviceManager.h"

using namespace DirectX::SimpleMath;

struct PointLightData
{
	Vector3 position;
	Vector3 color;
	float radius;
};

class PointLight
{
public:
	PointLight();
	PointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, float initRadius);
	~PointLight();

	const Vector3 GetPosition() { return data.position; }
	const float GetRadius() { return data.radius; }
	const Vector3 GetColor() { return data.color; }
	const std::string& GetID() const { return id; }

	void SetPosition(float x, float y, float z) { data.position = Vector3(x, y, z); }
	void SetRadius(float newRadius) { data.radius = newRadius; }
	void SetColor(float r, float g, float b) { data.color = Vector3(r, g, b); }


private:

	friend class LightManager;
	friend class Graphics;
	const PointLightData& GetPointLightData() { return data; }

	std::string id;
	PointLightData data;
	bool shouldUpdate;



};

