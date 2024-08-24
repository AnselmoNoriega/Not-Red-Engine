#include "Precompiled.h"
#include "PhysicsDebugDrawer.h"

#include <Graphics/Inc/Colors.h>
#include <Graphics/Inc/SimpleDraw.h>

using namespace NotRed;
using namespace NotRed::Physics;
using namespace NotRed::Graphics;

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(ConvertToVector3(from), ConvertToVector3(to), Color(color.getX(), color.getY(), color.getZ(), 1.0f));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	Vector3 point = ConvertToVector3(PointOnB);
	Vector3 norm = ConvertToVector3(normalOnB);
	SimpleDraw::AddLine(point, point + norm, Color(color.getX(), color.getY(), color.getZ(), 1.0f));
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("[PhysicsErrorWarning] &s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}