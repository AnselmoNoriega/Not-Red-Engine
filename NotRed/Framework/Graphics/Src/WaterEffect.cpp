#include "Precompiled.h"
#include "WaterEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

namespace NotRed::Graphics
{
    void WaterEffect::Initialize()
    {
        std::filesystem::path shaderFile = "../../Assets/Shaders/Water.fx";
        mVertexShader.Initialize<Vertex>(shaderFile);
        mGeometryShader.Initialize(shaderFile);
        mPixelShader.Initialize(shaderFile);

        mTransformBuffer.Initialize();
        mLightBuffer.Initialize();
        mMaterialBuffer.Initialize();
        mWaveBuffer.Initialize();
        mDepthTransformBuffer.Initialize();
        mBlendState.Initialize(BlendState::Mode::AlphaBlend);
        mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

        shaderFile = "../../Assets/Shaders/WaterDepth.fx";
        mDepthVertexShader.Initialize<VertexPX>(shaderFile);
        mDepthPixelShader.Initialize(shaderFile);

        shaderFile = "../../Assets/Shaders/Refraction.fx";
        mEffectVertexShader.Initialize<VertexPX>(shaderFile);
        mEffectPixelShader.Initialize(shaderFile);

        mFoam.Initialize("../../Assets/Images/water/shampoo.jpg");

        mTextures[0] = &mWaterTarget;
        mTextures[3] = &mWaterDepth;
        mTextures[4] = &mFoam;
        GraphicsSystem* gs = GraphicsSystem::Get();
        const uint32_t screenWidth = gs->GetBackBufferWidth();
        const uint32_t screenHeight = gs->GetBackBufferHeight();
        mWaterTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8); 
        mWaterDepth.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
    }

    void WaterEffect::Terminate()
    {
        mFoam.Terminate();
        mWaterDepth.Terminate();
        mWaterTarget.Terminate();
        mSampler.Terminate();
        mBlendState.Terminate();
        mDepthTransformBuffer.Terminate();
        mWaveBuffer.Terminate();
        mMaterialBuffer.Terminate();
        mLightBuffer.Terminate();
        mTransformBuffer.Terminate();
        mPixelShader.Terminate();
        mGeometryShader.Terminate();
        mVertexShader.Terminate();

        mEffectPixelShader.Terminate();
        mEffectVertexShader.Terminate();
        mDepthPixelShader.Terminate();
        mDepthVertexShader.Terminate();
        mPixelShader.Terminate();
        mGeometryShader.Terminate();
        mVertexShader.Terminate();
    }

    void WaterEffect::Update(float dt)
    {
        mWaterData.waveMovementTime += dt * mTimeMultiplier;
    }

    void WaterEffect::RenderDepth(const RenderObject& renderObject, const Math::Matrix4& position)
    {
        mWaterDepth.BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));

        mDepthVertexShader.Bind();
        mDepthPixelShader.Bind();
        mDepthTransformBuffer.BindVS(0);
        mDepthTransformBuffer.BindPS(0);
        mWaveBuffer.BindVS(1);
        mSampler.BindVS(0);
        mSampler.BindPS(0);

        const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = position * matWorld * matView * matProj;

        DepthTransform transformData;
        transformData.wvp = Math::Transpose(matFinal);
        mDepthTransformBuffer.Update(transformData);

        mWaveBuffer.Update(mWaterData);

        renderObject.meshBuffer.Render();

        mWaterDepth.EndRender();
    }

    void WaterEffect::Begin()
    {
        mWaterTarget.BeginRender();

        mVertexShader.Bind();
        mGeometryShader.Bind();
        mPixelShader.Bind();

        mBlendState.Set();

        mTransformBuffer.BindVS(0);

        mLightBuffer.BindVS(2);
        mLightBuffer.BindPS(2);

        mMaterialBuffer.BindPS(3);

        mWaveBuffer.BindVS(4);

        mSampler.BindVS(0);
        mSampler.BindPS(0);
    }

    void WaterEffect::Render(const RenderObject& renderObject, const Math::Matrix4& position)
    {
        mWaveBuffer.Update(mWaterData);

        const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = position * matWorld * matView * matProj;

        TransformData transformData;
        transformData.wvp = Math::Transpose(matFinal);
        transformData.world = Math::Transpose(matWorld);
        transformData.viewPosition = mCamera->GetPosition();
        mTransformBuffer.Update(transformData);

        mMaterialBuffer.Update(renderObject.material);

        TextureManager* tm = TextureManager::Get();
        tm->BindPS(renderObject.diffuseMapID, 0);
        tm->BindPS(renderObject.normalMapID, 1);
        tm->BindPS(renderObject.specMapID, 2);
        tm->BindPS(renderObject.bumpMapID, 3);

        renderObject.meshBuffer.Render();
    }

    void WaterEffect::RenderEffect(const RenderObject& renderObject)
    {
        mEffectVertexShader.Bind();
        mEffectPixelShader.Bind();
        mSampler.BindPS(0);

        mBlendState.Set();

        for (uint32_t i = 0; i < mTextures.size(); ++i)
        {
            if (mTextures[i] != nullptr)
            {
                mTextures[i]->BindPS(i);
            }
        }

        mLightBuffer.BindPS(0);
        mLightBuffer.Update(mLightData);

        renderObject.meshBuffer.Render();

        for (uint32_t i = 0; i < mTextures.size(); ++i)
        {
            Texture::UnbindPS(i);
        }
    }

    void WaterEffect::End()
    {
        mGeometryShader.Unbind();

        if (mShadowMap != nullptr)
        {
            Texture::UnbindPS(4);
        }

        mWaterTarget.EndRender();
    }

    void WaterEffect::DebugUI()
    {
        if (ImGui::CollapsingHeader("WaveEffect", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::CollapsingHeader("WavePatterns", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat4("Wave1##Wave", &mWaterData.waves[0].x, 0.05f, 0.0f, 10.0f);
                ImGui::DragFloat4("Wave2##Wave", &mWaterData.waves[1].x, 0.05f, 0.0f, 10.0f);
                ImGui::DragFloat4("Wave3##Wave", &mWaterData.waves[2].x, 0.05f, 0.0f, 10.0f);
                ImGui::DragFloat4("Wave4##Wave", &mWaterData.waves[3].x, 0.05f, 0.0f, 10.0f);
            }

            ImGui::DragFloat("mTimeMultiplier##Wave", &mTimeMultiplier, 0.05f, 0.0f, 10.0f);
            ImGui::DragFloat("waveStrength##Wave", &mWaterData.waveStrength, 0.05f, 0.0f, 10.0f);
        }
    }

    void WaterEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void WaterEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
    {
        mLightData.lightDirection = directionalLight.direction;
    }

    void WaterEffect::SetShadowMap(const Texture& shadowMap)
    {
        mShadowMap = &shadowMap;
    }

    void WaterEffect::SetTextures(const Texture* renderTarget, const Texture* depthbuffer)
    {
        mTextures[1] = renderTarget;
        mTextures[2] = depthbuffer;
    }
}