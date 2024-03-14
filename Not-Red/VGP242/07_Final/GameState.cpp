#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Input;


namespace
{
    void CameraControllers(float dt, Camera& camera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
        const float turnSpeed = 0.8f;

        if (input->IsKeyDown(KeyCode::W))
        {
            camera.Walk(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::A))
        {
            camera.Strafe(-moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::S))
        {
            camera.Walk(-moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::D))
        {
            camera.Strafe(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::E))
        {
            camera.Rise(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::Q))
        {
            camera.Rise(-moveSpeed * dt);
        }

        if (input->IsMouseDown(MouseButton::LBUTTON))
        {
            camera.Yaw(input->GetMouseMoveX() * turnSpeed * dt);
            camera.Pitch(input->GetMouseMoveY() * turnSpeed * dt);
        }
    }

    void CheackStates()
    {
        if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
        {
            MainApp().ChangeState("CubeState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
        {
            MainApp().ChangeState("RectState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
        {
            MainApp().ChangeState("PlaneState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
        {
            MainApp().ChangeState("SphereState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
        {
            MainApp().ChangeState("CylinderState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::SIX))
        {
            MainApp().ChangeState("SkyboxState");
        }
        else if (InputSystem::Get()->IsKeyPressed(KeyCode::SEVEN))
        {
            MainApp().ChangeState("SkysphereState");
        }
    }

    const char* shapeTypes[] =
    {
        "None",
        "Sphere",
        "AABB",
        "FilledAABB"
    };
}

void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10.0f, -20.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    DebugUI::SetTheme(DebugUI::Theme::Custom);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Texture.fx";

    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    CreateShapes();

    for (int i = 0; i < 10; ++i)
    {
        mPlanetSpeeds[i].first = (rand() % 40 + 1) / (10.0f + (i * 3));
        mPlanetSpeeds[i].second = (rand() % 50 + 1) / 10;
    }
}

void MainState::Terminate()
{
    mGalaxy->Terminate();

    for (auto planet : mPlanets)
    {
        planet->Terminate();
    }

    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
}

void MainState::Update(float dt)
{
    CameraControllers(dt, mCamera);

    for (int i = 0; i < 10; ++i)
    {
        mPlanets[i]->Update(dt, mPlanetSpeeds[i].first * mTimeSpeed, mPlanetSpeeds[i].second * mTimeSpeed);
    }
}

void MainState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    mGalaxy->Render(mCamera, mConstantBuffer);

    for (int i = 0; i < 10; ++i)
    {
        mPlanets[i]->Render(mCamera, mConstantBuffer);
        if (mDrawRings)
        {
            SimpleDraw::AddGroundCircle(40, mPlanets[i]->GetDistance(), Colors::Red);
        }
    }

    if (mDrawPlane)
    {
        SimpleDraw::AddGroundPlane(50, Colors::Gray);
    }

    SimpleDraw::Render(mCamera);
}

void MainState::DebugUI()
{
    ImGui::Begin("Final", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("World Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Draw Rings", &mDrawRings);
        ImGui::Checkbox("Draw Plane", &mDrawPlane);
    }
    if (ImGui::CollapsingHeader("Planets Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Time Speed", &mTimeSpeed, 0.01f, 0.0f, 10.0f);
    }

    ImGui::End();
}

void MainState::CreateShapes()
{
    mGalaxy = std::make_unique<Galaxy>("../../Assets/Images/skysphere/space.jpg", 500);

    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/sun.jpg", 40, 0));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/mercury.jpg", 1.5f, 50));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/venus.jpg", 2, 55));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/earth/earth.jpg", 2.5f, 60));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/mars.jpg", 3, 70));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/jupiter.jpg", 4, 90));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/saturn.jpg", 5, 100));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/uranus.jpg", 6, 120));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/neptune.jpg", 7, 140));
    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/pluto.jpg", 0.5f, 160));
}
