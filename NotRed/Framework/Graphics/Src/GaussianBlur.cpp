#include "Precompiled.h"
#include "GaussianBlur.h"

#include "GraphicsSystem.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void GaussianBlur::Initialize()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	const float screenWidth = gs->GetBackBufferWidth();
	const float screenHeight = gs->GetBackBufferHeight();
	mHorizontalBlurRenderTexture.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTexture.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	std::filesystem::path shaderFile = "../../Assets/Shaders/Blur.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mHorizontalBlurPixelShader.Initialize(shaderFile, "HorizontalBlurPS");
	mVerticalBlurPixelShader.Initialize(shaderFile, "VerticalBlurPS");
}

void GaussianBlur::Terminate()
{
	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mVertexShader.Terminate();
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mVerticalBlurRenderTexture.Terminate();
	mHorizontalBlurRenderTexture.Terminate();
}

void GaussianBlur::Begin()
{
	mVertexShader.Bind();

	GraphicsSystem* gs = GraphicsSystem::Get();

	SettingsData data;
	data.screenWidth = gs->GetBackBufferWidth();
	data.screenHeight = gs->GetBackBufferHeight();
	data.multiplier = mBlurSaturation;
	mSettingsBuffer.Update(data);

	mSettingsBuffer.BindPS(0);
}

void GaussianBlur::End()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	gs->ResetRenderTarget();
	gs->ResetViewport();
}

void GaussianBlur::Render(const RenderObject& renderObject)
{
	GraphicsSystem* gs = GraphicsSystem::Get();

	mHorizontalBlurRenderTexture.BeginRender();
	mSourceTexture->BindPS(0);
	mHorizontalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mHorizontalBlurRenderTexture.EndRender();

	for (uint32_t i = 1; i < mBlurIterations; ++i)
	{
		mVerticalBlurRenderTexture.BeginRender();
		mHorizontalBlurRenderTexture.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mVerticalBlurRenderTexture.EndRender();

		mHorizontalBlurRenderTexture.BeginRender();
		mVerticalBlurRenderTexture.BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mHorizontalBlurRenderTexture.EndRender();
	}
	mVerticalBlurRenderTexture.BeginRender();
	mHorizontalBlurRenderTexture.BindPS(0);
	mVerticalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mVerticalBlurRenderTexture.EndRender();
}

void GaussianBlur::DebugUI()
{
	if (ImGui::CollapsingHeader("GaussianBlur", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("BlurIterations", &mBlurIterations, 1, 1, 100);
		ImGui::DragFloat("BlurSaturation", &mBlurSaturation, 0.001f, 1.0f, 10.0f);
	}
}

void GaussianBlur::SetSourceTexture(const Texture& texture)
{
	mSourceTexture = &texture;
}

const Texture& GaussianBlur::GetHorizontalBlurTexture() const
{
	return mHorizontalBlurRenderTexture;
}

const Texture& GaussianBlur::GetVerticalBlurTexture() const
{
	return mVerticalBlurRenderTexture;
}

const Texture& GaussianBlur::GetResultTexture() const
{
	return mVerticalBlurRenderTexture;
}