#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Audio;

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
		SpacePressedEvent spacePressed;
		EventManager::Broadcast(&spacePressed);
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

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mBikerID = ModelManager::Get()->LoadModel("../../Assets/Models/Capoeira/Capoeira.model");
	ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/Capoeira/Capoeira.animset");
	mBiker = CreateRenderGroup(mBikerID, &mBikerAnimator);
	mBikerAnimator.Initialize(mBikerID);
	{
		mEventTime = 0.0f;
		mBikerEvent = AnimationBuilder()
			.AddPositionKey({ -10.0f, 0.0f, 0.0f }, 0.0f)
			.AddEventKey([&]() {mBikerAnimator.PlayAnimation(1, true); }, 2.0f)
			.AddEventKey([]() {}, 20.0f)
			.Build();
	}

	Mesh obj = MeshBuilder::CreateHorizontalPlane(40, 40, 1.0f);
	mGround.meshBuffer.Initialize(obj);
	mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
}

void MainState::Terminate()
{
	CleanRenderGroup(mBiker);
	mStandardEffect.Terminate();
}

void MainState::Update(const float deltaTime)
{
	float prevTime = mEventTime;
	mEventTime += deltaTime;
	mBikerEvent.PlayEvents(prevTime, mEventTime);
	while (mEventTime >= mBikerEvent.GetDuration())
	{
		mEventTime -= mBikerEvent.GetDuration();
	}

	mBikerAnimator.Update(deltaTime);

	UpdateCameraControl(deltaTime);
}

void MainState::Render()
{
	for (auto& ro : mBiker)
	{
		ro.transform = mBikerEvent.GetTransform(mEventTime);
	}

	mStandardEffect.Begin();
	{
		DrawRenderGroup(mStandardEffect, mBiker);
		mStandardEffect.Render(mGround);
	}
	mStandardEffect.End();
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
	ImGui::End();
}