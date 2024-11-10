#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "TransformComponent.h"

#include "GameWorld.h"
#include "PhysicsService.h"

using namespace NotRed;
using namespace NotRed::Physics;

void RigidBodyComponent::Initialize()
{
    PhysicsService* physicsService = GetOwner().GetWorld().GetService<PhysicsService>();
    if (physicsService)
    {
        TransformComponent* transformComponent = GetOwner().GetWorld().GetService<TransformComponent>();
        mRigidBody.Initialize(*transformComponent, mCollisionShape, mMass);
        physicsService->Register(this);
    }
}

void RigidBodyComponent::Terminate()
{
    PhysicsService* physicsService = GetOwner().GetWorld().GetService<PhysicsService>();
    if (physicsService)
    {
        physicsService->Unregister(this);
    }
    mRigidBody.Terminate();
    mCollisionShape.Terminate();
}

void RigidBodyComponent::DebugUI()
{
    ImGui::Text("RigidBody");
}

void RigidBodyComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Mass"))
    {
        mMass = value["Mass"].GetFloat();
    }

    if (value.HasMember("ColliderData"))
    {
        mCollisionShape.Terminate();
        auto colliderData = value["ColliderData"].GetObj();
        if (colliderData.HasMember("Shape"))
        {
            std::string shape = colliderData["Shape"].GetString();
            if (shape == "Empty")
            {
                mCollisionShape.InitializeEmpty();
            }
            else if (shape == "Box")
            {
                const auto halfExtents = colliderData["HalfExtents"].GetArray();
                const float x = halfExtents[0].GetFloat();
                const float y = halfExtents[1].GetFloat();
                const float z = halfExtents[2].GetFloat();
                mCollisionShape.InitializeBox({ x, y, z });
            }
            else if (shape == "Sphere")
            {
                const float radius = colliderData["Radius"].GetFloat();
                mCollisionShape.InitializeSphere(radius);
            }
            else if (shape == "Hull")
            {
                const auto halfExtents = colliderData["HalfExtents"].GetArray();
                const float hx = halfExtents[0].GetFloat();
                const float hy = halfExtents[1].GetFloat();
                const float hz = halfExtents[2].GetFloat();
                const auto origin = colliderData["Origin"].GetArray();
                const float ox = origin[0].GetFloat();
                const float oy = origin[1].GetFloat();
                const float oz = origin[2].GetFloat();
                mCollisionShape.InitializeHull({ hx, hy, hz }, { ox, oy, oz });
            }
            else
            {
                ASSERT(false, "Invalid Shape in RigidBodyComponent!");
            }
        }
    }
}

void RigidBodyComponent::SetPosition(const Math::Vector3& position)
{
    mRigidBody.SetPosition(position);
}

void RigidBodyComponent::SetVelocity(const Math::Vector3& velocity)
{
    mRigidBody.SetVelocity(velocity);
}
