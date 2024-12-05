#pragma once

#include <Not-Red/Inc/NotRed.h>

namespace NotRed::Graphics
{
	class Camera;
	class Texture;
	struct RenderObject;

	struct SpotLight
	{
		SpotLight()
		{
			cameraObj.SetPosition({ 0.0f, 10.0f, 0.0f });
			cameraObj.SetDirection({ 0.0f, -0.99f, 0.1f });
		}

		Camera cameraObj;
	};

	class VolumetricLightingEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Render(const RenderObject& renderObject, const RenderObject& inRenderObject, const RenderObject& renderTarget);

		void DebugUI();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		inline void SetTextures(const Texture* renderTarget, const Texture* depthTarget)
		{
			mGeometryTexture = renderTarget;
			mGeometryPositionTetxure = depthTarget;
		}

		inline void RegisterObject(const RenderGroup& obj)
		{
			mCharacters.push_back(&obj);
		}

	private:
		void RenderDepth(const RenderObject& renderObject, RenderTarget& target);
		void RenderLightCam();

	private:
		struct SimpleVolumeTransformData
		{
			Math::Matrix4 world;
			Math::Vector3 viewDir;
			Math::Matrix4 LightViewProj;
			Math::Matrix4 viewMatrix;
			float padding = 0.0f;
		};

		struct SimpleLightTransformData
		{
			Math::Matrix4 modelTransform;
			Math::Matrix4 viewMatrix;
			Math::Matrix4 viewProjectionMatrix;
		};

		using SimpleVolumeTransformBuffer = TypedConstantBuffer<SimpleVolumeTransformData>;
		using SimpleLightTransformBuffer = TypedConstantBuffer<SimpleLightTransformData>;

	private:

		SimpleVolumeTransformBuffer mTransformBuffer;
		SimpleLightTransformBuffer mLightTransformBuffer;

		Sampler mSampler;
		BlendState mBlendState;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		VertexShader mLightVertexShader;
		PixelShader mLightPixelShader;

		const Camera* mCamera = nullptr;

		const Texture* mGeometryTexture = nullptr;
		const Texture* mGeometryPositionTetxure = nullptr;

		RenderTarget mLightGeometryTarget;
		RenderTarget mLightInGeometryTarget;
		RenderTarget mLightViewTarget;

		std::vector<const RenderGroup*> mCharacters;
	};
}