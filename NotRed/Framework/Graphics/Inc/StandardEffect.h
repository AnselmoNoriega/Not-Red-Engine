#pragma once

#include "ConstantBuffer.h"
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

        void DebugUI();

    private:

        ConstantBuffer mConstantBuffer;
        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        const Camera* mCamera;
    };
}