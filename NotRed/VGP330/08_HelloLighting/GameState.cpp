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

void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 30.0f, -90.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    mMesh = MeshBuilder::CreateSpherePX(100, 100, 1.0f);
    mRenderObject.meshBuffer.Initialize(mMesh);
    mRenderObject.texture.Initialize(L"../../Assets/Images/planets/earth/earth.jpg");

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Texture.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
}

void MainState::Terminate()
{
    mStandardEffect.Terminate();
    mRenderObject.Terminate();
}

void MainState::Update(float dt)
{
    CameraControllers(dt);
}

void MainState::Render()
{
    mStandardEffect.Begin();
    mStandardEffect.Render(mRenderObject);
    mStandardEffect.End();
}

void MainState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    mStandardEffect.DebugUI();

    ImGui::End();
}

void MainState::CreateShapes()
{

}
