#include "Precompiled.h"
#include "WaterEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexTypes.h"

namespace NotRed::Graphics
{
	void WaterEffect::Initialize()
	{
		std::filesystem::path shaderFile = "../../Assets/Shaders/Water.fx";
		mVertexShader.Initialize<Vertex>(shaderFile);
		mPixelShader.Initialize(shaderFile);
		mGeometryShader.Initialize(shaderFile);

		mTransformBuffer.Initialize();
		mSettingsBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();
		mWaveBuffer.Initialize();
		mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	}

	void WaterEffect::Terminate()
	{
		mSampler.Terminate();
		mWaveBuffer.Terminate();
		mMaterialBuffer.Terminate();
		mLightBuffer.Terminate();
		mSettingsBuffer.Terminate();
		mTransformBuffer.Terminate();
		mGeometryShader.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void WaterEffect::Update(float dt)
	{
		mWaterData.waveMovementTime += dt * mTimeMultiplier;
	}

	void WaterEffect::Begin()
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		mTransformBuffer.BindVS(0);

		mSettingsBuffer.BindVS(1);
		mSettingsBuffer.BindPS(1);

		mLightBuffer.BindVS(2);
		mLightBuffer.BindPS(2);

		mMaterialBuffer.BindPS(3);

		mWaveBuffer.BindVS(4);

		mSampler.BindVS(0);
		mSampler.BindPS(0);
	}

	void WaterEffect::End()
	{
		if (mShadowMap != nullptr)
		{
			Texture::UnbindPS(4);
		}
	}

	void WaterEffect::Render(const RenderObject& renderObject, const Math::Matrix4& position)
	{
		mSettingsBuffer.Update(mSettingsData);
		mWaveBuffer.Update(mWaterData);

		const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
		const Math::Matrix4 matView = mCamera->GetViewMatrix();
		const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

		Math::Matrix4 matFinal = position * matWorld * matView * matProj;

		TransformData transformData;
		transformData.wvp = Math::Transpose(matFinal);
		transformData.world = Math::Transpose(matWorld);
		transformData.viewPosition = mCamera->GetPosition();
		if (mSettingsData.useShadowMap > 0)
		{
			const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
			const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
			transformData.lwvp = Transpose(matWorld * matLightView * matLightProj);

			mShadowMap->BindPS(4);
		}
		mTransformBuffer.Update(transformData);

		mLightBuffer.Update(*mDirectionalLight);
		mMaterialBuffer.Update(renderObject.material);

		TextureManager* tm = TextureManager::Get();
		tm->BindPS(renderObject.diffuseMapID, 0);
		tm->BindPS(renderObject.normalMapID, 1);
		tm->BindPS(renderObject.specMapID, 2);
		tm->BindPS(renderObject.bumpMapID, 3);

		renderObject.meshBuffer.Render();
	}

	void WaterEffect::DebugUI()
	{
		if (ImGui::CollapsingHeader("WaveLookEffect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool useSpecMap = mSettingsData.useSpecMap > 0;
			if (ImGui::Checkbox("UseSpecMap##Terrain", &useSpecMap))
			{
				mSettingsData.useSpecMap = useSpecMap ? 1 : 0;
			}
			bool useShadowMap = mSettingsData.useShadowMap > 0;
			if (ImGui::Checkbox("UseShadowMap##Terrain", &useShadowMap))
			{
				mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
			}
		}
		if (ImGui::CollapsingHeader("WaveEffect", ImGuiTreeNodeFlags_DefaultOpen))
		{
			float waveHeight = mWaterData.waveHeight;
			if (ImGui::DragFloat("waveHeight##Wave", &waveHeight, 0.05f, 0.0f, 10.0f))
			{
				mWaterData.waveHeight = waveHeight;
			}
			float waveStrength = mWaterData.waveStrength;
			if (ImGui::DragFloat("waveStrength##Wave", &waveStrength, 0.05f, 0.0f, 10.0f))
			{
				mWaterData.waveStrength = waveStrength;
			}
			ImGui::DragFloat("mTimeMultiplier##Wave", &mTimeMultiplier, 0.05f, 0.0f, 10.0f);
		}
	}

	void WaterEffect::SetCamera(const Camera& camera)
	{
		mCamera = &camera;
	}

	void WaterEffect::SetLightCamera(const Camera& camera)
	{
		mLightCamera = &camera;
	}

	void WaterEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
	{
		mDirectionalLight = &directionalLight;
	}

	void WaterEffect::SetShadowMap(const Texture& shadowMap)
	{
		mShadowMap = &shadowMap;
	}
}