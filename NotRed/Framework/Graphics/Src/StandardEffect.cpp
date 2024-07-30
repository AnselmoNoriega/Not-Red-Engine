#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtils.h"

namespace NotRed::Graphics
{
    static constexpr size_t MaxBoneCount = 256;

    void StandardEffect::Initialize(const std::filesystem::path& filePath)
    {
        mTransformBuffer.Initialize();
        mSettingsBuffer.Initialize();
        mLightBuffer.Initialize();
        mMaterialBuffer.Initialize();
        mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(Math::Matrix4));
        mVertexShader.Initialize<Vertex>(filePath);
        mPixelShader.Initialize(filePath);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    void StandardEffect::Terminate()
    {
        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
        mBoneTransformBuffer.Terminate();
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

        mSettingsBuffer.BindVS(1);
        mSettingsBuffer.BindPS(1);

        mLightBuffer.BindVS(2);
        mLightBuffer.BindPS(2);

        mMaterialBuffer.BindPS(3);
        mBoneTransformBuffer.BindVS(4);
    }

    void StandardEffect::End()
    {
        if (mShadowMap != nullptr)
        {
            Texture::UnbindPS(4);
        }
    }

    void StandardEffect::Render(const RenderObject& renderObject)
    {
        SettingsData settingsData;
        settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapID > 0;
        settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapID > 0;
        settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapID > 0;
        settingsData.useLighting = mSettingsData.useLighting;
        settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapID > 0;
        settingsData.bumpWeight = mSettingsData.bumpWeight;
        settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
        settingsData.depthBias = mSettingsData.depthBias;
        settingsData.useSkinning = mSettingsData.useSkinning > 0 && renderObject.skeleton != nullptr;
        mSettingsBuffer.Update(settingsData);

        const auto& matWorld = renderObject.transform.GetMatrix();
        const auto& matView = mCamera->GetViewMatrix();
        const auto& matProj = mCamera->GetProjectionMatrix();

        TransformData data;
        data.wvp = Transpose(matWorld * matView * matProj);
        data.world = Transpose(matWorld);
        data.viewPos = mCamera->GetPosition();
        if (settingsData.useShadowMap)
        {
            const auto& matLightView = mLightCamera->GetViewMatrix();
            const auto& matLightProj = mLightCamera->GetProjectionMatrix();
            data.lwvp = Transpose(matWorld * matLightView * matLightProj);

            mShadowMap->BindPS(4);
        }

        if (settingsData.useSkinning > 0)
        {
            AnimationUtil::BoneTransforms boneTransforms;
            AnimationUtil::ComputeBoneTransforms(renderObject.modelID, boneTransforms);
            AnimationUtil::ApplyBoneOfset(renderObject.modelID, boneTransforms);
            for (Math::Matrix4& transform : boneTransforms)
            {
                transform = Transpose(transform);
            }
            boneTransforms.resize(MaxBoneCount);
            mBoneTransformBuffer.Update(boneTransforms.data());
        }

        mTransformBuffer.Update(data);
        mLightBuffer.Update(*mDirectionalLight);
        mMaterialBuffer.Update(renderObject.material);

        auto tm = TextureManager::Get();
        tm->BindPS(renderObject.diffuseMapID, 0);
        tm->BindPS(renderObject.normalMapID, 1);
        tm->BindPS(renderObject.specMapID, 2);
        tm->BindVS(renderObject.bumpMapID, 3);
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
            bool useBumpMap = mSettingsData.useBumpMap > 0;
            if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
            {
                mSettingsData.useBumpMap = static_cast<int>(useBumpMap);
            }
            float bumpWeight = mSettingsData.bumpWeight;
            if (ImGui::DragFloat("BumpWeight", &bumpWeight, 0.1f, 0.0f, 10.0f))
            {
                mSettingsData.bumpWeight = bumpWeight;
            }
            ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.00001f, 0.0f, 1.0f, "%.6f");
        }
    }

    void StandardEffect::SetLightCamera(const Camera& camera)
    {
        mLightCamera = &camera;
    }

    void StandardEffect::SetShadowMap(const Texture& shadowMap)
    {
        mShadowMap = &shadowMap;
    }

    void StandardEffect::SetShadowMapFar(const Texture& shadowMapFar)
    {
        mShadowMapFar = &shadowMapFar;
    }
}