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
			MainApp().ChangeState("Sphere");
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
		{
			MainApp().ChangeState("Transform");
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
		{
			MainApp().ChangeState("AABB");
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
		{
			MainApp().ChangeState("FilledAABB");
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::FIVE))
		{
			MainApp().ChangeState("Line");
		}
	}
}

/// <summary>
/// Texure
/// </summary>
void MainState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 10.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 10.0f, -9.0f });
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
	SimpleDraw::AddGroundPlane(50, Colors::Gray);
	SimpleDraw::Render(mCamera);
	DrawShape();
}
