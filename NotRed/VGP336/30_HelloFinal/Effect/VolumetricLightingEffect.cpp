#include "VolumetricLightingEffect.h"

namespace NotRed::Graphics
{
    void VolumetricLightingEffect::Initialize()
    {
        mTransformBuffer.Initialize();
        mLightTransformBuffer.Initialize();

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

        mTransformBuffer.Terminate();
        mLightTransformBuffer.Terminate();

        mLightGeometryTarget.Terminate();
        mLightInGeometryTarget.Terminate();
        mLightViewTarget.Terminate();
    }

    void VolumetricLightingEffect::Render(const RenderObject& renderObject, const RenderObject& inRenderObject, const RenderObject& renderTarget)
    {
        RenderDepth(renderObject, mLightGeometryTarget);
        RenderDepth(inRenderObject, mLightInGeometryTarget);
        RenderLightCam();

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
        \
        SimpleVolumeTransformData transformData;\
        transformData.world = Math::Transpose(renderObject.transform.GetMatrix());
        transformData.viewDir = mCamera->GetDirection();
        transformData.viewMatrix = mCamera->GetViewMatrix();
        SpotLight light;
        transformData.LightViewProj = Math::Transpose(light.cameraObj.GetViewMatrix() * light.cameraObj.GetProjectionMatrix());

        mTransformBuffer.BindVS(0);
        mTransformBuffer.BindPS(0);
        mTransformBuffer.Update(transformData);

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
        mLightTransformBuffer.BindVS(0);
        mLightTransformBuffer.BindPS(0);

        SimpleLightTransformData transformData;
        transformData.modelTransform = renderObject.transform.GetMatrix();
        transformData.viewMatrix = mCamera->GetViewMatrix();
        transformData.viewProjectionMatrix = mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix();
        
        mLightTransformBuffer.Update(transformData);

        renderObject.meshBuffer.Render();

        target.EndRender();
    }

    void VolumetricLightingEffect::RenderLightCam()
    {
        mLightViewTarget.BeginRender();

        mLightVertexShader.Bind();
        mLightPixelShader.Bind();
        mLightTransformBuffer.BindVS(0);
        mLightTransformBuffer.BindPS(0);
        
        auto light = SpotLight();
        SimpleLightTransformData transformData;
        transformData.viewMatrix = light.cameraObj.GetViewMatrix();
        transformData.viewProjectionMatrix = light.cameraObj.GetViewMatrix() * light.cameraObj.GetProjectionMatrix();

        for (const auto& obj : mCharacters)
        {
            for (const RenderObject& renderObject : *obj)
            {
                transformData.modelTransform = renderObject.transform.GetMatrix();
                mLightTransformBuffer.Update(transformData);

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

    void VolumetricLightingEffect::Render(const RenderObject& renderObject)
    {
    }
}