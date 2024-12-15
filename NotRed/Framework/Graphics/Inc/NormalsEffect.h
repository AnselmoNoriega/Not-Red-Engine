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

    class NormalsEffect
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
            Math::Matrix4 modelTransform;
            Math::Matrix4 viewMatrix;
            Math::Matrix4 viewProjectionMatrix;
        };

        using TransformBuffer = TypedConstantBuffer<Transform>;

        TransformBuffer mTransformBuffer;

        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        const Camera* mCamera = nullptr;
    };
}