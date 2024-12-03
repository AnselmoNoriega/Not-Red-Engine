#pragma once

#include <Not-Red/Inc/NotRed.h>
#include "Effect/VolumetricLightingEffect.h"

class MainState : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;

private:
	void CameraControllers(float dt);

private:
	NotRed::Graphics::DirectionalLight mDirectionalLight;
	NotRed::Graphics::Camera mCamera;

	NotRed::Graphics::StandardEffect mStandardEffect;
	NotRed::Graphics::ShadowEffect mShadowEffect;
	NotRed::Graphics::VolumetricLightingEffect mVolumetricLighting;

	NotRed::Graphics::RenderGroup mCharacter;

	NotRed::Graphics::RenderObject mLight;
	NotRed::Graphics::RenderObject mInLight;

	NotRed::Graphics::DepthEffect mDepthEffect;

	//PostProcessing
	NotRed::Graphics::RenderTarget mRenderTarget;
	NotRed::Graphics::RenderTarget mDepthBuffer;

	//ScreenQuad
	NotRed::Graphics::RenderObject mScreenQuad;
};