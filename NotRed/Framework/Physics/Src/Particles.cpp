#include "Precompiled.h"
#include "ParticleSystemEffect.h"

#include "RenderObject.h"
#include "Camera.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;

void ParticleSystemEffect::Initialize()
{
	std::filesystem::path filePath = L"../../Assets/Shaders/Particle.fx";
	mVertexShader.Initialize<Vertex>(filePath);
	mPixelShader.Initialize(filePath);
	mParticleBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void ParticleSystemEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();
	mParticleBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ParticleSystemEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mParticleBuffer.BindVS(0);
	mSampler.BindPS(0);
	mBlendState.Set();
}

void ParticleSystemEffect::End()
{
	mBlendState.ClearState();
}

void ParticleSystemEffect::Render(const RenderObject& renderObject)
{
	Render(renderObject, Colors::White);
}

void ParticleSystemEffect::Render(const RenderObject& renderObject, const Color& color)
{
	Matrix4 matView = mCamera->GetViewMatrix();
	Matrix4 matScale = Matrix4::Scaling(renderObject.transform.scale);
	Vector3 localPosition = TransformCoord(renderObject.transform.position, matView);
	Matrix4 matTransform = Matrix4::Translation(localPosition);
	Matrix4 matCamLocal = matScale * matTransform;
	Matrix4 matProj = mCamera->GetProjectionMatrix();
	Matrix4 matFinal = matCamLocal * matProj;

	ParticleData data;
	data.wvp = Transpose(matFinal);
	data.color = color;
	mParticleBuffer.Update(data);
	TextureManager::Get()->BindPS(renderObject.diffuseMapID, 0);

	renderObject.meshBuffer.Render();
}

void ParticleSystemEffect::DebugUI()
{
}

void ParticleSystemEffect::SetCamer(const Camera& camera)
{
	mCamera = &camera;
}