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

	NotRed::Graphics::DirectionalLight mDirectionalLight;
	NotRed::Graphics::Camera mCamera;

	NotRed::Graphics::ModelID mModelID;
	NotRed::Graphics::RenderGroup mCharacter;
	NotRed::Graphics::Animator mCharacterAnimator;
	NotRed::Graphics::StandardEffect mStandardEffect;

	int mAnimIndex = -1;
	bool mDrawSkeleton = false;
};