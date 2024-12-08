#pragma once

#include <Not-Red/Inc/NotRed.h>

namespace NotRed::Graphics
{
    class Camera;
    class Texture;
    struct RenderObject;

    struct SpotLight
    {
        SpotLight()
        {
            cameraObj.SetPosition({ 0.0f, 10.0f, 0.0f });
            cameraObj.SetDirection({ 0.0f, -0.99f, 0.1f });
        }

        Camera cameraObj;
    };

    class VolumetricLightingEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Render(const RenderObject& renderObject, const RenderObject& inRenderObject, const RenderObject& renderTarget);

        void DebugUI();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);
        inline void SetTextures(const Texture* renderTarget, const Texture* depthTarget)
        {
            mGeometryTexture = renderTarget;
            mGeometryPositionTetxure = depthTarget;
        }

        inline void RegisterObject(const RenderGroup& obj)
        {
            mCharacters.push_back(&obj);
        }

    private:
        void RenderDepth(const RenderObject& renderObject, RenderTarget& target);
        void RenderDepthFromLight();

    private:

        struct ViewData
        {
            Math::Vector3 viewDir;
            Math::Matrix4 viewMatrix;
            Math::Vector3 camPos;
            float padding[2];
        };
        struct LightData
        {
            Math::Matrix4 lightViewProj;
            Math::Vector3 lightPos;
            float padding;
        };
        struct MatrixData
        {
            Math::Matrix4 lightMatrix;
            Math::Matrix4 geoMatrix;
        };

        struct DepthData
        {
            Math::Matrix4 modelTransform;
            Math::Matrix4 viewMatrix;
            Math::Matrix4 viewProjectionMatrix;
        };

        using ViewDataBuffer = TypedConstantBuffer<ViewData>;
        using LightDataBuffer = TypedConstantBuffer<LightData>;
        using MatrixDataBuffer = TypedConstantBuffer<MatrixData>;

        using DepthDataBuffer = TypedConstantBuffer<DepthData>;

    private:

        ViewDataBuffer mViewDataBuffer;
        LightDataBuffer mLightDataBuffer;
        MatrixDataBuffer mMatrixDataBuffer;

        DepthDataBuffer mDepthDataBuffer;

        Sampler mSampler;
        BlendState mBlendState;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        VertexShader mLightVertexShader;
        PixelShader mLightPixelShader;

        const Camera* mCamera = nullptr;

        const Texture* mGeometryTexture = nullptr;
        const Texture* mGeometryPositionTetxure = nullptr;

        RenderTarget mLightGeometryTarget;
        RenderTarget mLightInGeometryTarget;
        RenderTarget mLightViewTarget;

        std::vector<const RenderGroup*> mCharacters;
    };
}