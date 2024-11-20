#pragma once

#include "UIComponent.h"

namespace NotRed
{
	class UISpriteComponent : public UIComponent
	{
	public:
		SET_TYPE_ID(ComponentId::UISprite);

		void Initialize() override;
		void Terminate() override;
		void Render() override;
		void Deserialize(const rapidjson::Value& value) override;

	private:
		std::filesystem::path mTexturePath;
		Graphics::UISprite mUISprite;
	};
}