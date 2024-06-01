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
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f,0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.5f, 0.5f,0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 0.1f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    Model model;
    ModelIO::LoadModel("../../Assets/Models/Maw/Maw.model", model);
    ModelIO::LoadMaterial("../../Assets/Models/Maw/Maw.model", model);
    mCharacter = CreateRenderGroup(model);

    Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(20, 20, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

    std::filesystem::path shaderFilePath = (L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
    mStandardEffect.SetShadowMapFar(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mCharacterPos = GetMatrix({ 1.0f, 0.0f, 0.0f });
    mGroundPos = GetMatrix({ 0.0f, 0.0f, 0.0f });
}

void MainState::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
    CleanRenderGroup(mCharacter);
}

void MainState::Update(float dt)
{
    CameraControllers(dt);
}

void MainState::Render()
{
    mShadowEffect.Begin();
		DrawRenderGroup(mShadowEffect, mCharacter, mCharacterPos);
	mShadowEffect.End();

	mStandardEffect.Begin();
		DrawRenderGroup(mStandardEffect, mCharacter, mCharacterPos);
		mStandardEffect.Render(mGround, mGroundPos);
	mStandardEffect.End();
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

    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();

    ImGui::End();
}