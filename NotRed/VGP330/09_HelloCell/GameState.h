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
	NotRed::Graphics::Mesh mMesh;

	NotRed::Graphics::RenderObject mRenderObject;
	NotRed::Graphics::StandardEffect mStandardEffect;
};
