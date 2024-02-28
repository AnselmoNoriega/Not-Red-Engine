#pragma once

#include <Not-Red/Inc/NotRed.h>

class ShapeTexture : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;

protected:
	virtual void CreateShape() {}
	virtual const std::string GetFilePath() { return std::string(); }

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::MeshPX mMesh;
	NotRed::Graphics::ConstantBuffer mConstantBuffer;
	NotRed::Graphics::MeshBuffer mMeshBuffer;
	NotRed::Graphics::VertexShader mVertexShader;
	NotRed::Graphics::PixelShader mPixelShader;

	NotRed::Graphics::Texture mTexture;
	NotRed::Graphics::Sampler mSampler;
};

class ShapeColor : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;

protected:
	void virtual CreateShape() { }

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::MeshPC mMesh;
	NotRed::Graphics::ConstantBuffer mConstantBuffer;
	NotRed::Graphics::MeshBuffer mMeshBuffer;
	NotRed::Graphics::VertexShader mVertexShader;
	NotRed::Graphics::PixelShader mPixelShader;
};

using namespace NotRed::Input;

class CubeState : public ShapeColor
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateCubePC(5); }
};

class RectState : public ShapeColor
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateRectPC(5, 2, 4); }
};

class PlaneState : public ShapeColor
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateHorizontalPlanePC(10, 10, 1); }
};

class SphereState : public ShapeColor
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateSpherePC(50, 50, 5); }
};

class CylinderState : public ShapeColor
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateCylinderPC(50, 6); }
};

class SkyboxState : public ShapeTexture
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateSkyBoxPX(200); }
	const std::string GetFilePath() { return "../../Assets/Images/skybox/skybox_texture.jpg"; }
};

class SkysphereState : public ShapeTexture
{
protected:
	void CreateShape() override { mMesh = NotRed::Graphics::MeshBuilder::CreateSkySpherePX(50, 50, 200); }
	const std::string GetFilePath() { return "../../Assets/Images/skysphere/space.jpg"; }
};
