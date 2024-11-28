#pragma once

#include <Not-Red/Inc/NotRed.h>

namespace NotRed::Graphics
{
    class Camera;
    class Texture;
    struct RenderObject;

    class VolumetricLightingEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Render(const std::vector<RenderObject>& renderObjects, const Camera& lightCamera);

        void DebugUI();

        void SetCamera(const Camera& camera);
        void AddObjectForShadows(const RenderObject& obj);

    private:
        void RenderDepth();

    private:
        struct SimpleVolumeTransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewDir;
            // float padding = 0.0f;
        };

        using SimpleVolumeTransformBuffer = TypedConstantBuffer<SimpleVolumeTransformData>;

    private:

        SimpleVolumeTransformBuffer mTransformBuffer;

        Sampler mSampler;
        BlendState mBlendState;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        std::vector<const RenderObject*> mObjects;

        const Camera* mCamera = nullptr;
    };
}