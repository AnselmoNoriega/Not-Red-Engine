#pragma once

#include <Not-Red/Inc/NotRed.h>

class MainState : public NotRed::AppState
{
public:
	void Initialize()  override;
	void Terminate()  override;
	void Update(const float dt) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::DirectionalLight mDirectionalLight;
	NotRed::Graphics::StandardEffect mStandardEffect;

	NotRed::Graphics::RenderObject mBall;
	NotRed::Physics::CollisionShape mBallShape;
	NotRed::Physics::RigidBody mBallRB;

	NotRed::Graphics::RenderObject mGround;
	NotRed::Physics::CollisionShape mGroundShape;
	NotRed::Physics::RigidBody mGroundRB;

	NotRed::Graphics::RenderObject mCloth;
	NotRed::Graphics::Mesh mClothMesh;
	NotRed::Physics::SoftBody mClothSoftBody;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;
};