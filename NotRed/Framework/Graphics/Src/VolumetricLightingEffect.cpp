#include "Precompiled.h"
#include "VolumetricLightingEffect.h"

#include "RenderTarget.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

namespace NotRed::Graphics
{
    void VolumetricLightingEffect::Initialize()
    {
        mViewDataBuffer.Initialize();
        mLightDataBuffer.Initialize();
        mMatrixDataBuffer.Initialize();

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
        mLightViewTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U32);
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

        mViewDataBuffer.Terminate();
        mMatrixDataBuffer.Terminate();
        mLightDataBuffer.Terminate();

        mLightGeometryTarget.Terminate();
        mLightInGeometryTarget.Terminate();
        mLightViewTarget.Terminate();
    }

    void VolumetricLightingEffect::Render(const SpotLight& light,
        const RenderObject& renderObject,
        const RenderObject& inRenderObject)
    {
        RenderDepth(renderObject, mLightGeometryTarget);
        RenderDepth(inRenderObject, mLightInGeometryTarget);
        RenderDepthFromLight(light);
    }

    void VolumetricLightingEffect::RenderScreenQuad(const SpotLight& light, const RenderObject& renderTarget) const
    {
        mVertexShader.Bind();
        mPixelShader.Bind();
        mSampler.BindPS(0);

        mBlendState.Set();

        {
            mGeometryTexture->BindPS(0);
            mGeometryPositionTetxure->BindPS(1);

            mLightGeometryTarget.BindPS(2);
            mLightInGeometryTarget.BindPS(3);
            mLightViewTarget.BindPS(4);
        }

        ViewData viewData;
        viewData.viewProjection = Math::Transpose(mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix());
        viewData.viewMatrix = mCamera->GetViewMatrix();
        viewData.camPos = mCamera->GetPosition();

        mViewDataBuffer.BindVS(0);
        mViewDataBuffer.BindPS(0);
        mViewDataBuffer.Update(viewData);

        LightData lightData;
        lightData.lightViewProj = Math::Transpose(light.CameraObj.GetViewMatrix() * light.CameraObj.GetProjectionMatrix());
        lightData.lightPos = light.CameraObj.GetPosition();
        lightData.lightColor = light.LightColor;
        lightData.wvp = lightData.lightViewProj;

        mLightDataBuffer.Update(lightData);
        mLightDataBuffer.BindVS(1);
        mLightDataBuffer.BindPS(1);

        MatrixData matrixData;
        matrixData.geoMatrix = Math::Matrix4::Identity;
        matrixData.lightMatrix = Math::Matrix4::Translation(light.LightPosition);

        mMatrixDataBuffer.BindVS(2);
        mMatrixDataBuffer.BindPS(2);
        mMatrixDataBuffer.Update(matrixData);

        renderTarget.meshBuffer.Render();

        for (int i = 0; i < 3; ++i)
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

    void VolumetricLightingEffect::RenderDepthFromLight(const SpotLight& light)
    {
        mLightViewTarget.BeginRender();

        mLightVertexShader.Bind();
        mLightPixelShader.Bind();
        mDepthDataBuffer.BindVS(0);
        mDepthDataBuffer.BindPS(0);
        
        DepthData transformData;
        transformData.viewMatrix = light.CameraObj.GetViewMatrix();
        transformData.viewProjectionMatrix = light.CameraObj.GetViewMatrix() * light.CameraObj.GetProjectionMatrix();

        for (const auto& obj : mCharacters)
        {
            for (const RenderObject& renderObject : *obj)
            {
                transformData.modelTransform = renderObject.transform.GetMatrix();
                mDepthDataBuffer.Update(transformData);

                renderObject.meshBuffer.Render();
            }
        }

        mLightViewTarget.EndRender();
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
                mGeometryPositionTetxure->GetRawData(),
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
            ImGui::Image(
                mLightViewTarget.GetRawData(),
                { 144, 144 },
                { 0, 0 },
                { 1, 1 },
                { 1, 1, 1, 1 },
                { 1, 1, 1, 1 });
        }
    }
}