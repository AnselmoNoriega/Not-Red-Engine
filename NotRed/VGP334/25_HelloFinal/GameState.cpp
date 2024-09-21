#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Audio;
using namespace NotRed::Physics;

void MainState::UpdateCameraControl(float dt)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.8f;

	/*if (input->IsKeyDown(KeyCode::W))
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
	}*/

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
		Mesh obj = MeshBuilder::CreateHorizontalPlane(40, 40, 1.0f);
		mGround.meshBuffer.Initialize(obj);
		mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");

		obj = MeshBuilder::CreateSphere(100, 100, 10.0f);
		mMeteorite.meshBuffer.Initialize(obj);
		mMeteorite.diffuseMapID = TextureManager::Get()->LoadTexture("Terrain-Height-Maps/terrain_map-0.jpg");

		mMeteorAnim = AnimationBuilder()
			.AddPositionKey({ 10.0f, 30.0f, 10.0f }, 0.0f)
			.AddEventKey([&]() {mCamera.SetPosition({ -9.0f, 3.0f, -4.33f }); mCamera.Yaw(-1.0f); }, 0.1f)
			.AddEventKey([&]() {mCamera.SetPosition({11.4f, 3.6f, -3.5f});  mCamera.Yaw(2.0f); }, 5.1f)
			.AddEventKey([&]() {mCamera.SetPosition({0.0f, 5.0f, -10.0f});  mCamera.Yaw(-1.0f); }, 11.1f)
			.AddEventKey([&]() {mCamera.SetPosition({2.0f, 2.0f, -1.0f});  mCamera.Yaw(-1.5f);  mCamera.Pitch(-0.2f); }, 19.6f)
			.AddEventKey([&]() {mCamera.SetPosition({0.3f, 2.5f, 2.0f});  mCamera.Yaw(-1.2f); }, 23.0f)
			.AddEventKey([&]() {mCamera.SetPosition({-0.2f, 1.4f, -2.0f});  mCamera.Yaw(3.0f);  mCamera.Pitch(-0.3f); }, 27.0f)
			.AddEventKey([&]() {mCamera.SetPosition({-0.2f, 1.0f, -2.3f});  mCamera.Yaw(0.5f);  mCamera.Pitch(-0.3f); }, 32.0f)
			.AddPositionKey({ 10.0f, 30.0f, 10.0f }, 32.0f)
			.AddEventKey([&]() {mCamera.SetPosition({0.0f, 5.0f, -20.0f});  mCamera.Yaw(-0.8f);  mCamera.Pitch(0.5f); }, 35.0f)
			.AddPositionKey({ 0.0f, 0.5f, 0.0f }, 37.0f)
			.AddPositionKey({ 0.0f, 0.5f, 0.0f }, 100.0f)
			.Build();
	}
	{
		MeshPX obj = MeshBuilder::CreateSkySpherePX(100, 100, 60.0f);
		mSkyBox.meshBuffer.Initialize(obj);
		mSkyBox.diffuseMapID = TextureManager::Get()->LoadTexture("skysphere/sky.jpg");
	}
	{
		mBikerID = ModelManager::Get()->LoadModel("../../Assets/Models/WalkingBiker/Walking.model");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/Mike/Bboy.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/WalkingBiker/Walking.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/PunchingBag/PunchingBag.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/Dancing/Dancing.animset");
		ModelManager::Get()->AddAnimation(mBikerID, "../../Assets/Models/HipHopDancing/HipHopDancing.animset");
		mBiker = CreateRenderGroup(mBikerID, &mBikerAnimator);
		mBikerAnimator.Initialize(mBikerID);
		{
			mBikerEventTime = 0.0f;
			mBikerEvent = AnimationBuilder()
				.AddPositionKey({ -15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 0.0f)
				.AddScaleKey({ 2.0f, 2.0f, 2.0f }, 0.0f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(4, true); }, 0.1f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(1, true); }, 10.0f)
				.AddPositionKey({ -15.0f, 0.0f, 0.0f }, 10.0f)
				.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 17.5f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(2, false); }, 17.6f)
				.AddEventKey([&]() {mBikerAnimator.PlayAnimation(3, true); }, 19.6f)
				.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}
	{
		mGuyID = ModelManager::Get()->LoadModel("../../Assets/Models/Capoeira/Capoeira.model");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/Capoeira/Capoeira.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/WalkingBlonde/Walking.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/Dying/Dying.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/Crawling/Crawling.animset");
		mGuy = CreateRenderGroup(mGuyID, &mGuyAnimator);
		mGuyAnimator.Initialize(mGuyID);
		{
			mGuyEventTime = 0.0f;
			mGuyEvent = AnimationBuilder()
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, -80.0f), 0.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(0, true); }, 0.1f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(1, true); }, 11.0f)
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 11.0f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 17.5f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, false); }, 17.6f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, -80.0f), 20.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(3, true); }, 20.1f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 21.0f)
				.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 21.0f)
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 45.0f)
				.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}
	{
		mParticleEffect.Initialize();
		mParticleEffect.SetCamera(mCamera);

		ParticleSystemInfo info;
		info.maxParticle = 150;
		info.particleTextureId = TextureManager::Get()->LoadTexture("pikachu.png");
		info.spawnPosition = Math::Vector3::Zero;
		info.spawnDirection = Math::Vector3::YAxis;
		info.spawnDelay = 0.05f;
		info.spawnLifeTime = 9999.0f;
		info.minParticlesPerEmit = 2;
		info.maxParticlesPerEmit = 6;
		info.minTimeBetweenEmit = 0.15f;
		info.maxTimeBetweenEmit = 0.65f;
		info.minSpawnAngle = -30.0f * Math::Constants::Pi / 180.0f;
		info.maxSpawnAngle = -30.0f * Math::Constants::Pi / 180.0f;
		info.minSpeed = 10.0f;
		info.maxSpeed = 20.0f;
		info.minParticleLifeTime = 0.5f;
		info.maxParticleLifeTime = 1.0f;
		info.minStartColor = Colors::Red;
		info.maxStartColor = Colors::Yellow;
		info.minEndColor = Colors::White;
		info.maxEndColor = Colors::Orange;
		info.minStartScale = Math::Vector3::One;
		info.maxStartScale = { 1.5f, 1.5f,1.5f };
		info.minEndScale = { 0.15f, 0.15f, 0.15f };
		info.maxEndScale = { 1.5f, 1.5f,1.5f };
		mParticleSystem.Initialize(info);
		mParticleSystem.SetCamera(mCamera);
	}

	SoundEffectManager* sem = SoundEffectManager::Get();
	mBackgroundSound = sem->Load("megamanx_storm_eagle.wav");
	SoundEffectManager::Get()->Play(mBackgroundSound);
}

