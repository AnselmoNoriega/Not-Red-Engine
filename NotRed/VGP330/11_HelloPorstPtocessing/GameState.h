#pragma once

#include <Not-Red/Inc/NotRed.h>

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
	NotRed::Graphics::PostPricessingEffect mPostPricessingEffect;

	NotRed::Graphics::RenderGroup mRenderGroupA;
	Math::Matrix4 mPositionA;
	NotRed::Graphics::RenderGroup mRenderGroupB;
	Math::Matrix4 mPositionB;

	NotRed::Graphics::RenderObject mGround;
	NotRed::Graphics::RenderObject mScreenQuad;

	NotRed::Graphics::RenderTarget mRenderTarget;
};
