#pragma once

// framework headers
#include <Math/Inc/NRMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>

// bullet files
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftBodySolvers.h>

#define USE_SOFT_BODY

//helper functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

//math helper functions
inline btVector3 ConvertTobtVector3(const NotRed::Math::Vector3& v)
{
	return btVector3(v.x, v.y, v.z);
}
inline btQuaternion ConvertTobtQuaternion(const NotRed::Math::Quaternion& q)
{
	return btQuaternion(q.x, q.y, q.z, q.w);
}
inline btTransform ConvertTobtTransform(const NotRed::Graphics::Transform& transform)
{
	return btTransform(ConvertTobtQuaternion(transform.rotation), ConvertTobtVector3(transform.position));
}
inline NotRed::Math::Vector3 ConvertToVector3(const btVector3& v)
{
	return { v.getX(), v.getY(), v.getZ() };
}
inline NotRed::Math::Quaternion ConvertToQuaternion(const btQuaternion& q)
{
	return {q.getX(), q.getY(), q.getZ(), q.getW()};
}
inline void ApplybtTransform(NotRed::Graphics::Transform& transform, const btTransform& t)
{
	transform.position = ConvertToVector3(t.getOrigin());
	transform.rotation = ConvertToQuaternion(t.getRotation());
}
