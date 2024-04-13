#include <Not-Red/Inc/NotRed.h>

#include "GameState.h"

using namespace NotRed;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = NotRed::MainApp();
	myApp.AddState<ShapeOne>("ShapeOne");
	myApp.AddState<ShapeTwo>("ShapeTwo");
	myApp.AddState<ShapeThree>("ShapeThree");

	AppConfig config;
	config.appName = L"This Shape";

	myApp.Run(config);

	return(0);
}