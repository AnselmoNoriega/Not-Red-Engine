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
	GameWorld mGameWorld;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;
};