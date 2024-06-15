#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "BlendState.h"
#include "RenderTarget.h"

namespace NotRed::Graphics
{
	class Camera;
	class Texture;
	struct RenderObject;

	class WaterEffect final
	{
	public:
		void Initialize();
		void Begin();
		void RenderEffect(const RenderObject& renderObject);

		void End();
		void Terminate();

		void Update(float dt);

		void Render(const RenderObject& renderObject, const Math::Matrix4& position);
		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

		void SetTextures(const Texture* renderTarget, const Texture* depthbuffer);

		void RenderDepth(const RenderObject& renderObject, const Math::Matrix4& position);

	private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 lwvp;
            Math::Matrix4 lfwvp;
            Math::Matrix4 world;
            Math::Vector3 viewPos;
			Math::Vector3 viewPosition;
            float padding[2];
        };

		struct DepthTransform
		{
			Math::Matrix4 wvp;
		};

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useSpecMap = 1;
            int useShadowMap = 1;
            float depthBias = 0.0f;
        };

        struct WaterData
        {
			Math::Vector4 waves[3] = {{0.85f, 0.8f, 0.20f, 9},
									  {2.35f, 1.0f, 0.25f, 20},
									  {2.75f, 1.1f, 0.05f, 3}};
			float waveMovementTime = 0.0f;
			float waveStrength = 1.35f;
			float padding[2];
        };
		
        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using DepthTransformBuffer = TypedConstantBuffer<DepthTransform>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using WaveBuffer = TypedConstantBuffer<WaterData>;

		TransformBuffer mTransformBuffer;
		DepthTransformBuffer mDepthTransformBuffer;
		SettingsBuffer mSettingsBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		WaveBuffer mWaveBuffer;

		VertexShader mVertexShader;
		GeometryShader mGeometryShader;
		PixelShader mPixelShader;

		VertexShader mDepthVertexShader;
		PixelShader mDepthPixelShader;

		VertexShader mEffectVertexShader;
		PixelShader mEffectPixelShader;

		BlendState mBlendState;
		Sampler mSampler;

		SettingsData mSettingsData;
		WaterData mWaterData;

		RenderTarget mWaterTarget;
		RenderTarget mWaterDepth;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		std::array<const Texture*, 4> mTextures;

		float mTimeMultiplier = 1.75f;
	};
}