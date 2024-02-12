#pragma once

#include <Not-Red/Inc/NotRed.h>

class MainState : public NotRed::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;

protected:
	void virtual CreateShape();

	NotRed::Graphics::Camera mCamera;
	NotRed::Graphics::MeshPC mMesh;
	NotRed::Graphics::ConstantBuffer mConstantBuffer;
	NotRed::Graphics::MeshBuffer mMeshBuffer;
	NotRed::Graphics::VertexShader mVertexShader;
	NotRed::Graphics::PixelShader mPixelShader;
};
