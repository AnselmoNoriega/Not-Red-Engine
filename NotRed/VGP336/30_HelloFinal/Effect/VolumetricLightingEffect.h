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
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void DebugUI();

    private:
        struct SimpleVolumeTransformData
        {
            Math::Matrix4 wvp;
            // float padding = 0.0f;
        };

        using SimpleVolumeTransformBuffer = TypedConstantBuffer<SimpleVolumeTransformData>;

        SimpleVolumeTransformBuffer mTransformBuffer;

        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;
    };
}