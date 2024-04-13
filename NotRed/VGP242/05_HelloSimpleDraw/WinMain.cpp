#include <Not-Red/Inc/NotRed.h>

#include "GameState.h"

using namespace NotRed;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = NotRed::MainApp();
	myApp.AddState<Sphere>("Sphere");
	myApp.AddState<Transform>("Transform");
	myApp.AddState<AABB>("AABB");
	myApp.AddState<FilledAABB>("FilledAABB");
	myApp.AddState<Line>("Line");

	AppConfig config;
	config.appName = L"This Shape";

	myApp.Run(config);

	return(0);
}