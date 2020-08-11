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

PointLight::PointLight(const Vector3& initPos, const Vector3& initColor, float initRadius) :
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
