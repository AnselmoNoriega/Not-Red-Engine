#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace NotRed::Graphics
{
    class Camera;
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
        void SetDirectionalLight(const DirectionalLight& directionalLight);

        void DebugUI();

    private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewPos;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            float padding[3] = { 0.0f };
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;

        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;
        LightBuffer mLightBuffer;
        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        SettingsData mSettingsData;
        const Camera* mCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
    };
}