#pragma once

#include "Component.h"

namespace NotRed
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
	};
}