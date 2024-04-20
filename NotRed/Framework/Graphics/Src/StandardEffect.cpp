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
        mMaterialBuffer.Initialize();
        mVertexShader.Initialize<Vertex>(filePath);
        mPixelShader.Initialize(filePath);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void StandardEffect::Terminate()
    {
        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
        mMaterialBuffer.Terminate();
        mLightBuffer.Terminate();
        mSettingsBuffer.Terminate();
        mTransformBuffer.Terminate();
    }

    void StandardEffect::Begin()
    {
        ASSERT(mCamera != nullptr, "No camera passed!");
        ASSERT(mDirectionalLight != nullptr, "No light passed!");

        mVertexShader.Bind();
        mPixelShader.Bind();

        mSampler.BindVS(0);
        mSampler.BindPS(0);

        mTransformBuffer.BindVS(0);

        mSettingsBuffer.BindPS(1);

        mLightBuffer.BindVS(2);
        mLightBuffer.BindPS(2);

        mMaterialBuffer.BindPS(3);
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
        settingsData.useDiffuseMap = renderObject.diffuseMapID > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 0;
        settingsData.useNormalMap = renderObject.normalMapID > 0 && mSettingsData.useNormalMap > 0 ? 1 : 0;
        settingsData.useSpecMap = renderObject.specMapID > 0 && mSettingsData.useSpecMap > 0 ? 1 : 0;
        settingsData.useLighting = mSettingsData.useLighting;
        mSettingsBuffer.Update(settingsData);

        mLightBuffer.Update(*mDirectionalLight);
        mMaterialBuffer.Update(renderObject.material);

        TextureManager* tm = TextureManager::Get();
        tm->BindPS(renderObject.diffuseMapID, 0);
        tm->BindPS(renderObject.normalMapID, 1);
        tm->BindPS(renderObject.specMapID, 2);

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
            bool useNormalMap = mSettingsData.useNormalMap > 0;
            if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
            {
                mSettingsData.useNormalMap = static_cast<int>(useNormalMap);
            }
            bool useSpecMap = mSettingsData.useSpecMap > 0;
            if (ImGui::Checkbox("UseSpecMap", &useSpecMap))
            {
                mSettingsData.useSpecMap = static_cast<int>(useSpecMap);
            }
            bool useLighting = mSettingsData.useLighting > 0;
            if (ImGui::Checkbox("UseLighting", &useLighting))
            {
                mSettingsData.useLighting = static_cast<int>(useLighting);
            }
        }
    }
}