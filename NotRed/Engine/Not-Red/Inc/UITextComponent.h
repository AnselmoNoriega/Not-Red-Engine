#pragma once

#include "UIComponent.h"

namespace NotRed
{
	class UITextComponent :public UIComponent
	{
	public:
		SET_TYPE_ID(ComponentId::UIText);

		void Initialize() override;
		void Terminate() override;
		void Render() override;

		void Deserialize(const rapidjson::Value& value) override;

	private:
		std::filesystem::path mText;
		Math::Vector2 mPosition = Math::Vector2::Zero;
		float mSize = 1.0f;
		Color mColor = Colors::Black;
	};
}