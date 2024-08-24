#include "Precompiled.h"
#include "Particle.h"

using namespace NotRed;
using namespace NotRed::Physics;

void Particle::Initialize()
{
	mLifeTime = 0.0f;
	mCollisionShape.InitializeEmpty();
	mTransform.position = { static_cast<float>(std::rand()), static_cast<float>(std::rand()), static_cast<float>(std::rand()) };
	mRigidBody.Initialize(mTransform, mCollisionShape, 1.0f);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mCollisionShape.Terminate();
}

void Particle::Activate(const ParticleActivationData& data)
{
	mData = data;
	mLifeTime = data.lifeTime;
	mRigidBody.SetPosition(data.position);
	mRigidBody.SetVelocity(data.velocity);
}

void Particle::Update(float deltaTime)
{
	mLifeTime -= deltaTime;
}

bool Particle::IsActive() const
{
	return mLifeTime > 0.0f;
}

void Particle::ObtainCurrentInfo(CurrentParticleInfo& info) const
{
	if (mData.lifeTime > 0.0f)
	{
		float t = (1.0f - Clamp(mLifeTime / mData.lifeTime, 0.0f, 1.0f));
		info.color = Lerp(mData.startColor, mData.endColor, t);

		info.transform = mTransform;
		info.transform.scale - Lerp(mData.startScale, mData.endScale, t);
	}
}

const Math::Vector3& Particle::GetPosition() const
{
	return mTransform.position;
}