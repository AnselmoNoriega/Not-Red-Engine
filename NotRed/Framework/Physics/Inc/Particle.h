#pragma once

#include "RigidBody.h"
#include "CollisionShape.h"

namespace NotRed::Physics
{
	class ParticleActivationData
	{
	public:
		float lifeTime = 0.0f;
		Color startColor = Colors::White;
		Color endColor = Colors::White;
		Math::Vector3 startScale = Math::Vector3::One;
		Math::Vector3 endScale = Math::Vector3::One;
		Math::Vector3 position = Math::Vector3::One;
		Math::Vector3 velocity = Math::Vector3::One;

	};

	struct CurrentParticleInfo
	{
		Color color = Colors::White;
		Graphics::Transform transform;
	};

	class Particle
	{
	public:
		void Initialize();
		void Terminate();
		void Activate(const ParticleActivationData& data);
		void Update(float deltaTime);

		bool IsActive() const;
		void ObtainCurrentInfo(CurrentParticleInfo& info) const;
		const Math::Vector3& GetPosition() const;

	private:
		Graphics::Transform mTransform;
		RigidBody mRigidBody;
		CollisionShape mCollisionShape;

		ParticleActivationData mData;
		float mLifeTime = 0.0f;

	};
}