#pragma once

#include <Not-Red/Inc/NotRed.h>

class Planet
{
public:
	Planet(const std::string& texture, float size, float distance);

	void Terminate();
	void Update(float dt, float rotSpeed, float worldRot);
	void Render(const NotRed::Graphics::Camera& camera, NotRed::Graphics::ConstantBuffer& constantBuffer);

	float GetDistance() { return mDistance; }

	Math::Vector3 GetLookPosition();
	Math::Vector3 GetLookDirection() { return -mWorldDirection; }

private:
	Math::Matrix4 Transform();
	Math::Matrix4 WorldRotation();

private:
	NotRed::Graphics::MeshPX mMesh;
	NotRed::Graphics::MeshBuffer mMeshBuffer;
	NotRed::Graphics::Texture mTexture;

	Math::Vector3 mPosition;
	Math::Vector3 mDirection;

	Math::Vector3 mWorldDirection;

	float mDistance;
	float mSize;
};