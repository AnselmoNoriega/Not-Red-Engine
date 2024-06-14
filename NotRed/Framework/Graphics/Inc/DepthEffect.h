#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Material.h"
#include "VertexShader.h"

namespace NotRed::Graphics
{
    class Camera;
    class Texture;
    struct RenderObject;

    class DepthEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject, const Math::Matrix4& pos);

        void SetCamera(const Camera& camera);

        void DebugUI();

    private:
        struct Transform
        {
            Math::Matrix4 wvp;
            Math::Vector3 position;
            float padding;
        };

        using TransformBuffer = TypedConstantBuffer<Transform>;

        TransformBuffer mTransformBuffer;

        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        const Camera* mCamera = nullptr;
    };
}