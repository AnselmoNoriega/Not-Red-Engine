#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;

void MainState::UpdateCameraControl(float dt)
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

void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    {
        Mesh obj;

        obj = MeshBuilder::CreateSphere(60, 60, 1.0f);
        mBall.meshBuffer.Initialize(obj);
        mBall.diffuseMapID = TextureManager::Get()->LoadTexture("materials/Foam/TCom_Various_AcousticFoam_4K_albedo.tif");

        obj = MeshBuilder::CreateHorizontalPlane(10, 10, 1.0f);
        mGround.meshBuffer.Initialize(obj);
        mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
    }

    mAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 0.5f, 1.0f }, 0.0f)

        .AddPositionKey({ 0.0f, 2.5f, 0.0f }, 2.0f)
        .AddScaleKey({ 1.0f, 1.5f, 1.0f }, 2.0f)

        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 4.0f)
        .AddScaleKey({ 1.0f, 0.5f, 1.0f }, 4.0f)

        .Build();
}

void MainState::Terminate()
{
    mStandardEffect.Terminate();
}

void MainState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);

    mAnimationTime += deltaTime;
    while (mAnimationTime > mAnimation.GetDuration())
    {
        mAnimationTime -= mAnimation.GetDuration();
    }
}

void MainState::Render()
{
    mBall.transform = mAnimation.GetTransform(mAnimationTime);

    mStandardEffect.Begin();
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mGround);
    mStandardEffect.End();
}

void MainState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }
    ImGui::End();
}
