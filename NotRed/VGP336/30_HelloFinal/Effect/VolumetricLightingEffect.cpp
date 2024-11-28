#include "VolumetricLightingEffect.h"

namespace NotRed::Graphics
{
    void VolumetricLightingEffect::Initialize(const std::filesystem::path& filePath)
    {
        mTransformBuffer.Initialize();

        mVertexShader.Initialize<Vertex>(filePath);
        mPixelShader.Initialize(filePath);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void VolumetricLightingEffect::Terminate()
    {
        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
        
        mTransformBuffer.Terminate();
    }

    void VolumetricLightingEffect::Begin()
    {
        mVertexShader.Bind();
        mPixelShader.Bind();

        mSampler.BindVS(0);
        mSampler.BindPS(0);

        mTransformBuffer.BindVS(0);
    }

    void VolumetricLightingEffect::End()
    {
    }

    void VolumetricLightingEffect::Render(const RenderObject& renderObject)
    {
    }

    void VolumetricLightingEffect::DebugUI()
    {
    }
}