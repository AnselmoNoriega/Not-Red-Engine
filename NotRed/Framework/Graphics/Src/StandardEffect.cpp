#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

namespace NotRed::Graphics
{
    void StandardEffect::Initialize(const std::filesystem::path& filePath)
    {
        mConstantBuffer.Initialize(sizeof(Math::Matrix4));
        mVertexShader.Initialize<VertexPX>(filePath);
        mPixelShader.Initialize(filePath);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void StandardEffect::Terminate()
    {
        mVertexShader.Terminate();
        mPixelShader.Terminate();
        mSampler.Terminate();
        mConstantBuffer.Terminate();
    }

    void StandardEffect::Begin()
    {
        ASSERT(mCamera != nullptr, "No camera passed!");

        mVertexShader.Bind();
        mPixelShader.Bind();

        mSampler.BindVS(0);
        mSampler.BindPS(0);

        mConstantBuffer.BindVS(0);
    }

    void StandardEffect::End()
    {
    }

    void StandardEffect::Render(const RenderObject& renderObject)
    {
        const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = matWorld * matView * matProj;
        matFinal = Math::Transpose(matFinal);
        mConstantBuffer.Update(&matFinal);

        renderObject.texture.BindPS(0);
        renderObject.meshBuffer.Render();
    }

    void StandardEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void StandardEffect::DebugUI()
    {
        if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
        {

        }
    }
}