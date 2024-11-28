#include "VolumetricLightingComponent.h"

#include "CustomDebugDrawService.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;

void VolumetricLightingComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    drawService->Register(this);
}

void VolumetricLightingComponent::Terminate()
{
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    drawService->Unregister(this);
}

void VolumetricLightingComponent::Update(float dt)
{
}

void VolumetricLightingComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Slices"))
    {
        mSlices = static_cast<uint32_t>(value["Slices"].GetInt());
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

void VolumetricLightingComponent::AddDebugDraw() const
{
    Vector3 worldSpace = mPosition;
    if (mTransformComponent)
    {
        Matrix4 matWorld = mTransformComponent->GetMatrix();
        worldSpace = TransformCoord(mPosition, matWorld);
    }

    SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}

glm::quat LookAtRotation(const glm::vec3& targetPosition, const glm::vec3& pivotPosition, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f))
{
    glm::vec3 direction = glm::normalize(targetPosition - pivotPosition);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::quat rotation = glm::rotation(forward, direction);

    glm::vec3 right = glm::normalize(glm::cross(up, direction));
    glm::vec3 correctedUp = glm::normalize(glm::cross(direction, right));
    glm::mat3 rotationMatrix = glm::mat3(right, correctedUp, -direction);

    return glm::quat_cast(rotationMatrix);
}
