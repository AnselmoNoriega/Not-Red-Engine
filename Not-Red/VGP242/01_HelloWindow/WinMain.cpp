#include <Not-Red/Inc/NotRed.h>

using namespace NotRed;
using namespace NotRed::Graphics;

class MainState : public AppState
{
public:
	void Initialize()
	{
		LOG("MAIN INITIALIZED");
		GraphicsSystem::Get()->SetClearColor(Colors::Red);
	}

	void Terminate()
	{
		LOG("MAIN TERMINATED");
	}

	void Update(float dt)
	{
	}
};

class GameState : public AppState
{
public:
	void Initialize()
	{
		LOG("GAME INITIALIZED");
		GraphicsSystem::Get()->SetClearColor(Colors::Blue);
	}

	void Terminate()
	{
		LOG("GAME TERMINATED");
	}

	void Update(float dt)
	{
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = NotRed::MainApp();
	myApp.AddState<MainState>("MainState");
	myApp.AddState<GameState>("GameState");

	AppConfig config;
	config.appName = L"This Window";

	myApp.Run(config);

	return(0);
}