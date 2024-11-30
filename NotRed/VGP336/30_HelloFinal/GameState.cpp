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
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.9f, 0.9f,0.9f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 0.1f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	{
		Model model;
		ModelIO::LoadModel("../../Assets/Models/Maw/Maw.model", model);
		ModelIO::LoadMaterial("../../Assets/Models/Maw/Maw.model", model);
		mCharacter = CreateRenderGroup(model);
	}
	{
		const Mesh& m = NotRed::Graphics::MeshBuilder::CreateCone(100, 10.0f, 5.0f);
		mLight.meshBuffer.Initialize(
			m.vertices.data(),
			static_cast<uint32_t>(sizeof(Vertex)),
			static_cast<uint32_t>(m.vertices.size()),
			m.indices.data(),
			static_cast<uint32_t>(m.indices.size())
		);
	}
	{
		std::filesystem::path shaderFilePath = (L"../../Assets/Shaders/Standard.fx");
		mStandardEffect.Initialize(shaderFilePath);
	}

	MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenQuad);

	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mDepthEffect.Initialize();
	mDepthEffect.SetCamera(mCamera);

	mVolumetricLighting.Initialize();
	mVolumetricLighting.SetCamera(mCamera);
	//mVolumetricLighting.SetShadowMap(mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	//PostProcessing
	mVolumetricLighting.SetTextures(&mRenderTarget, &mDepthBuffer);

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mDepthBuffer.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
}

void MainState::Terminate()
{
	mDepthBuffer.Terminate();
	mRenderTarget.Terminate();
	mShadowEffect.Terminate();

	mVolumetricLighting.Terminate();
	
	mDepthEffect.Terminate();
	mStandardEffect.Terminate();

	mLight.Terminate();
	CleanRenderGroup(mCharacter);
}

void MainState::Update(float dt)
{
	CameraControllers(dt);
}

void MainState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mShadowEffect.SetFocus(mCamera.GetPosition());

	mShadowEffect.Begin();
	DrawRenderGroup(mShadowEffect, mCharacter);
	mShadowEffect.End();

	mRenderTarget.BeginRender();
	mStandardEffect.Begin();
	DrawRenderGroup(mStandardEffect, mCharacter);
	mStandardEffect.End();
	mRenderTarget.EndRender();

	mDepthBuffer.BeginRender(Color(0.0f, 0.0f, 0.0f, 0.0f));
	mDepthEffect.Begin();
	DrawRenderGroup(mDepthEffect, mCharacter);
	mDepthEffect.End();
	mDepthBuffer.EndRender();

	mVolumetricLighting.Render(mLight, mScreenQuad);
}

void MainState::DebugUI()
{/*
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
	mWaterEffect.DebugUI();
	mShadowEffect.DebugUI();
	ImGui::End();*/
}