#pragma once

#include <Not-Red/Inc/NotRed.h>

class Galaxy
{
public:
	Galaxy(const std::string& texture, float size);

	void Terminate();
	void Render(const NotRed::Graphics::Camera& camera, NotRed::Graphics::ConstantBuffer& constantBuffer);

private:
	NotRed::Graphics::MeshPX mMesh;
	NotRed::Graphics::MeshBuffer mMeshBuffer;
	NotRed::Graphics::Texture mTexture;
};

