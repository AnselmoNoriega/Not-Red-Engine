#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Input;

void MainState::CameraControllers(float dt)
{
    auto input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.8f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * dt);
    }

    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * dt);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * dt);
    }
}

Math::Matrix4 GetMatrix(const Math::Vector3& position)
{
    const Math::Vector3 l = { 0.0f, 0.0f, 1.0f };
    const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
    const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
    const float x = -Math::Dot(r, position);
    const float y = -Math::Dot(u, position);
    const float z = -Math::Dot(l, position);

    return
    {
        r.x, u.x, l.x, 0.0f,
        r.y, u.y, l.y, 0.0f,
        r.z, u.z, l.z, 0.0f,
        x,   y,   z,   1.0f
    };
}

void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, 5.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    {
        Model modelA;
        ModelIO::LoadModel("../../Assets/Models/Maw/Maw.model", modelA);
        ModelIO::LoadMaterial("../../Assets/Models/Maw/Maw.model", modelA);
        mRenderGroupA = CreateRenderGroup(modelA);
    }
    {
        Model modelB;
        ModelIO::LoadModel("../../Assets/Models/Paladin/Paladin.model", modelB);
        ModelIO::LoadMaterial("../../Assets/Models/Paladin/Paladin.model", modelB);
        mRenderGroupB = CreateRenderGroup(modelB);
    }

    Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(20, 20, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

    MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    shaderFilePath = L"../../Assets/Shaders/PostProcessing.fx";
    mPostPricessingEffect.Initialize(shaderFilePath);
    mPostPricessingEffect.SetTexture(&mRenderTarget);
    mPostPricessingEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);
    mPostPricessingEffect.SetMode(PostPricessingEffect::Mode::Combine2);

    mGaussianBlurEffect.Initialize();
    mGaussianBlurEffect.SetSourceTexture(mBlurRenderTarget);

    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth();
    const uint32_t screenHeight = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
    mBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    mPositionA = GetMatrix({ 1.0f, 0.0f, 0.0f });
    mPositionB = GetMatrix({ -1.0f, 0.0f, 0.0f });
}

void MainState::Terminate()
{
    mBlurRenderTarget.Terminate();
    mRenderTarget.Terminate();
    mGaussianBlurEffect.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
    mScreenQuad.Terminate();
    mPostPricessingEffect.Terminate();

    CleanRenderGroup(mRenderGroupA);
    CleanRenderGroup(mRenderGroupB);
}

void MainState::Update(float dt)
{
    CameraControllers(dt);
}

void MainState::Render()
{
    mRenderTarget.BeginRender();
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mRenderGroupA, mPositionA);
            DrawRenderGroup(mStandardEffect, mRenderGroupB, mPositionB);
            mStandardEffect.Render(mGround, Math::Matrix4::Identity);
        mStandardEffect.End();
    mRenderTarget.EndRender();

    mBlurRenderTarget.BeginRender();
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mRenderGroupA, mPositionA);
            DrawRenderGroup(mStandardEffect, mRenderGroupB, mPositionB);
        mStandardEffect.End();
    mBlurRenderTarget.EndRender();

    mGaussianBlurEffect.Begin();
        mGaussianBlurEffect.Render(mScreenQuad);
    mGaussianBlurEffect.End();

    mPostPricessingEffect.Begin();
        mPostPricessingEffect.Render(mScreenQuad);
    mPostPricessingEffect.End();
}

void MainState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }

    ImGui::Separator();
    ImGui::Text("RenderTarget:");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 }
    );

    mStandardEffect.DebugUI();
    mPostPricessingEffect.DebugUI();
    mGaussianBlurEffect.DebugUI();

    ImGui::End();
}