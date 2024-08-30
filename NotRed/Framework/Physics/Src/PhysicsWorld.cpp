#include "Precompiled.h"
#include "PhysicsWorld.h"

#include "PhysicsObject.h"

using namespace NotRed;
using namespace NotRed::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
	if (sPhysicsWorld != nullptr)
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: is not initialized");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: terminate must be called");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();

#ifdef USE_SOFT_BODY
	mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mDynamicsWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
#else
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
#endif
	mDynamicsWorld->setGravity(ConvertTobtVector3(settings.gravity));

	mDynamicsWorld->setDebugDrawer(&mDebugDrawer);
}

void PhysicsWorld::Terminate()
{
	SafeDelete(mDynamicsWorld);
	SafeDelete(mSolver);
	SafeDelete(mInterface);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
	//update the physics world
	mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationStep, mSettings.fixedTimeStep);
	//sync up the graphics
	for (PhysicsObject* po : mPhysicsObjects)
	{
		po->SyncGraphics();
	}
}

void PhysicsWorld::DebugUI()
{
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int debugMode = mDebugDrawer.getDebugMode();
		bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
		if (ImGui::Checkbox("DrawWireFrame", &isEnabled))
		{
			debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
		}
		isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
		if (ImGui::Checkbox("DrawAABB", &isEnabled))
		{
			debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
		}
		isEnabled = (debugMode & btIDebugDraw::DBG_DrawContactPoints) > 0;
		if (ImGui::Checkbox("DrawCollisionPoints", &isEnabled))
		{
			debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawContactPoints : debugMode & ~btIDebugDraw::DBG_DrawContactPoints;
		}
		mDebugDrawer.setDebugMode(debugMode);
		mDynamicsWorld->debugDrawWorld();
	}
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
#ifdef USE_SOFT_BODY
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mDynamicsWorld->addSoftBody(physicsObject->GetSoftBody());
		}
#endif
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
		}
	}
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	if (iter != mPhysicsObjects.end())
	{
#ifdef USE_SOFT_BODY
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mDynamicsWorld->removeSoftBody(physicsObject->GetSoftBody());
		}
#endif
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}
