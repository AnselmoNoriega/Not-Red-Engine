#include "VolumetricLightingEffect.h"

namespace NotRed::Graphics
{
    void VolumetricLightingEffect::Initialize()
    {
        mTransformBuffer.Initialize();
        mLightTransformBuffer.Initialize();

        std::filesystem::path shaderFile = "../../Assets/Shaders/VolumetricLighting.fx";
        mVertexShader.Initialize<Vertex>(shaderFile);
        mPixelShader.Initialize(shaderFile);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

        mBlendState.Initialize(BlendState::Mode::AlphaBlend);

        shaderFile = "../../Assets/Shaders/Depth.fx";
        mLightVertexShader.Initialize<VertexPC>(shaderFile);

        GraphicsSystem* gs = GraphicsSystem::Get();
        const uint32_t screenWidth = gs->GetBackBufferWidth();
        const uint32_t screenHeight = gs->GetBackBufferHeight();

        mLightGeometryTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::R32_FLOAT);
        mLightInGeometryTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::R32_FLOAT);
        mLightViewTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::R32_FLOAT);

        //mLightGeometryTexture = &mLightGeometryTarget;
    }

    void VolumetricLightingEffect::Terminate()
    {
        mBlendState.Terminate();

        mSampler.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();

        mTransformBuffer.Terminate();
        mLightTransformBuffer.Terminate();

        mLightGeometryTarget.Terminate();
        mLightInGeometryTarget.Terminate();
        mLightViewTarget.Terminate();
    }

    void VolumetricLightingEffect::Render(const RenderObject& renderObject, const RenderObject& inRenderObject, const RenderObject& renderTarget)
    {
        RenderDepth(renderObject, mLightGeometryTarget);
        RenderDepth(renderObject, mLightInGeometryTarget);
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
        }

        Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = matWorld * matView * matProj;

        SimpleVolumeTransformData transformData;
        transformData.wvp = Math::Transpose(matFinal);
        transformData.world = Math::Transpose(renderObject.transform.GetMatrix());
        transformData.viewDir = mCamera->GetDirection();

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

        Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = matWorld * matView * matProj;

        SimpleLightTransformData transformData;
        transformData.wvp = Math::Transpose(matFinal);

        //const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
        //const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
        //transformData.lwvp = Transpose(matWorld * matLightView * matLightProj);

        mLightTransformBuffer.Update(transformData);

        renderObject.meshBuffer.Render();

        target.EndRender();
    }

    void VolumetricLightingEffect::RenderLightCam()
    {
        mLightViewTarget.BeginRender();

        mLightVertexShader.Bind();

        SpotLight light;
        const Math::Matrix4 matView = light.cameraObj.GetViewMatrix();
        const Math::Matrix4 matProj = light.cameraObj.GetProjectionMatrix();

        for (const auto& obj : mCharacters)
        {
            for (const RenderObject& renderObject : *obj)
            {
                const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
                Math::Matrix4 matFinal = matWorld * matView * matProj;

                SimpleLightTransformData transformData;
                transformData.wvp = Math::Transpose(matFinal);
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