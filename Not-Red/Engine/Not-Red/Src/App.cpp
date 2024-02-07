#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace NotRed;
using namespace NotRed::Core;
using namespace NotRed::Graphics;

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
	GraphicsSystem::StaticInitialize(myWindow.GetWindowHandle(), false);

	ASSERT(mCurrentState, "App: no app state found");
	mCurrentState->Initialize();

	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		if (!myWindow.IsActive())
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

		auto deltaTime = TimeUtil::GetdeltaTime();
		mCurrentState->Update(deltaTime);
		GraphicsSystem* gs = GraphicsSystem::Get();
		gs->BeginRender();
		mCurrentState->Render();
		gs->EndRender();
	}

	mCurrentState->Terminate();
	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
