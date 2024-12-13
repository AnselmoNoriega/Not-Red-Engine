#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "BlendState.h"
#include "RenderTarget.h"
#include "Texture.h"

#include "Camera.h"
#include "RenderObject.h"

namespace NotRed::Graphics
{
    class Texture;
    struct RenderObject;

    struct SpotLight
    {
        SpotLight()
        {
            CameraObj.SetPosition({ 0.0f, 10.0f, 0.0f });
            CameraObj.SetDirection({ 0.0f, -0.99f, 0.1f });
        }

        Camera CameraObj;
        Math::Vector3 LightPosition{ 0.0f, 10.0f, 0.0f };
        Math::Vector3 LightColor{ 1.0f, 1.0f, 0.8f };
    };

    class VolumetricLightingEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Render(const SpotLight& light,
            const RenderObject& renderObject, 
            const RenderObject& inRenderObject);

        void RenderScreenQuad(const SpotLight& light, const RenderObject& renderTarget)const;

        void DebugUI();

        void SetCamera(const Camera& camera);
        inline void SetDepthTexture(const Texture* depthTarget)
        {
            mGeometryPositionTetxure = depthTarget;
        }

        inline void UpdateRenderImage(const Texture* renderTarget)
        {
            mGeometryTexture = renderTarget;
        }

        inline void RegisterObject(const RenderGroup& obj)
        {
            mCharacters.push_back(&obj);
        }

    private:
        void RenderDepth(const RenderObject& renderObject, RenderTarget& target);
        void RenderDepthFromLight(const SpotLight& light);

    private:

        struct ViewData
        {
            Math::Matrix4 viewMatrix;
            Math::Matrix4 viewProjection;
            Math::Vector3 camPos;
            float padding;
        };
        struct LightData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 lightViewProj;
            Math::Matrix4 lightView;
            Math::Vector3 lightPos;
            float padding = 0.0f;
            Math::Vector3 lightColor;
            float padding2 = 0.0f;
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