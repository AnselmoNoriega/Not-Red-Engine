#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace NotRed::Graphics
{
	class Camera;
	class Texture;
	struct RenderObject;

	class TerrainEffect final
	{
	public:
		void Initialize();
		void Begin();

		void End();
		void Terminate();

		void Render(const RenderObject& renderObject, const Math::Matrix4& position);
		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 lwvp;
			Math::Matrix4 world;
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData
		{
			int useNormalMap = 0;
			int useSpecMap = 0;
			int useShadowMap = 0;
			int useBlend = 0;
			float depthBias = 0.00001f;
			float blendHeight = 0.0f;
			float blendWidth = 0.0f;
			float padding = 0.0f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;

		TransformBuffer mTransformBuffer;
		SettingsBuffer mSettingsBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}