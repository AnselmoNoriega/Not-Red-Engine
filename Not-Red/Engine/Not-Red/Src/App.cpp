#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace NotRed;
using namespace NotRed::Core;

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
		if (deltaTime < 0.5f)
		{
			mCurrentState->Update(deltaTime);
		}
	}

	mCurrentState->Terminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
