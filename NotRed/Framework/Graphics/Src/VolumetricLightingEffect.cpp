#include "Precompiled.h"
#include "VolumetricLightingEffect.h"

#include "RenderTarget.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

namespace NotRed::Graphics
{
    void VolumetricLightingEffect::Initialize()
    {
        mPerFrameBuffer.Initialize();
        mRayMarchingBuffer.Initialize();
        mLightDataBuffer.Initialize();
        mDepthDataBuffer.Initialize();

        std::filesystem::path shaderFile = "../../Assets/Shaders/VolumetricLighting.fx";
        mVertexShader.Initialize<VertexPX>(shaderFile);
        mPixelShader.Initialize(shaderFile);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

        mBlendState.Initialize(BlendState::Mode::AlphaBlend);

        shaderFile = "../../Assets/Shaders/NewDepth.fx";
        mLightVertexShader.Initialize<VertexP>(shaderFile);
        mLightPixelShader.Initialize(shaderFile);

        GraphicsSystem* gs = GraphicsSystem::Get();
        const uint32_t screenWidth = gs->GetBackBufferWidth();
        const uint32_t screenHeight = gs->GetBackBufferHeight();

        mLightGeometryTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U32);
        mLightInGeometryTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U32);
    }

    void VolumetricLightingEffect::Terminate()
    {
        mBlendState.Terminate();

        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();

        mLightVertexShader.Terminate();
        mLightPixelShader.Terminate();

        mDepthDataBuffer.Terminate();
        mLightDataBuffer.Terminate();
        mRayMarchingBuffer.Terminate();
        mPerFrameBuffer.Terminate();

        mLightGeometryTarget.Terminate();
        mLightInGeometryTarget.Terminate();
    }

    void VolumetricLightingEffect::Render(
        const RenderObject& renderObject,
        const RenderObject& inRenderObject)
    {
        RenderDepth(renderObject, mLightGeometryTarget);
        RenderDepth(inRenderObject, mLightInGeometryTarget);
    }

    void VolumetricLightingEffect::RenderScreenQuad(const SpotLight& light, const RenderObject& renderTarget) const
    {
        mVertexShader.Bind();
        mPixelShader.Bind();
        mSampler.BindPS(0);

        mBlendState.Set();

        {
            mGeometryTexture->BindPS(0);
            mDepthTetxure->BindPS(1);
            mNormalsTetxure->BindPS(2);

            mLightGeometryTarget.BindPS(3);
            mLightInGeometryTarget.BindPS(4);
        }

        PerFrameData perFrameData;
        perFrameData.viewMatrix = Math::Transpose(mCamera->GetViewMatrix());

        perFrameData.invViewMatrix = Math::Transpose(mCamera->GetViewMatrix().Inverse());
        perFrameData.invProjectionMatrix = Math::Transpose(mCamera->GetProjectionMatrix().Inverse());
        perFrameData.invModelMatrix = Math::Matrix4::Translation(light.LightPosition);

        perFrameData.cameraPosition = mCamera->GetPosition();
        mPerFrameBuffer.Update(perFrameData);
        mPerFrameBuffer.BindPS(0);

        RayMarchingData marchData;
        marchData.stepSize = mStepSize;
        marchData.densityMultiplier = mDensityMultiplier;
        marchData.lightIntensity = mLightIntensity;
        mRayMarchingBuffer.Update(marchData);
        mRayMarchingBuffer.BindPS(1); 
        
        LightData lightData;
        lightData.LightColor = light.LightColor;
        mLightDataBuffer.Update(lightData);
        mLightDataBuffer.BindPS(2);

        renderTarget.meshBuffer.Render();

        for (int i = 0; i < 6; ++i)
        {
            Texture::UnbindPS(i);
        }
    }

    void VolumetricLightingEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void VolumetricLightingEffect::RenderDepth(const RenderObject& renderObject, RenderTarget& target)
    {
        target.BeginRender();

        mLightVertexShader.Bind();
        mLightPixelShader.Bind();
        mDepthDataBuffer.BindVS(0);
        mDepthDataBuffer.BindPS(0);

        DepthData transformData;
        transformData.modelTransform = renderObject.transform.GetMatrix();
        transformData.viewMatrix = mCamera->GetViewMatrix();
        transformData.viewProjectionMatrix = mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix();
        
        mDepthDataBuffer.Update(transformData);

        renderObject.meshBuffer.Render();

        target.EndRender();
    }

    void VolumetricLightingEffect::DebugUI()
    {
        if (ImGui::CollapsingHeader("Textures:"))
        {
            ImGui::Image(
                mGeometryTexture->GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
            ImGui::Image(
                mDepthTetxure->GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
            ImGui::Image(
                mNormalsTetxure->GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
            ImGui::Image(
                mLightGeometryTarget.GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
            ImGui::Image(
                mLightInGeometryTarget.GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
        }
        if (ImGui::CollapsingHeader("Shader Data:"))
        {
            ImGui::DragFloat("Step Size", &mStepSize, 0.002f, 0.001f, 10.0f);
            ImGui::DragFloat("Density Multiplier", &mDensityMultiplier, 0.01f);
            ImGui::DragFloat("Light Intensity", &mLightIntensity, 0.2f);
        }
    }
}