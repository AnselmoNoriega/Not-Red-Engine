#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Sampler.h"
#include "VertexShader.h"

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
            Math::Matrix4 lfwvp;
            Math::Matrix4 world;
            Math::Vector3 viewPos;
			Math::Vector3 viewPosition;
            float padding[2];
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            //int useNormalMap = 1;
            int useSpecMap = 1;
            //int useLighting = 1;
            int useShadowMap = 1;
            float depthBias = 0.0f;
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
		GeometryShader mGeometryShader;
		Sampler mSampler;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}