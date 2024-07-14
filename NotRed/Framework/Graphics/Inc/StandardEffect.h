#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Material.h"
#include "VertexShader.h"

namespace NotRed::Graphics
{
    class Camera;
    class Texture;
    struct RenderObject;

    class StandardEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);
        void SetLightCamera(const Camera& camera);

        void SetDirectionalLight(const DirectionalLight& directionalLight);
        void SetShadowMap(const Texture& shadowMap);
        void SetShadowMapFar(const Texture& shadowMapFar);

        void DebugUI();

    private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 lwvp;
            Math::Matrix4 lfwvp;
            Math::Matrix4 world;
            Math::Vector3 viewPos;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useNormalMap = 1;
            int useSpecMap = 1;
            int useLighting = 1;
            int useBumpMap = 1;
            int useShadowMap = 1;
            float bumpWeight = 1.0f;
            float depthBias = 0.0f;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;

        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;
        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        SettingsData mSettingsData;

        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;

        const DirectionalLight* mDirectionalLight = nullptr;

        const Texture* mShadowMap = nullptr;
        const Texture* mShadowMapFar = nullptr;
    };
}