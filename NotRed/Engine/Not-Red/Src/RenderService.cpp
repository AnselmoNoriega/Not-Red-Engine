#include "Precompiled.h"
#include "RenderService.h"

#include "CameraService.h"

#include "RenderObjectComponent.h"
#include "VolumetricLightingComponent.h"
#include "TransformComponent.h"
#include "AnimatorComponent.h"

#include "GameWorld.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void RenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();

	mDirectionalLight.direction = Normalize({ 1.0, -1.0, 1.0 });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	mDepthEffect.Initialize("../../Assets/Shaders/NewDepth.fx");
	mNormalsEffect.Initialize();

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mRenderTargetHelper.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mDepthBuffer.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mNormalsBuffer.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	std::filesystem::path shaderFile = "../../Assets/Shaders/PostProcessingBasic.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mPostProcessingTargets[0].Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mPostProcessingTargets[1].Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mVolumetricLighting.Initialize();
	mVolumetricLighting.SetTextures(&mDepthBuffer, &mNormalsBuffer);

	MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenQuad);
}

void RenderService::Terminate()
{
	mVolumetricLighting.Terminate();
	mNormalsEffect.Terminate();
	mDepthEffect.Terminate();

	mPostProcessingTargets[0].Terminate();
	mPostProcessingTargets[1].Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();

	mScreenQuad.Terminate();
	mRenderTargetHelper.Terminate();
	mRenderTarget.Terminate();
	mNormalsBuffer.Terminate();
	mDepthBuffer.Terminate();

	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
	const Camera& camera = mCameraService->GetMain();
	mStandardEffect.SetCamera(camera);
	mDepthEffect.SetCamera(camera);
	mNormalsEffect.SetCamera(camera);

	for (Entry& entry : mRenderEntries)
	{
		for (RenderObject& renderObject : entry.renderGroup)
		{
			renderObject.transform = *entry.transformComponent;
		}
	}

	mShadowEffect.Begin();
	for (Entry& entry : mRenderEntries)
	{
		if (entry.renderComponent->CanCastShadow())
		{
			DrawRenderGroup(mShadowEffect, entry.renderGroup);
		}
	}
	mShadowEffect.End();

	mRenderTarget.BeginRender();
	mStandardEffect.Begin();
	for (Entry& entry : mRenderEntries)
	{
		DrawRenderGroup(mStandardEffect, entry.renderGroup);
	}
	mStandardEffect.End();
	mRenderTarget.EndRender();

	mDepthBuffer.BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));
	mDepthEffect.Begin();
	for (Entry& entry : mRenderEntries)
	{
		DrawRenderGroup(mDepthEffect, entry.renderGroup);
	}
	mDepthEffect.End();
	mDepthBuffer.EndRender();

	mNormalsBuffer.BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));
	mNormalsEffect.Begin();
	for (Entry& entry : mRenderEntries)
	{
		DrawRenderGroup(mNormalsEffect, entry.renderGroup);
	}
	mNormalsEffect.End();
	mNormalsBuffer.EndRender();

	// Get render Texture
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mRenderTarget.BindPS(0);

	mPostProcessingTargets[1].BeginRender();
	mScreenQuad.meshBuffer.Render();
	mPostProcessingTargets[1].EndRender();

	int i = 0;
	if (mRenderVolumes.size() > 0)
	{
		mVolumetricLighting.SetCamera(mCameraService->GetMain());
		for (const VolumeEntry& entry : mRenderVolumes)
		{
			mVolumetricLighting.UpdateRenderImage(&mPostProcessingTargets[(i % 2) ? 0 : 1]);

			mVolumetricLighting.Render(
				entry.renderComponent->GetLightModelFront(),
				entry.renderComponent->GetLightModelBack());

			mPostProcessingTargets[i % 2].BeginRender();
			mVolumetricLighting.RenderScreenQuad(entry.renderComponent->GetLight(), mScreenQuad);
			mPostProcessingTargets[i % 2].EndRender();
			++i;
		}
	}

	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mPostProcessingTargets[(i == 1) ? 0 : 1].BindPS(0);

	mScreenQuad.meshBuffer.Render();
}

void RenderService::DebugUI()
{
	if (ImGui::CollapsingHeader("Rendering"))
	{
		ImGui::Text("FPS: %f", mFPS);
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
			{
				mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
			}
			ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
		}
		mStandardEffect.DebugUI();
		mShadowEffect.DebugUI();
	}

	if (ImGui::CollapsingHeader("Postprocessing"))
	{
		mVolumetricLighting.DebugUI();
	}
}

void RenderService::Register(const RenderObjectComponent* renderObjectComponent)
{
	Entry& entry = mRenderEntries.emplace_back();

	entry.renderComponent = renderObjectComponent;
	entry.transformComponent = renderObjectComponent->GetOwner().GetComponent<TransformComponent>();

	const AnimatorComponent* animatorComponent = renderObjectComponent->GetOwner().GetComponent<AnimatorComponent>();
	const Animator* animator = nullptr;
	if (animatorComponent != nullptr)
	{
		animator = &animatorComponent->GetAnimator();
	}

	if (renderObjectComponent->GetModelId() > 0)
	{
		entry.renderGroup = CreateRenderGroup(renderObjectComponent->GetModelId(), animator);
	}
	else
	{
		entry.renderGroup = CreateRenderGroup(renderObjectComponent->GetModel());
	}
}

void RenderService::Unregister(const RenderObjectComponent* renderObjectComponent)
{
	auto iter = std::find_if(
		mRenderEntries.begin(),
		mRenderEntries.end(),
		[&](const Entry& entry)
		{
			return entry.renderComponent == renderObjectComponent;
		});

	if (iter != mRenderEntries.end())
	{
		CleanRenderGroup(iter->renderGroup);
		mRenderEntries.erase(iter);
	}
}

void RenderService::Register(const VolumetricLightComponent* volumeObjectComponent)
{
	VolumeEntry& entry = mRenderVolumes.emplace_back();

	entry.renderComponent = volumeObjectComponent;
	entry.transformComponent = volumeObjectComponent->GetOwner().GetComponent<TransformComponent>();
}

void RenderService::Unregister(const VolumetricLightComponent* volumeObjectComponent)
{
	auto iter = std::find_if(
		mRenderVolumes.begin(),
		mRenderVolumes.end(),
		[&](const VolumeEntry& entry)
		{
			return entry.renderComponent == volumeObjectComponent;
		});

	if (iter != mRenderVolumes.end())
	{
		mRenderVolumes.erase(iter);
	}
}
