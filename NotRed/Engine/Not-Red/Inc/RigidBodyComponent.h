#pragma once

#include "Component.h"

namespace NotRed
{
    class RigidBodyComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::Count);

		void Initialize() override;
		void Terminate() override;

		void Deserialize(const rapidjson::Value& value) override;

		void SetPosition(const Math::Vector3& position);
		void SetVelocity(const Math::Vector3& velocity);

	private:
		friend class PhysicsService;

	private:
		Physics::CollisionShape mCollisionShape;
		Physics::RigidBody mRigidBody;
		float mMass = -1.0f;
	};
}