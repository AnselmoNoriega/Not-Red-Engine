#pragma once

#include "CustomTypeIDs.h"

namespace NotRed
{
	class CameraService;
	class RenderObjectComponent;
	class TransformComponent;

	class VolumetricLightingService final : public Service
	{
	public:
		SET_TYPE_ID(CustomServiceID::VolumeRenderer);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;

		void Render() override;
		void DebugUI() override;

		void Register(const RenderObjectComponent* renderObjectComponent);
		void Unregister(const RenderObjectComponent* renderObjectComponent);

	private:
		struct Entry
		{
			const RenderObjectComponent* renderComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		using RenderEntries = std::vector<Entry>;

	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		RenderEntries mRenderEntries;
		float mFPS = 0.0f;
	};
}