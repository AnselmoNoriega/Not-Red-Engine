#pragma once

#include "Component.h"

namespace NotRed
{
	class CameraComponent;

	class FPSCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCamera);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;

	private:
		CameraComponent* mCameraComponent = nullptr;

		float mShiftSpeed = 10.0f;
		float mMoveSpeed = 1.0f;
		float mTurnSpeed = 1.0f;
	};
}