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

	void OnSpaceEvent(const NotRed::Event* event);
	void OnAnimEvent(const NotRed::Event* event);

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::DirectionalLight mDirectionalLight;
	NotRed::Graphics::StandardEffect mStandardEffect;

	NotRed::Graphics::RenderGroup mCharacter;

	std::vector<NotRed::Audio::SoundId> mEventSoundIds;
	NotRed::Graphics::Animation mEventAnimation;
	float mEventAnimationTime;

	NotRed::ListenerId mSpaceEventId = 0;
	NotRed::ListenerId mAnimEventId = 0;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;

	void OnEvent2();
	void OnEvent3();
};