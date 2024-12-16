#pragma once

#include "RenderObjectComponent.h"
#include "Graphics/Inc/VolumetricLightingEffect.h"

namespace NotRed
{
	class TransformComponent;

	class VolumetricLightComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::VolumetricLight);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;

		void Deserialize(const rapidjson::Value& value) override;

		void DebugUI() override;

		inline const Graphics::SpotLight& GetLight() const
		{
			return mLight;
		}

		inline const Graphics::RenderObject& GetLightModelFront() const
		{
			return mLightObj;
		}

		inline const Graphics::RenderObject& GetLightModelBack() const
		{
			return mInLightObj;
		}

	private:
		Graphics::RenderObject mLightObj;
		Graphics::RenderObject mInLightObj;

		TransformComponent* mTransformComponent = nullptr;

		Graphics::SpotLight mLight;

		float mTimeSpeed = 1.0f;
	};
}