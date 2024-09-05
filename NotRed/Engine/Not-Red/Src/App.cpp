#include "Precompiled.h"
#include "App.h"
#include "AppState.h"
#include "EventManager.h"

using namespace NotRed;
using namespace NotRed::Core;
using namespace NotRed::Graphics;
using namespace NotRed::Input;
using namespace NotRed::Physics;
using namespace NotRed::Audio;

void App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}

void App::Run(const AppConfig& config)
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight
    );
    ASSERT(myWindow.IsActive(), "Failed to create a window");
    auto handle = myWindow.GetWindowHandle();;
    GraphicsSystem::StaticInitialize(handle, false);
    InputSystem::StaticInitialize(handle);
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticInitialize(config.maxVertexCount);
    TextureManager::Initialize("../../Assets/Images/");
    ModelManager::StaticInitialize();

    AudioSystem::StaticInitialize();
    SoundEffectManager::StaticInitialize("../../Assets/Sounds");
    EventManager::StaticInitialize();

    PhysicsWorld::Settings settings;
    PhysicsWorld::StaticInitialize(settings);
        
    ASSERT(mCurrentState, "App: no app state found");
    mCurrentState->Initialize();

    mRunning = true;
    while (mRunning)
    {
        myWindow.ProcessMessage();
        InputSystem* input = InputSystem::Get();
        input->Update();

        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            continue;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        AudioSystem::Get()->Update();

        auto deltaTime = TimeUtil::GetdeltaTime();
        if (deltaTime < 0.5f)
        {
            PhysicsWorld::Get()->Update(deltaTime);
            mCurrentState->Update(deltaTime);
        }
        GraphicsSystem* gs = GraphicsSystem::Get();

        gs->BeginRender();
        {
            mCurrentState->Render();

            DebugUI::BeginRender();
            {
                mCurrentState->DebugUI();
            }
            DebugUI::EndRender();
        }
        gs->EndRender();
    }

    mCurrentState->Terminate();

    EventManager::StaticTerminate();
    SoundEffectManager::StaticTerminate();
    AudioSystem::StaticTerminate();

    PhysicsWorld::StaticTerminate();
    ModelManager::StaticTerminate();
    TextureManager::Terminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    InputSystem::StaticTerminate();
    GraphicsSystem::StaticTerminate();
    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}
