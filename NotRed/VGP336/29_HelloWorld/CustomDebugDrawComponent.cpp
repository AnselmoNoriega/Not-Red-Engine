#include "CustomDebugDrawComponent.h"

#include "CustomDebugDrawService.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;

void CustomDebugDrawComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    drawService->Register(this);
}

void CustomDebugDrawComponent::Terminate()
{
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    drawService->Unregister(this);
}

void CustomDebugDrawComponent::Update(float dt)
{
}

void CustomDebugDrawComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Slices"))
    {
        mSlices = static_cast<uint32_t>(value["Rings"].GetInt());
    }
    if (value.HasMember("Rings"))
    {
        mRings = static_cast<uint32_t>(value["Rings"].GetInt());
    }
    if (value.HasMember("Radius"))
    {
        mRadius = value["Radius"].GetFloat();
    }
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
        mPosition.z = pos[2].GetFloat();
    }
    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        mColor.x = color[0].GetFloat();
        mColor.y = color[1].GetFloat();
        mColor.z = color[2].GetFloat();
        mColor.w = color[3].GetFloat();
    }
}

void CustomDebugDrawComponent::AddDebugDraw() const
{
    Vector3 worldSpace = mPosition;
    if (mTransformComponent)
    {
        Matrix4 matWorld = mTransformComponent->GetMatrix();
        worldSpace = TransformCoord(mPosition, matWorld);
    }

    SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}
