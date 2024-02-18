#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Input;

void MainState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	CreateShape();

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Texture.fx";

	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mTexture.Initialize(L"../../Assets/Images/misc/basketball.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void MainState::Terminate()
{
	mSampler.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
}

void MainState::Update(float dt)
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

void MainState::CreateShape()
{
	// Shapes
	/*
	mMesh = MeshBuilder::CreatePyramidPC(2.0f);
	mMesh = MeshBuilder::CreateCubePC(2.0f);
	mMesh = MeshBuilder::CreateRectPC(2.0f, 0.5f, 1.0f);
	mMesh = MeshBuilder::CreateVerticalPlanePC(10, 10, 1.0f);
	mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);
	mMesh = MeshBuilder::CreateCylinderPC(100, 4);
	mMesh = MeshBuilder::CreateSpherePC(100, 100, 1.0f);*/

	mMesh.vertices.push_back({ {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} });
	mMesh.vertices.push_back({ {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f} });
	mMesh.vertices.push_back({ { 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f} });
	mMesh.vertices.push_back({ { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} });
	mMesh.indices = {
		0, 1, 2,
		0, 2, 3
	};
}

void MainState::Render()
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
