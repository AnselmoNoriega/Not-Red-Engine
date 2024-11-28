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

        Math::Matrix4 matWorld;
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal;

        SimpleVolumeTransformData transformData;
        transformData.viewDir = mCamera->GetDirection();

        //const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
        //const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
        //transformData.lwvp = Transpose(matWorld * matLightView * matLightProj);

        mTransformBuffer.Update(transformData);

        for (auto& obj : renderObjects)
        {
            matWorld = obj.transform.GetMatrix();
            matFinal = matWorld * matView * matProj;

            transformData.wvp = Math::Transpose(matFinal);
            transformData.world = Math::Transpose(matWorld);

            mTransformBuffer.Update(transformData);

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