#pragma once

#include "Planet.h"

class MainState : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;

private:
	void CreateShapes();

private:
	NotRed::Graphics::Camera mCamera;

	std::vector<std::shared_ptr<Planet>> mPlanets;
};
