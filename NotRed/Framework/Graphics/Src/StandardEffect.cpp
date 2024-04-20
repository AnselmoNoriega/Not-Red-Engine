#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

namespace NotRed::Graphics
{
    void StandardEffect::Initialize(const std::filesystem::path& filePath)
    {
        mTransformBuffer.Initialize();
        mSettingsBuffer.Initialize();
        mLightBuffer.Initialize();
        mVertexShader.Initialize<Vertex>(filePath);
        mPixelShader.Initialize(filePath);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void StandardEffect::Terminate()
    {
        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
        mLightBuffer.Terminate();
        mSettingsBuffer.Terminate();
        mTransformBuffer.Terminate();
    }

    void StandardEffect::Begin()
    {
        ASSERT(mCamera != nullptr, "No camera passed!");

        mVertexShader.Bind();
        mPixelShader.Bind();

        mSampler.BindVS(0);
        mSampler.BindPS(0);

        mTransformBuffer.BindVS(0);

        mSettingsBuffer.BindPS(1);

        mLightBuffer.BindVS(2);
        mLightBuffer.BindPS(2);
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

        TransformData transformData;
        transformData.wvp = Math::Transpose(matFinal);
        transformData.world = Math::Transpose(matWorld);
        transformData.viewPos = mCamera->GetPosition();
        mTransformBuffer.Update(transformData);

        SettingsData settingsData;
        settingsData.useDiffuseMap = renderObject.textureID > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 0;
        mSettingsBuffer.Update(settingsData);

        mLightBuffer.Update(*mDirectionalLight);

        TextureManager* tm = TextureManager::Get();
        tm->BindPS(renderObject.textureID, 0);

        renderObject.meshBuffer.Render();
    }

    void StandardEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
    {
        mDirectionalLight = &directionalLight;
    }

    void StandardEffect::DebugUI()
    {
        if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
            if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
            {
                mSettingsData.useDiffuseMap = static_cast<int>(useDiffuseMap);
            }
        }
    }
}