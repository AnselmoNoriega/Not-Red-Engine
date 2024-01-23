#include "Precompiled.h"
#include "Not-Red/Inc/App.h"
#include "Not-Red/Inc/AppState.h"

using namespace NotRed;
using namespace NotRed::Core;

void App::Run(const AppConfig& config)
{
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);
	ASSERT(myWindow.IsActive(), "Failed in creating a Window");

	ASSERT(mCurrentState != nullptr, "APP: need an app state");
	mCurrentState->Initialize();

	LOG("APP Started: %.3f", TimeUtil::GetTime());
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		if (!myWindow.IsActive())
		{
			Quit();
			break;
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		mCurrentState->Update(TimeUtil::GetdeltaTime());
	}

	mCurrentState->Terminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}

