#pragma once

#include <Not-Red/Inc/NotRed.h>

class MainState : public NotRed::AppState
{
public:
	void Initialize()  override;
	void Terminate()  override;
	void Update(const float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::DirectionalLight mDirectionalLight;
	NotRed::Graphics::StandardEffect mStandardEffect;

	NotRed::Graphics::RenderGroup mBiker;
	NotRed::Graphics::ModelID mBikerID;
	NotRed::Graphics::Animator mBikerAnimator;
	NotRed::Graphics::Animation mBikerEvent;
	float mBikerEventTime;

	NotRed::Graphics::RenderGroup mGuy;
	NotRed::Graphics::ModelID mGuyID;
	NotRed::Graphics::Animator mGuyAnimator;
	NotRed::Graphics::Animation mGuyEvent;
	float mGuyEventTime;

	NotRed::Graphics::RenderObject mGround;
};