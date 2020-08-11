#include "PointLight.h"

PointLight::PointLight() :
	data{
	Vector3(0.f, 0.f, 0.f),
	Vector3(0.f, 0.f, 0.f),
	0
	},
	shouldUpdate(true)
{
}

PointLight::PointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, float initRadius) :
	id(identifier),
	data{
	initPos,
	initColor,
	initRadius
	},
	shouldUpdate(true)
{
}

PointLight::~PointLight()
{
}
