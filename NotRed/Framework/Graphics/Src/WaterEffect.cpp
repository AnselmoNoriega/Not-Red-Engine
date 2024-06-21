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
		mGeometryShader.Initialize(shaderFile);
		mVertexShader[NORMAL].Initialize<Vertex>(shaderFile);
		mPixelShader[NORMAL].Initialize(shaderFile);

		shaderFile = "../../Assets/Shaders/WaterDepth.fx";
		mVertexShader[DEPTH].Initialize<VertexPX>(shaderFile);
		mPixelShader[DEPTH].Initialize(shaderFile);

		shaderFile = "../../Assets/Shaders/WaterHeight.fx";
		mVertexShader[HEIGHT].Initialize<Vertex>(shaderFile);
		mPixelShader[HEIGHT].Initialize(shaderFile);

		shaderFile = "../../Assets/Shaders/Refraction.fx";
		mVertexShader[EFFECT].Initialize<VertexPX>(shaderFile);
		mPixelShader[EFFECT].Initialize(shaderFile);

		mWaveBuffer.Initialize();
		mSimpleTransformBuffer.Initialize();
		mRefractionHelperBuffer.Initialize();

		mBlendState.Initialize(BlendState::Mode::AlphaBlend);

		mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

		mFoam.Initialize("../../Assets/Images/water/shampoo.jpg");
		mTextures[0] = &mWaterTarget[NORMAL];
		mTextures[1] = &mWaterTarget[DEPTH];
		mTextures[2] = &mWaterTarget[HEIGHT];
		mTextures[5] = &mFoam;

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

		GraphicsSystem* gs = GraphicsSystem::Get();
		const uint32_t screenWidth = gs->GetBackBufferWidth();
		const uint32_t screenHeight = gs->GetBackBufferHeight();
		for (int i = 0; i < mWaterTarget.size(); ++i)
		{
			mWaterTarget[i].Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
		}
	}

	void WaterEffect::Terminate()
	{
		mFoam.Terminate(); 
		for (int i = 0; i < mWaterTarget.size(); ++i)
		{
			mWaterTarget[i].Terminate();
		}
		mSampler.Terminate();
		mBlendState.Terminate();
		mSimpleTransformBuffer.Terminate();
		mWaveBuffer.Terminate();
		mRefractionHelperBuffer.Terminate();

		mGeometryShader.Terminate();
		for (int i = 0; i < mVertexShader.size(); ++i)
		{
			mPixelShader[i].Terminate();
			mVertexShader[i].Terminate();
		}
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
		mWaterTarget[DEPTH].BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));

		mVertexShader[DEPTH].Bind();
		mPixelShader[DEPTH].Bind();
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

		mWaterTarget[DEPTH].EndRender();
	}

	void WaterEffect::RenderHeight(const RenderObject& renderObject, const Math::Matrix4& position)
	{
		mWaterTarget[HEIGHT].BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));

		mVertexShader[HEIGHT].Bind();
		mPixelShader[HEIGHT].Bind();
		mSimpleTransformBuffer.BindVS(0);
		mSimpleTransformBuffer.BindPS(0);
		mWaveBuffer.BindVS(1);
		mSampler.BindPS(0);
		TextureManager::Get()->BindPS(mAnimatedTexture[mTextureIndex], 0);

		const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
		const Math::Matrix4 matView = mCamera->GetViewMatrix();
		const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

		Math::Matrix4 matFinal = position * matWorld * matView * matProj;

		SimpleTransform transformData;
		transformData.wvp = Math::Transpose(matFinal);
		mSimpleTransformBuffer.Update(transformData);

		mWaveBuffer.Update(mWaterData);

		renderObject.meshBuffer.Render();

		mWaterTarget[HEIGHT].EndRender();
	}

	void WaterEffect::RenderNormal(const RenderObject& renderObject, const Math::Matrix4& position)
	{
		mWaterTarget[NORMAL].BeginRender();

		mVertexShader[NORMAL].Bind();
		mGeometryShader.Bind();
		mPixelShader[NORMAL].Bind();

		mBlendState.Set();

		mSimpleTransformBuffer.BindVS(0);
		mWaveBuffer.BindVS(1);

		mWaveBuffer.Update(mWaterData);

		const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
		const Math::Matrix4 matView = mCamera->GetViewMatrix();
		const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

		Math::Matrix4 matFinal = position * matWorld * matView * matProj;

		SimpleTransform transformData;
		transformData.wvp = Math::Transpose(matFinal);
		mSimpleTransformBuffer.Update(transformData);

		renderObject.meshBuffer.Render();

		mGeometryShader.Unbind();

		if (mShadowMap != nullptr)
		{
			Texture::UnbindPS(4);
		}

		mWaterTarget[NORMAL].EndRender();
	}

	void WaterEffect::RenderEffect(const RenderObject& renderObject)
	{
		mVertexShader[EFFECT].Bind();
		mPixelShader[EFFECT].Bind();
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

		TextureManager::Get()->BindPS(mAnimatedTexture[mTextureIndex], 6);
		mRefractionHelperBuffer.Update(mRefractionHelper);

		renderObject.meshBuffer.Render();

		for (uint32_t i = 0; i < mTextures.size(); ++i)
		{
			Texture::UnbindPS(i);
		}
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
		mTextures[3] = renderTarget;
		mTextures[4] = depthbuffer;
	}

	void WaterEffect::RenderWater(const RenderObject& renderObject, const Math::Matrix4& position, const RenderObject& renderTarget)
	{
		RenderDepth(renderObject, position);
		RenderNormal(renderObject, position);
		RenderHeight(renderObject, position);
		RenderEffect(renderTarget);
	}
}