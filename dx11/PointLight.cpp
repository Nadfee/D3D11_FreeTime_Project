#include "PointLight.h"

PointLight::PointLight() :
	data{
	Vector3(0.f, 0.f, 0.f),
	Vector3(0.f, 0.f, 0.f),
	Vector3(0.f, 0.f, 0.f)
	},
	shouldUpdate(true)
{
}

PointLight::PointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initAttenuation) :
	id(identifier),
	data{
	initPos,
	initColor,
	initAttenuation
	},
	shouldUpdate(true)
{
}

PointLight::~PointLight()
{
}
