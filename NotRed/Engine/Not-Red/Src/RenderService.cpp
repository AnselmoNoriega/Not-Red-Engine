#include "Precompiled.h"
#include "RenderService.h"

#include "CameraService.h"

#include "RenderObjectComponent.h"
#include "TransformComponent.h"

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
}

void RenderService::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
    mFPS = 1 / deltaTime;
}

void RenderService::Render()
{
    const Camera& camera = mCameraService->GetMain();
    mStandardEffect.SetCamera(camera);

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

    mStandardEffect.Begin();
    for (Entry& entry : mRenderEntries)
    {
        DrawRenderGroup(mShadowEffect, entry.renderGroup);
    }
    mStandardEffect.End();
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
}

void RenderService::Register(const RenderObjectComponent* renderObjectComponent)
{
    Entry& entry = mRenderEntries.emplace_back();

    entry.renderComponent = renderObjectComponent;
    entry.transformComponent = renderObjectComponent->GetOwner().GetComponent<TransformComponent>();
    if (renderObjectComponent->GetModelId() > 0)
    {
        entry.renderGroup = CreateRenderGroup(renderObjectComponent->GetModelId());
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