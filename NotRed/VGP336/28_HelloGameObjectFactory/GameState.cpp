#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Audio;

void MainState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<RenderService>();

    mGameWorld.Initialize();
    
    mGameWorld.CreateGameObject("Camera", "../../Assets/Saves/FPS_Cam.json");
    mGameWorld.CreateGameObject("Object0", "../../Assets/Saves/Obj.json");
}

void MainState::Terminate()
{
    mGameWorld.Terminate();
}

void MainState::Update(const float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void MainState::Render()
{
    mGameWorld.Render();
}

void MainState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        mGameWorld.DebugUI();
    }
    ImGui::End();
}