void MainState::Terminate()
{
	CleanRenderGroup(mBiker);
	CleanRenderGroup(mGuy);
	mStandardEffect.Terminate();
	mParticleEffect.Terminate();
	mParticleSystem.Terminate();
	mGround.Terminate();
	mMeteorite.Terminate();
	mSkyBox.Terminate();
}

void MainState::Update(const float deltaTime)
{
	{
		float prevTime = mBikerEventTime;
		mBikerEventTime += deltaTime * mAnimSpeed;

		mBikerEvent.PlayEvents(prevTime, mBikerEventTime);
		while (mBikerEventTime >= mBikerEvent.GetDuration())
		{
			mBikerEventTime -= mBikerEvent.GetDuration();
		}
	}
	{
		float prevTime = mGuyEventTime;
		mGuyEventTime += deltaTime * mAnimSpeed;
		mMeteorAnim.PlayEvents(prevTime, mBikerEventTime);

		mGuyEvent.PlayEvents(prevTime, mGuyEventTime);
		while (mGuyEventTime >= mGuyEvent.GetDuration())
		{
			mGuyEventTime -= mGuyEvent.GetDuration();
		}
	}
	mParticleSystem.SetPosition(mMeteorite.transform.position);
	mBikerAnimator.Update(deltaTime);
	mGuyAnimator.Update(deltaTime);

	UpdateCameraControl(deltaTime);
	mParticleSystem.Update(deltaTime);
}

void MainState::Render()
{
	mMeteorite.transform = mMeteorAnim.GetTransform(mGuyEventTime);

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
		mStandardEffect.Render(mMeteorite);
		mStandardEffect.Render(mSkyBox);
	}
	mStandardEffect.End();

	mParticleEffect.Begin();
	mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();
}

void MainState::DebugUI()
{
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("AnimSpeed", &mAnimSpeed, 0.1f);
		auto pos = mCamera.GetPosition();
		ImGui::DragFloat3("CamPos", &pos.x, 0.1f);
		mParticleEffect.DebugUI();
		mParticleSystem.DebugUI();
	}
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