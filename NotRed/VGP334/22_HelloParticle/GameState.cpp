#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Physics;

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

    if (input->IsKeyPressed(KeyCode::SPACE))
    {
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

    mParticleEffect.Initialize();
    mParticleEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.maxParticle = 150;
    info.particleTextureId = TextureManager::Get()->LoadTexture("pikachu.png");
    info.spawnPosition = Math::Vector3::Zero;
    info.spawnDirection = Math::Vector3::YAxis;
    info.spawnDelay = 0.05f;
    info.spawnLifeTime = 9999.0f;
    info.minParticlesPerEmit = 2;
    info.maxParticlesPerEmit = 6;
    info.minTimeBetweenEmit = 0.15f;
    info.maxTimeBetweenEmit = 0.65f;
    info.minSpawnAngle = -30.0f * Math::Constants::Pi / 180.0f;
    info.maxSpawnAngle = -30.0f * Math::Constants::Pi / 180.0f;
    info.minSpeed = 10.0f;
    info.maxSpeed = 20.0f;
    info.minParticleLifeTime = 0.5f;
    info.maxParticleLifeTime = 1.0f;
    info.minStartColor = Colors::Red;
    info.maxStartColor = Colors::Yellow;
    info.minEndColor = Colors::White;
    info.maxEndColor = Colors::Orange;
    info.minStartScale = Math::Vector3::One;
    info.maxStartScale = { 1.5f, 1.5f,1.5f };
    info.minEndScale = { 0.15f, 0.15f, 0.15f };
    info.maxEndScale = { 1.5f, 1.5f,1.5f };
    mParticleSystem.Initialize(info);
    mParticleSystem.SetCamera(mCamera);
}

void MainState::Terminate()
{
    mParticleEffect.Terminate();
}

void MainState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);
    mParticleSystem.Update(deltaTime);
}

void MainState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::WhiteSmoke);
    SimpleDraw::Render(mCamera);

    mParticleEffect.Begin();
    mParticleSystem.Render(mParticleEffect);
    mParticleEffect.End();
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
    mParticleEffect.DebugUI();
    mParticleSystem.DebugUI();
    Physics::PhysicsWorld::Get()->DebugUI();

    ImGui::End();

    SimpleDraw::Render(mCamera);
}