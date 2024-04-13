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
}

/// <summary>
/// Texure
/// </summary>
void ShapeTexture::Initialize()
{
	mCamera.SetPosition({ 0.0f, 0.0f, 0.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 1.0f });

	CreateShape();

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Texture.fx";

	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mTexture.Initialize(GetFilePath());
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeTexture::Terminate()
{
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
}

void ShapeTexture::Update(float dt)
{
	CameraControllers(dt, mCamera);
	CheackStates();
}

void ShapeTexture::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindPS(0);
	mSampler.BindPS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}

/// <summary>
/// Color
/// </summary>
void ShapeColor::Initialize()
{
	mCamera.SetPosition({ 0.0f, 4.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	CreateShape();

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Transform.fx";

	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize<VertexPC>(shaderFile);
	mPixelShader.Initialize(shaderFile);
}

void ShapeColor::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
}

void ShapeColor::Update(float dt)
{
	CameraControllers(dt, mCamera); 
	CheackStates();
}

void ShapeColor::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}
