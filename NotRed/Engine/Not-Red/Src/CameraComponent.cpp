#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "CameraService.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void CameraComponent::Initialize()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Register(this);
}

void CameraComponent::Terminate()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Unregister(this);
}

void CameraComponent::DebugUI()
{
	Vector3 pos = mCamera.GetPosition();
	if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
	{
		mCamera.SetPosition(pos);
	}
	Matrix4 matTrans = Matrix4::Translation(mCamera.GetPosition());
	SimpleDraw::AddTransform(matTrans);
}

Graphics::Camera& CameraComponent::GetCamera()
{
	return mCamera;
}

const Graphics::Camera& CameraComponent::GetCamera() const
{
	return mCamera;
}