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

	mDirectionalLight.direction = Math::Normalize({ 0.5f,-1.0f,0.5f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	{
		mBikerID = ModelManager::Get()->LoadModel("../../Assets/Models/WalkingBiker/Walking.model");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/Mike/Bboy.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/WalkingBiker/Walking.animset");
		mBiker = CreateRenderGroup(mBikerID, &mBikerAnimator);
		mBikerAnimator.Initialize(mBikerID);
		{
			mBikerEventTime = 0.0f;
			mBikerEvent = AnimationBuilder()
				.AddPositionKey({ -15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 0.0f)
				.AddScaleKey({2.0f, 2.0f, 2.0f}, 0.0f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(1, false); }, 0.1f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(2, true); }, 1.0f)
				.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 10.0f)
				.Build();
		}
	}
	{
		mGuyID = ModelManager::Get()->LoadModel("../../Assets/Models/Capoeira/Capoeira.model");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/Capoeira/Capoeira.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/WalkingBlonde/Walking.animset");
		mGuy = CreateRenderGroup(mGuyID, &mGuyAnimator);
		mGuyAnimator.Initialize(mGuyID);
		{
			mGuyEventTime = 0.0f;
			mGuyEvent = AnimationBuilder()
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, -80.0f), 0.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(1, true); }, 0.1f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, true); }, 3.0f)
				.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 10.0f)
				.AddEventKey([]() {}, 20.0f)
				.Build();
		}
	}

	Mesh obj = MeshBuilder::CreateHorizontalPlane(40, 40, 1.0f);
	mGround.meshBuffer.Initialize(obj);
	mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
}

void MainState::Terminate()
{
	CleanRenderGroup(mBiker);
	CleanRenderGroup(mGuy);
	mStandardEffect.Terminate();
}

void MainState::Update(const float deltaTime)
{
	{
		float prevTime = mBikerEventTime;
		mBikerEventTime += deltaTime;

		mBikerEvent.PlayEvents(prevTime, mBikerEventTime);
		while (mBikerEventTime >= mBikerEvent.GetDuration())
		{
			mBikerEventTime -= mBikerEvent.GetDuration();
		}
	}
	{
		float prevTime = mGuyEventTime;
		mGuyEventTime += deltaTime;

		mGuyEvent.PlayEvents(prevTime, mGuyEventTime);
		while (mGuyEventTime >= mGuyEvent.GetDuration())
		{
			mGuyEventTime -= mGuyEvent.GetDuration();
		}
	}

	mBikerAnimator.Update(deltaTime);
	mGuyAnimator.Update(deltaTime);

	UpdateCameraControl(deltaTime);
}

void MainState::Render()
{
	for (auto& ro : mBiker)
	{
		ro.transform = mBikerEvent.GetTransform(mBikerEventTime);
	}
	for (auto& ro : mGuy)
	{
		ro.transform = mGuyEvent.GetTransform(mGuyEventTime);
	}

	mStandardEffect.Begin();
	{
		DrawRenderGroup(mStandardEffect, mBiker);
		DrawRenderGroup(mStandardEffect, mGuy);
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