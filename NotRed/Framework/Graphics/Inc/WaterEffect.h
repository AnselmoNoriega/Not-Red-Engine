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

		void Terminate();

		void Update(float dt);
		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

		void SetTextures(const Texture* renderTarget, const Texture* depthbuffer);

		void RenderWater(const RenderObject& renderObject, const Math::Matrix4& position, const RenderObject& renderTarget);

	private:
		void RenderDepth(const RenderObject& renderObject, const Math::Matrix4& position);
		void RenderNormal(const RenderObject& renderObject, const Math::Matrix4& position);
		void RenderEffect(const RenderObject& renderObject);

	private:
		struct SimpleTransform
		{
			Math::Matrix4 wvp;
		};

		struct RefractionHelper
		{
			Math::Vector3 lightDirection;
			Math::Vector4 lightColor;
			float time;
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
		
        using SimpleTransformBuffer = TypedConstantBuffer<SimpleTransform>;
        using WaveBuffer = TypedConstantBuffer<WaterData>;
        using RefractionHelperBuffer = TypedConstantBuffer<RefractionHelper>;

		SimpleTransformBuffer mSimpleTransformBuffer;
		WaveBuffer mWaveBuffer;
		RefractionHelperBuffer mRefractionHelperBuffer;

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
		RefractionHelper mRefractionHelper;

		RenderTarget mWaterTarget;
		RenderTarget mWaterDepth;

		const Camera* mCamera = nullptr;
		const Texture* mShadowMap = nullptr;

		std::array<const Texture*, 5> mTextures;
		Texture mFoam;

		float mTimeMultiplier = 1.75f;

		float mTime = 0.0f;
		float mAnimationChangeTime = 0.02f;
		std::vector<size_t> mAnimatedTexture;
		uint8_t mTextureIndex = 0;
	};
}