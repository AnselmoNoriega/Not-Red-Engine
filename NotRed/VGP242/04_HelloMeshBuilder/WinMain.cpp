#include <Not-Red/Inc/NotRed.h>

#include "GameState.h"

using namespace NotRed;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	App& myApp = NotRed::MainApp();
	myApp.AddState<CubeState>("CubeState");
	myApp.AddState<RectState>("RectState");
	myApp.AddState<PlaneState>("PlaneState");
	myApp.AddState<SphereState>("SphereState");
	myApp.AddState<CylinderState>("CylinderState");
	myApp.AddState<SkyboxState>("SkyboxState");
	myApp.AddState<SkysphereState>("SkysphereState");

	AppConfig config;
	config.appName = L"This Shape";

	myApp.Run(config);

	return(0);
}