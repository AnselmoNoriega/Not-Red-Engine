#include "Galaxy.h"

Galaxy::Galaxy(const std::string& texture, float size)
{
	mMesh =  NotRed::Graphics::MeshBuilder::CreateSkySpherePX(50, 50, size);

	mTexture.Initialize(texture);
	mMeshBuffer.Initialize(mMesh);
}

void Galaxy::Terminate()
{
	mTexture.Terminate();
	mMeshBuffer.Terminate();
}

void Galaxy::Render(const NotRed::Graphics::Camera& camera, NotRed::Graphics::ConstantBuffer& constantBuffer)
{
	mTexture.BindPS(0);

	Math::Matrix4 matWorld = Matrix4::Identity;
	Math::Matrix4 matView = camera.GetViewMatrix();
	Math::Matrix4 matProj = camera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	constantBuffer.Update(&wvp);
	constantBuffer.BindVS(0);

	mMeshBuffer.Render();
}
