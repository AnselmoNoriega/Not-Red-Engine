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
	NotRed::Graphics::ShadowEffect mShadowEffect;
	NotRed::Graphics::TerrainEffect mTerrainEffect;

	NotRed::Graphics::Terrain mTerrain;
	NotRed::Graphics::RenderGroup mCharacter;
	Math::Matrix4 mCharacterPos;
	NotRed::Graphics::RenderObject mGround;
	Math::Matrix4 mGroundPos;

	bool mOnTerrain = false;
};