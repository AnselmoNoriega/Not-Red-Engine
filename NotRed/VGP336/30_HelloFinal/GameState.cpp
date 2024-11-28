#include "GameState.h"

#include "VolumetricLightingComponent.h"
#include "VolumetricLightingService.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;
using namespace NotRed::Input;
using namespace NotRed::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }

    return nullptr;
}
Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }

    return nullptr;
}
Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }

    return nullptr;
}

void MainState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);
    GameWorld::SetCustomService(CustomServiceMake);

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
    UIFont::Get()->DrawString(L"Hello World", { 100.0f, 100.0f }, 20.0f, Colors::Aqua);
}

void MainState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        mGameWorld.DebugUI();
    }
    ImGui::End();
}