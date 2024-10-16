#pragma once

#include "Planet.h"
#include "Galaxy.h"

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
	void CameraControllers(float dt);

private:
	std::vector<std::unique_ptr<Planet>> mPlanets;
	std::unique_ptr<Galaxy> mGalaxy;

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::Mesh mMesh;
	NotRed::Graphics::ConstantBuffer mConstantBuffer;

	NotRed::Graphics::RenderObject mRenderObject;
	NotRed::Graphics::StandardEffect mStandardEffect;

	std::pair<float, float> mPlanetSpeeds[10];

	bool mDrawRings = true;
	bool mDrawPlane = true;

	float mTimeSpeed = 1.0f;
	int mLookAtPlanet = 0;
};
