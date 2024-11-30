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

    void VolumetricLightingEffect::Render(const RenderObject& renderObject, const RenderObject& renderTarget)
    {
        RenderDepth(renderObject);

        mVertexShader.Bind();
        mPixelShader.Bind();
        mSampler.BindPS(0);

        mBlendState.Set();

        //TODO: Set all textures for shader

        SimpleVolumeTransformData transformData;
        transformData.world = Math::Transpose(renderObject.transform.GetMatrix());
        transformData.viewDir = mCamera->GetDirection();

        mTransformBuffer.Update(transformData);

        renderTarget.meshBuffer.Render();
    }

    void VolumetricLightingEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void VolumetricLightingEffect::RenderDepth(const RenderObject& renderObject)
    {
        // TODO: Change to render with the transform fx
        mVertexShader.Bind();
        mPixelShader.Bind();
        mSampler.BindPS(0);

        mBlendState.Set();

        Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = matWorld * matView * matProj;

        SimpleVolumeTransformData transformData;
        transformData.wvp = Math::Transpose(matFinal);
        transformData.world = Math::Transpose(matWorld);
        transformData.viewDir = mCamera->GetDirection();

        //const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
        //const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
        //transformData.lwvp = Transpose(matWorld * matLightView * matLightProj);

        mTransformBuffer.Update(transformData);

        renderObject.meshBuffer.Render();
    }

    void VolumetricLightingEffect::DebugUI()
    {
    }
}