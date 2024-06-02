#include "Precompiled.h"
#include "TerrainEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexTypes.h"

namespace NotRed::Graphics
{
	void TerrainEffect::Initialize()
	{
		std::filesystem::path shaderFile = "../../Assets/Shaders/Terrain.fx";
		mVertexShader.Initialize<Vertex>(shaderFile);
		mPixelShader.Initialize(shaderFile);

		mTransformBuffer.Initialize();
		mSettingsBuffer.Initialize();
		mLightBuffer.Initialize();
		mMaterialBuffer.Initialize();
		mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	}

	void TerrainEffect::Terminate()
	{
		mSampler.Terminate();
		mMaterialBuffer.Terminate();
		mLightBuffer.Terminate();
		mSettingsBuffer.Terminate();
		mTransformBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void TerrainEffect::Begin()
	{
		mVertexShader.Bind();
		mPixelShader.Bind();

		mTransformBuffer.BindVS(0);

		mSettingsBuffer.BindVS(1);
		mSettingsBuffer.BindPS(1);

		mLightBuffer.BindVS(2);
		mLightBuffer.BindPS(2);

		mMaterialBuffer.BindPS(3);

		mSampler.BindVS(0);
		mSampler.BindPS(0);
	}

	void TerrainEffect::End()
	{
		if (mShadowMap != nullptr)
		{
			Texture::UnbindPS(4);
		}
	}

	void TerrainEffect::Render(const RenderObject& renderObject, const Math::Matrix4& position)
	{
		SettingsData settingsData;
		settingsData.useNormalMap = renderObject.normalMapID > 0 && mSettingsData.useNormalMap > 0 ? 1 : 0;
		settingsData.useSpecMap = renderObject.specMapID > 0 && mSettingsData.useSpecMap > 0 ? 1 : 0;
		settingsData.useShadowMap = mShadowMap != nullptr && mSettingsData.useShadowMap > 0;
		settingsData.useBlend = renderObject.bumpMapID > 0 && mSettingsData.useBlend > 0;
		settingsData.depthBias = mSettingsData.depthBias;
		settingsData.blendHeight = mSettingsData.blendHeight;
		settingsData.blendWidth = mSettingsData.blendWidth;
		mSettingsBuffer.Update(settingsData);


		const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
		const Math::Matrix4 matView = mCamera->GetViewMatrix();
		const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

		Math::Matrix4 matFinal = position * matWorld * matView * matProj;

		TransformData transformData;
		transformData.wvp = Math::Transpose(matFinal);
		transformData.world = Math::Transpose(matWorld);
		transformData.viewPosition = mCamera->GetPosition();
		if (settingsData.useShadowMap > 0)
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

	void TerrainEffect::DebugUI()
	{
		if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
		{

			bool useNormalMap = mSettingsData.useNormalMap > 0;
			if (ImGui::Checkbox("UseNormalMap##Terrain", &useNormalMap))
			{
				mSettingsData.useNormalMap = useNormalMap ? 1 : 0;
			}
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
			ImGui::DragFloat("DepthBias##Terrain", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");

			bool useBlend = mSettingsData.useBlend > 0;
			if (ImGui::Checkbox("UseBlend##Terrain", &useBlend))
			{
				mSettingsData.useBlend = useBlend ? 1 : 0;
			}
			ImGui::DragFloat("BlendHeight##Terrain", &mSettingsData.blendHeight, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("BlendWidth##Terrain", &mSettingsData.blendWidth, 0.1f, 0.0f, 100.0f);
		}
	}

	void TerrainEffect::SetCamera(const Camera& camera)
	{
		mCamera = &camera;
	}

	void TerrainEffect::SetLightCamera(const Camera& camera)
	{
		mLightCamera = &camera;
	}

	void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
	{
		mDirectionalLight = &directionalLight;
	}

	void TerrainEffect::SetShadowMap(const Texture& shadowMap)
	{
		mShadowMap = &shadowMap;
	}
}