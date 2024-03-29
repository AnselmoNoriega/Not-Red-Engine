#include "Planet.h"

using namespace NotRed::Graphics;

Planet::Planet(const std::string& texture, float size, float distance)
	:mSize(size), mDistance(distance)
{
	mMesh = mMesh = NotRed::Graphics::MeshBuilder::CreateSpherePX(50, 50, mSize);
	
	mTexture.Initialize(texture);
	mMeshBuffer.Initialize(mMesh);

	mPosition = { 0.0f, 0.0f, mDistance };
	mDirection = { 0.0f, 0.0f, 1.0f };

	mWorldDirection = { 0.0f, 0.0f, 1.0f };
}

void Planet::Terminate()
{
	mTexture.Terminate();
	mMeshBuffer.Terminate();
}

void Planet::Update(float dt, float rotSpeed, float worldRot)
{
	float rotation = 1.0f * dt * rotSpeed;
	if (mDistance)
	{
		rotation /= mDistance;
	}
	Math::Matrix4 matRotate = Math::Matrix4::RotationY(rotation);
	mDirection = Math::TransformNormal(mDirection, matRotate);

	matRotate = Math::Matrix4::RotationY(1.0f * dt * worldRot);
	mWorldDirection = Math::TransformNormal(mWorldDirection, matRotate);
}

void Planet::Render(const NotRed::Graphics::Camera& camera, NotRed::Graphics::ConstantBuffer& constantBuffer)
{
	mTexture.BindPS(0);

	Math::Matrix4 matWorld = WorldRotation();
	Math::Matrix4 local = Transform();

	Math::Matrix4 matView = camera.GetViewMatrix();
	Math::Matrix4 matProj = camera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * local * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	constantBuffer.Update(&wvp);
	constantBuffer.BindVS(0);

	mMeshBuffer.Render();
}

Math::Vector3 Planet::GetLookPosition()
{
	Math::Vector3 position = mPosition;
	position.z += (4 * mSize);

	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float x = -Math::Dot(r, position);
	const float y = -Math::Dot(u, position);
	const float z = -Math::Dot(l, position);

	return { x, y, z };
}

Math::Matrix4 Planet::Transform()
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float x = -Math::Dot(r, mPosition);
	const float y = -Math::Dot(u, mPosition);
	const float z = -Math::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

Math::Matrix4 Planet::WorldRotation()
{
	Math::Vector3 position(0.0f, 0.0f, 0.0f);

	const Math::Vector3 l = mWorldDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float x = -Math::Dot(r, position);
	const float y = -Math::Dot(u, position);
	const float z = -Math::Dot(l, position);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}
