#pragma once
#include "Service.h"

namespace NotRed
{
	class CameraService;
	class RenderObjectComponent;
	class TransformComponent;
	class VolumetricLightComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;

		void Render() override;
		void DebugUI() override;

		void Register(const RenderObjectComponent* renderObjectComponent);
		void Unregister(const RenderObjectComponent* renderObjectComponent);

		void Register(const VolumetricLightComponent* volumeObjectComponent);
		void Unregister(const VolumetricLightComponent* volumeObjectComponent);

	private:
		struct Entry
		{
			const RenderObjectComponent* renderComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		using RenderEntries = std::vector<Entry>;

		struct VolumeEntry
		{
			const VolumetricLightComponent* renderComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
		};

		using VolumeRenderEntries = std::vector<VolumeEntry>;

	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::VolumetricLightingEffect mVolumetricLighting;
		Graphics::ShadowEffect mShadowEffect;
		Graphics::DepthEffect mDepthEffect;
		Graphics::NormalsEffect mNormalsEffect;

		RenderEntries mRenderEntries;
		VolumeRenderEntries mRenderVolumes;
		float mFPS = 0.0f;

		//PostProcessing
		NotRed::Graphics::RenderTarget mRenderTarget;
		NotRed::Graphics::RenderTarget mDepthBuffer;
		NotRed::Graphics::RenderTarget mNormalsBuffer;
		NotRed::Graphics::RenderTarget mRenderTargetHelper;
		
		NotRed::Graphics::VertexShader mVertexShader;
		NotRed::Graphics::PixelShader mPixelShader;
		NotRed::Graphics::Sampler mSampler;
		NotRed::Graphics::RenderTarget mPostProcessingTargets[2];

		//ScreenQuad
		NotRed::Graphics::RenderObject mScreenQuad;
	};
}