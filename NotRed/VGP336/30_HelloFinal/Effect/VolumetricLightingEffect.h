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

        void Render(const RenderObject& renderObject, const RenderObject& inRenderObject, const RenderObject& renderTarget);

        void DebugUI();

        void SetCamera(const Camera& camera);
        inline void SetTextures(const Texture* renderTarget, const Texture* depthTarget)
        {
            mGeometryTexture = renderTarget;
            mGeometryPositionTetxure = depthTarget;
        }

    private:
        void RenderDepth(const RenderObject& renderObject, RenderTarget& target);

    private:
        struct SimpleVolumeTransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewDir;
            float padding = 0.0f;
        };

        struct SimpleLightTransformData
        {
            Math::Matrix4 wvp;
        };

        using SimpleVolumeTransformBuffer = TypedConstantBuffer<SimpleVolumeTransformData>;
        using SimpleLightTransformBuffer = TypedConstantBuffer<SimpleLightTransformData>;

    private:

        SimpleVolumeTransformBuffer mTransformBuffer;
        SimpleLightTransformBuffer mLightTransformBuffer;

        Sampler mSampler;
        BlendState mBlendState;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        VertexShader mLightVertexShader;

        const Camera* mCamera = nullptr;

        const Texture* mGeometryTexture = nullptr;
        const Texture* mGeometryPositionTetxure = nullptr;

        RenderTarget mLightGeometryTarget;
        RenderTarget mLightInGeometryTarget;
    };
}