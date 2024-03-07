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

    mPlanets.push_back(std::make_shared<Planet>("../../Assets/Images/planets/sun.jpg", 2, 10));
}

void MainState::Terminate()
{
    for (auto planet : mPlanets)
    {
        planet->Terminate();
    }
}

void MainState::Update(float dt)
{
    CameraControllers(dt, mCamera);

    for (auto planet : mPlanets)
    {
        planet->Update(dt, 1, 1);
    }
}

void MainState::Render()
{
    for(auto planet : mPlanets)
    {
        planet->Render(mCamera);
    }
    SimpleDraw::AddGroundPlane(50, Colors::Gray);
    SimpleDraw::Render(mCamera);
}

void MainState::DebugUI()
{
    ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    

    ImGui::End();
}

void MainState::CreateShapes()
{
}
