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
	int GiveDistance(int& num);

private:
	NotRed::Graphics::Camera mCamera;

	std::vector<std::unique_ptr<Planet>> mPlanets;
	std::unique_ptr<Galaxy> mGalaxy;

	NotRed::Graphics::ConstantBuffer mConstantBuffer;
	NotRed::Graphics::VertexShader mVertexShader;
	NotRed::Graphics::PixelShader mPixelShader;
	NotRed::Graphics::Sampler mSampler;

	std::pair<float, float> mPlanetSpeeds[10];

	bool mDrawRings = true;
	bool mDrawPlane = true;

	float mTimeSpeed = 1.0f;
};
