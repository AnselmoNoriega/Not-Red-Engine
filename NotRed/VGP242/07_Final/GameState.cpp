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

const char* PlanetNames[] =
{
"None",
"Mercury",
"Venus",
"Earth",
"Mars",
"Jupiter",
"Saturn",
"Uranus",
"Neptune",
"Pluto"
};

void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 30.0f, -90.0f });
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
        mPlanetSpeeds[i].first = ((rand() % 40) / 10.0f) + 1;
        mPlanetSpeeds[i].second = ((rand() % 50) / 10) + 1;
    }
}

void MainState::Terminate()
{
    mGalaxy->Terminate();

    for (int i = 0; i < 10; ++i)
    {
        mPlanets[i]->Terminate();
    }

    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
}

void MainState::Update(float dt)
{
    if (!mLookAtPlanet)
    {
        CameraControllers(dt);
    }
    else
    {
        mCamera.SetPosition(mPlanets[mLookAtPlanet]->GetLookPosition());
        mCamera.SetLookAt(mPlanets[mLookAtPlanet]->GetLookDirection());
    }

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
        ImGui::Combo("CheckPlanet", &mLookAtPlanet, PlanetNames, 10);
    }

    ImGui::End();
}

void MainState::CreateShapes()
{
    mGalaxy = std::make_unique<Galaxy>("../../Assets/Images/skysphere/space.jpg", 500);

    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/sun.jpg", 40, 0));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/mercury.jpg", 1.5f, 50));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/venus.jpg", 2, 55));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/earth/earth.jpg", 2.5f, 60));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/mars.jpg", 3, 70));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/jupiter.jpg", 4, 90));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/saturn.jpg", 5, 100));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/uranus.jpg", 6, 120));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/neptune.jpg", 7, 140));
    mPlanets.push_back(std::make_unique<Planet>("../../Assets/Images/planets/pluto.jpg", 0.5f, 160));
}
