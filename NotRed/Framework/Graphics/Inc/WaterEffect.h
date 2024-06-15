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
#include "Texture.h"

namespace NotRed::Graphics
{
	class Camera;
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

		struct LightData
		{
			Math::Vector3 lightDirection;
			float padding;
		};

        struct WaterData
        {
			Math::Vector4 waves[4] = {{0.85f, 0.8f, 0.20f, 9},
									  {2.35f, 1.0f, 0.25f, 20},
									  {2.75f, 1.1f, 0.05f, 3},
									  {0.70f, 1.3f, 0.05f, 4.65}};
			float waveMovementTime = 0.0f;
			float waveStrength = 1.35f;
			float padding[2];
        };
		
        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using DepthTransformBuffer = TypedConstantBuffer<DepthTransform>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using WaveBuffer = TypedConstantBuffer<WaterData>;
        using LightBuffer = TypedConstantBuffer<LightData>;

		TransformBuffer mTransformBuffer;
		DepthTransformBuffer mDepthTransformBuffer;
		MaterialBuffer mMaterialBuffer;
		WaveBuffer mWaveBuffer;
		LightBuffer mLightBuffer;

		VertexShader mVertexShader;
		GeometryShader mGeometryShader;
		PixelShader mPixelShader;

		VertexShader mDepthVertexShader;
		PixelShader mDepthPixelShader;

		VertexShader mEffectVertexShader;
		PixelShader mEffectPixelShader;

		BlendState mBlendState;
		Sampler mSampler;

		WaterData mWaterData;
		LightData mLightData;

		RenderTarget mWaterTarget;
		RenderTarget mWaterDepth;

		const Camera* mCamera = nullptr;
		const Texture* mShadowMap = nullptr;

		std::array<const Texture*, 5> mTextures;
		Texture mFoam;

		float mTimeMultiplier = 1.75f;
	};
}