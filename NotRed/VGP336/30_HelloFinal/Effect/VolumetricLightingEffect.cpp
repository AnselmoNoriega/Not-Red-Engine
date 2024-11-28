#include "VolumetricLightingEffect.h"

namespace NotRed::Graphics
{
	void VolumetricLightingEffect::Initialize()
	{
		mTransformBuffer.Initialize();

		std::filesystem::path shaderFile = "../../Assets/Shaders/VolumetricLighting.fx";
		mVertexShader.Initialize<Vertex>(shaderFile);
		mPixelShader.Initialize(shaderFile);
		mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

		mBlendState.Initialize(BlendState::Mode::AlphaBlend);
	}

	void VolumetricLightingEffect::Terminate()
	{
		mBlendState.Terminate();

		mSampler.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();

		mTransformBuffer.Terminate();
	}

	void VolumetricLightingEffect::Render(const std::vector<RenderObject>& renderObjects, const Camera& lightCamera)
	{
		RenderDepth();

		mVertexShader.Bind();
		mPixelShader.Bind();
		mSampler.BindPS(0);

		mBlendState.Set();

		SimpleVolumeTransformData data;
		data.wvp = mCamera;

		for (auto& obj : renderObjects)
		{
			obj.meshBuffer.Render();
		}
	}

	void VolumetricLightingEffect::SetCamera(const Camera& camera)
	{
		mCamera = &camera;
	}

	void VolumetricLightingEffect::AddObjectForShadows(const RenderObject& obj)
	{
		mObjects.push_back(&obj);
	}

	void VolumetricLightingEffect::RenderDepth()
	{

	}

	void VolumetricLightingEffect::DebugUI()
	{
	}
}