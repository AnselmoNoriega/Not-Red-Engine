#include "Precompiled.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "PhysicsWorld.h"

using namespace NotRed;
using namespace NotRed::Physics;
using namespace NotRed::Math;
using namespace NotRed::Graphics;

RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr && mMotionState == nullptr, "RigidBody: terminate must be called");
}

void RigidBody::Initialize(NotRed::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
	mGraphicsTransform = &graphicsTransform;
	mMass = mass;

	mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mMass, mMotionState, shape.GetCollisionShape());
	PhysicsWorld::Get()->Register(this);
}

void RigidBody::Terminate()
{
	PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mRigidBody);
	SafeDelete(mMotionState);
	mGraphicsTransform = nullptr;
}

void RigidBody::SetPosition(const NotRed::Math::Vector3& position)
{
	if (IsDynamic())
	{
		mRigidBody->activate();
	}
	mGraphicsTransform->position = position;
	mRigidBody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));
}

void RigidBody::SetVelocity(const NotRed::Math::Vector3& velocity)
{

	mRigidBody->activate();
	mRigidBody->setLinearVelocity(ConvertTobtVector3(velocity));
}

bool RigidBody::IsDynamic() const
{
	return mMass > 0.0f;
}

void RigidBody::SyncGraphics()
{
	ApplybtTransform(*mGraphicsTransform, mRigidBody->getWorldTransform());
}
