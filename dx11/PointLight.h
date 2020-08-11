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

	const PointLightData& GetPointLightData() { return data; }
	const Vector3 GetPosition() { return data.position; }
	const float GetRadius() { return data.radius; }
	const Vector3 GetColor() { return data.color; }
	const std::string& GetID() const { return id; }


private:
	std::string id;
	PointLightData data;
	bool shouldUpdate;



};

