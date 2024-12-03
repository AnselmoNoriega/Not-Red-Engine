#pragma once

#include "Colors.h"

namespace NotRed::Graphics
{
	struct DirectionalLight
	{
		Color ambient = Colors::Wheat;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
		Math::Vector3 direction = -Math::Vector3::YAxis;
		float padding = 0.0f;
	};

	struct PointLight
	{

	};

	//struct SpotLight
	//{

	//};
}