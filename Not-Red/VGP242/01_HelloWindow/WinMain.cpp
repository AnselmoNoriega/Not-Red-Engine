#include <Not-Red/Inc/NotRed.h>

using namespace NotRed;

class GameState : public AppState
{
public:
	virtual ~GameState() = default;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Window";

	App myApp;
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return(0);
}