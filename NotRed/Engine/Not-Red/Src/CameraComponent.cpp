#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "CameraService.h"

#include "SaveUtil.h"

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

void CameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", mCamera.GetPosition(), doc, componentValue);
	SaveUtil::SaveVector3("LookAt", mCamera.GetPosition() + mCamera.GetDirection(), doc, componentValue);
	value.AddMember("CameraComponent", componentValue, doc.GetAllocator());
}

void CameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();
		float x = pos[0].GetFloat();
		float y = pos[1].GetFloat();
		float z = pos[2].GetFloat();
		mCamera.SetPosition({ x, y, z });
	}

	if (value.HasMember("LookAt"))
	{
		const auto& pos = value["LookAt"].GetArray();
		float x = pos[0].GetFloat();
		float y = pos[1].GetFloat();
		float z = pos[2].GetFloat();
		mCamera.SetLookAt({ x, y, z });
	}
}

Graphics::Camera& CameraComponent::GetCamera()
{
	return mCamera;
}

const Graphics::Camera& CameraComponent::GetCamera() const
{
	return mCamera;
}