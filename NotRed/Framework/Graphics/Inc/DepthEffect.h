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

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);

        void DebugUI();

    private:
        struct Transform
        {
            Math::Matrix4 wvp;
        };

        using TransformBuffer = TypedConstantBuffer<Transform>;

        TransformBuffer mTransformBuffer;

        Sampler mSampler;
        VertexShader mVertexShader;

        const Camera* mCamera = nullptr;
    };
}