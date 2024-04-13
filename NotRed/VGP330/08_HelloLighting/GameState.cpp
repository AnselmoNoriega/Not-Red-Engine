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

/// <summary>
/// Texure
/// </summary>
void MainState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10.0f, -20.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });
    
    DebugUI::SetTheme(DebugUI::Theme::Custom);
}

void MainState::Terminate()
{

}

void MainState::Update(float dt)
{
    CameraControllers(dt, mCamera);
    CheackStates();
}

void MainState::Render()
{
    SimpleDraw::AddTransform(Math::Matrix4::Translation(shapePos));
    SimpleDraw::Render(mCamera);
}

void MainState::DebugUI()
{
    ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::LabelText("Assignment 6", "Info");
    if (ImGui::CollapsingHeader("World Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Draw Plane", &drawPlane);
        ImGui::DragFloat3("Drag", &shapePos.x, 0.01f, -5.0f, 5.0f);
        ImGui::Combo("ShapeType", &shapeType, shapeTypes, 4);
    }
    if (ImGui::CollapsingHeader("Shape Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit3("ShapeColor", &shapeColor.r);
        ImGui::DragFloat("Alpha", &shapeColor.a, 0.01f, 0.0f, 1.0f);
    }

    ImGui::LabelText("", "Shape Info");
    switch (shapeType)
    {
    case 1:
    {
        SimpleDraw::AddSphere(shapeSize.x, shapeSize.y, shapeSize.z, shapeColor);
        ImGui::DragFloat3("Sphere Info", &shapeSize.x, 1.00f, 1.0f, 50.0f);
        break;
    }
    case 2:
    {
        SimpleDraw::AddAABB(minBoxSize.x, minBoxSize.y, minBoxSize.z, maxBoxSize.x, maxBoxSize.y, maxBoxSize.z, shapeColor);
        ImGui::DragFloat3("Min Pos Info", &minBoxSize.x, 1.00f, -50.0f, 50.0f);
        ImGui::DragFloat3("Max Pos Info", &maxBoxSize.x, 1.00f, -50.0f, 50.0f);
        break;
    }
    case 3:
    {
        SimpleDraw::AddFilledAABB(minBoxSize.x, minBoxSize.y, minBoxSize.z, maxBoxSize.x, maxBoxSize.y, maxBoxSize.z, shapeColor);
        ImGui::DragFloat3("Min Pos Info", &minBoxSize.x, 1.00f, -50.0f, 50.0f);
        ImGui::DragFloat3("Max Pos Info", &maxBoxSize.x, 1.00f, -50.0f, 50.0f);
        break;
    }
    default:
        break;
    }
    if (drawPlane)
    {
        SimpleDraw::AddGroundPlane(50, Colors::Gray);
    }

    ImGui::End();
}
