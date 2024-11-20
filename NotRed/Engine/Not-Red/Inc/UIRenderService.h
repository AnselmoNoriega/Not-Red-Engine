#pragma once

#include "Service.h"

namespace NotRed
{
	class UIComponent;

	class UIRenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::UIRender);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;

		void Register(UIComponent* uiComponent);
		void Unregister(UIComponent* uiComponent);

	private:
		using UIComponents = std::vector<UIComponent*>;
		UIComponents mUIComponents;
	};
}