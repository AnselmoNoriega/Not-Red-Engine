#pragma once

#include "Service.h"

namespace NotRed
{
	class CameraComponent;

	class CameraService final : public Service
	{
	private:
		using CameraEntries = std::vector<const CameraComponent*>;

	public:
		SET_TYPE_ID(ServiceId::Camera);

		void DebugUI() override;

		void SetMainCamera(uint32_t index);
		void Register(const CameraComponent* cameraComponent);
		void Unregister(const CameraComponent* cameraComponent);

		const Graphics::Camera& GetMain() const;

	private:
		CameraEntries mCameraEntries;
		const CameraComponent* mMainCamera = nullptr;
	};
}