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

void MainState::OnSpaceEvent(const NotRed::Event* event)
{
    LOG("SPACE EVENT OCCURRED");
    SoundEffectManager::Get()->Play(mEventSoundIds[2]);
}

void MainState::OnAnimEvent(const NotRed::Event* event)
{
    AnimEvent* eventData = (AnimEvent*)event;
    if (eventData->eventName == "Shoot")
    {
        LOG("SHOOT EVENT OCCURRED");
    }
}

void MainState::OnEvent2()
{
    SoundEffectManager::Get()->Play(mEventSoundIds[1]);
    AnimEvent animEvent;
    animEvent.eventName = "Explode";
    EventManager::Broadcast(&animEvent);
}

void MainState::OnEvent3()
{
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

    Model modelA;
    ModelIO::LoadModel("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
    ModelIO::LoadMaterial("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
    mCharacter = CreateRenderGroup(modelA);

    SoundEffectManager* sem = SoundEffectManager::Get();
    mEventSoundIds.push_back(sem->Load("megamanx_blast.wav"));
    mEventSoundIds.push_back(sem->Load("photongun1.wav"));
    mEventSoundIds.push_back(sem->Load("explosion.wav"));

    AnimationCallback cb = [&]() {SoundEffectManager::Get()->Play(mEventSoundIds[0]); };
    mEventAnimationTime = 0.0f;
    mEventAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 2.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 3.0f)
        .AddEventKey(cb, 1.0f)
        .AddEventKey(std::bind(&MainState::OnEvent2, this), 2.0f)
        .AddEventKey(std::bind(&MainState::OnEvent3, this), 3.0f)
        .Build();

    EventManager* em = EventManager::Get();
    mSpaceEventId = em->AddListener(EventType::SpacePressed, std::bind(&MainState::OnSpaceEvent, this, std::placeholders::_1));
    mAnimEventId = em->AddListener(EventType::AnimEvent, std::bind(&MainState::OnAnimEvent, this, std::placeholders::_1));
    //mSoundId = SoundEffectManager::Get()->Load("megamanx_shot.wav");

}

void MainState::Terminate()
{
    CleanRenderGroup(mCharacter);
    mStandardEffect.Terminate();
}

void MainState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);

    float prevTime = mEventAnimationTime;
    mEventAnimationTime += deltaTime;
    mEventAnimation.PlayEvents(prevTime, mEventAnimationTime);
    while (mEventAnimationTime >= mEventAnimation.GetDuration())
    {
        mEventAnimationTime -= mEventAnimation.GetDuration();
    }
}

void MainState::Render()
{
    for (auto& ro : mCharacter)
    {
        ro.transform = mEventAnimation.GetTransform(mEventAnimationTime);
    }
    mStandardEffect.Begin();
    DrawRenderGroup(mStandardEffect, mCharacter);
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
    if (ImGui::CollapsingHeader("Quaternion", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat("Yaw", &mYaw, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
        if (ImGui::DragFloat("Pitch", &mPitch, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
        if (ImGui::DragFloat("Roll", &mRoll, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
    }
    ImGui::End();
}