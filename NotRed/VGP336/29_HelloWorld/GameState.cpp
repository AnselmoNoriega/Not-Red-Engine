#include "GameState.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "")
    {
        return gameObject.AddComponent<Component>();
    }

    return nullptr;
}
Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    return nullptr;
}
Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
    return nullptr;
}

void MainState::Initialize()
{
    mGameWorld.LoadLevel(L"../../Assets/Saves/Levels/TestLevel.json");
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