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

        mRefractionHelperBuffer.Initialize();
        mWaveBuffer.Initialize();
        mSimpleTransformBuffer.Initialize();
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

        TextureManager* tm = TextureManager::Get();
        std::filesystem::path path;
        for (int i = 1; i <= 120; ++i)
        {
            path = "water/Animated_Water_Normal_Map_2/0";
            for (int j = 100; j > i; j *= 0.1f)
            {
                path += "0";
            }
            path += std::to_string(i) + ".png";
            mAnimatedTexture.push_back(tm->LoadTexture(path));
        }
    }

    void WaterEffect::Terminate()
    {
        mFoam.Terminate();
        mWaterDepth.Terminate();
        mWaterTarget.Terminate();
        mSampler.Terminate();
        mBlendState.Terminate();
        mSimpleTransformBuffer.Terminate();
        mWaveBuffer.Terminate();
        mRefractionHelperBuffer.Terminate();
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
        mRefractionHelper.time += dt;

        mTime += dt;
        if (mTime >= mAnimationChangeTime)
        {
            mTime = 0.0f;
            ++mTextureIndex;

            if (mTextureIndex >= 120)
            {
                mTextureIndex = 0;
            }
        }
    }

    void WaterEffect::RenderDepth(const RenderObject& renderObject, const Math::Matrix4& position)
    {
        mWaterDepth.BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));

        mDepthVertexShader.Bind();
        mDepthPixelShader.Bind();
        mSimpleTransformBuffer.BindVS(0);
        mSimpleTransformBuffer.BindPS(0);
        mWaveBuffer.BindVS(1);
        mSampler.BindVS(0);
        mSampler.BindPS(0);

        const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = position * matWorld * matView * matProj;

        SimpleTransform transformData;
        transformData.wvp = Math::Transpose(matFinal);
        mSimpleTransformBuffer.Update(transformData);

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

        mSimpleTransformBuffer.BindVS(0);
        mWaveBuffer.BindVS(1);
    }

    void WaterEffect::Render(const RenderObject& renderObject, const Math::Matrix4& position)
    {
        mWaveBuffer.Update(mWaterData);

        const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        const Math::Matrix4 matView = mCamera->GetViewMatrix();
        const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

        Math::Matrix4 matFinal = position * matWorld * matView * matProj;

        SimpleTransform transformData;
        transformData.wvp = Math::Transpose(matFinal);
        mSimpleTransformBuffer.Update(transformData);

        renderObject.meshBuffer.Render();
    }

    void WaterEffect::RenderEffect(const RenderObject& renderObject)
    {
        mEffectVertexShader.Bind();
        mEffectPixelShader.Bind();
        mRefractionHelperBuffer.BindPS(0);
        mSampler.BindPS(0);

        mBlendState.Set();

        for (uint32_t i = 0; i < mTextures.size(); ++i)
        {
            if (mTextures[i] != nullptr)
            {
                mTextures[i]->BindPS(i);
            }
        }

        TextureManager::Get()->BindPS(mAnimatedTexture[mTextureIndex], 5);
        mRefractionHelperBuffer.Update(mRefractionHelper);

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
            ImGui::DragFloat("time##Wave", &mRefractionHelper.time, 0.05f, 0.0f, 10.0f);
        }
    }

    void WaterEffect::SetCamera(const Camera& camera)
    {
        mCamera = &camera;
    }

    void WaterEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
    {
        mRefractionHelper.lightDirection = directionalLight.direction;
        mRefractionHelper.lightColor = directionalLight.ambient;
        mRefractionHelper.lightColor.a = 1;
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