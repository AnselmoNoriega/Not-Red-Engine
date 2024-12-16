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
        float TimeRunning = 0.0f;
    };

    class VolumetricLightingEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Render(
            const RenderObject& renderObject, 
            const RenderObject& inRenderObject);

        void RenderScreenQuad(const SpotLight& light, const RenderObject& renderTarget)const;

        void DebugUI();

        void SetCamera(const Camera& camera);
        inline void SetTextures(const Texture* depthTarget, const Texture* normalsTarget)
        {
            mDepthTetxure = depthTarget;
            mNormalsTetxure = normalsTarget;
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

    private:

        struct PerFrameData
        {
            Math::Matrix4 viewMatrix;

            Math::Matrix4 invViewMatrix;
            Math::Matrix4 invProjectionMatrix;
            Math::Matrix4 invModelMatrix;

            Math::Vector3 cameraPosition;
            float padding;
        };

        struct LightData
        {
            Math::Vector3 LightColor;
            float padding1;
            Math::Vector3 MoveDirection;
            float padding2;
        };

        struct RayMarchingData
        {
            float stepSize;
            float densityMultiplier;
            float lightIntensity;
            float padding1;
        };

        struct DepthData
        {
            Math::Matrix4 modelTransform;
            Math::Matrix4 viewMatrix;
            Math::Matrix4 viewProjectionMatrix;
        };

        using PerFrameBuffer = TypedConstantBuffer<PerFrameData>;
        using RayMarchingDataBuffer = TypedConstantBuffer<RayMarchingData>;
        using LightDataBuffer = TypedConstantBuffer<LightData>;
        using DepthDataBuffer = TypedConstantBuffer<DepthData>;

    private:

        PerFrameBuffer mPerFrameBuffer;
        RayMarchingDataBuffer mRayMarchingBuffer;
        LightDataBuffer mLightDataBuffer;

        DepthDataBuffer mDepthDataBuffer;

        Sampler mSampler;
        BlendState mBlendState;
        VertexShader mVertexShader;
        PixelShader mPixelShader;

        VertexShader mLightVertexShader;
        PixelShader mLightPixelShader;

        const Camera* mCamera = nullptr;

        const Texture* mGeometryTexture = nullptr;
        const Texture* mDepthTetxure = nullptr;
        const Texture* mNormalsTetxure = nullptr;

        RenderTarget mLightGeometryTarget;
        RenderTarget mLightInGeometryTarget;

        std::vector<const RenderGroup*> mCharacters;

        // RayMarching stuff
        float mStepSize = 0.012f;
        float mDensityMultiplier = 0.2f;
        float mLightIntensity = 15.0f;
        Math::Vector3 mDustMoveDirection = Math::Normalize({1.0f, 0.5f, 0.3f});
    };
}