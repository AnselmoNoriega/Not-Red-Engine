#include "Precompiled.h"
#include "DepthEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

namespace NotRed::Graphics
{
    void DepthEffect::Initialize()
    {
        std::filesystem::path shaderFile = "../../Assets/Shaders/Depth.fx";

        mTransformBuffer.Initialize();
        mVertexShader.Initialize<Vertex>(shaderFile);
        mPixelShader.Initialize(shaderFile);

        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void DepthEffect::Terminate()
    {
        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
        mTransformBuffer.Terminate();
    }

    void DepthEffect::Begin()
    {
        mVertexShader.Bind();
        mPixelShader.Bind();

        mSampler.BindVS(0);
        mSampler.BindPS(0);

        mTransformBuffer.BindVS(0);
        mTransformBuffer.BindPS(0);
    }

    void DepthEffect::End()
    {
    }

    void DepthEffect::Render(const RenderObject& renderObject)
    {
        Transform transformData;
        transformData.modelTransform = renderObject.transform.GetMatrix();
        transformData.viewMatrix = mCamera->GetViewMatrix();
        transformData.viewProjectionMatrix = mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix();
        mTransformBuffer.Update(transformData);

        renderObject.meshBuffer.Render();
    }

    void DepthEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void DepthEffect::DebugUI()
    {
    }
}