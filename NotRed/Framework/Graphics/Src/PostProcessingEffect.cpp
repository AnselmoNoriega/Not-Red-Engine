#include "Precompiled.h"
#include "PostProcessingEffect.h"

#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void PostPricessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void PostPricessingEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostPricessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr) {
			mTextures[i]->BindPS(i);
		}
	}
}

void PostPricessingEffect::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{

	}
}

void PostPricessingEffect::Render(const RenderObject& renderObject)
{
}

void PostPricessingEffect::DebugUI()
{
}

void PostPricessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
}
