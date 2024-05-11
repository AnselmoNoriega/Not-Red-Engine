#include "Precompiled.h"
#include "PostProcessingEffect.h"

#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
	const char* gmodeNames[] =
	{
		"None",
		"Monochrome",
		"Invert",
		"Mirror",
		"Blur",
		"Combine2",
		"MotionBlur",
		"ChromaticAberration"
	};
}

void PostPricessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mPostProcessingBuffer.Initialize();
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void PostPricessingEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessingBuffer.Terminate();
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

	PostProcessData data;
	data.mode = static_cast<int>(mMode);

	switch (mMode)
	{
	case Mode::Mirror:
	{
		data.param0 = mMirrorX;
		data.param1 = mMirrorY;
	}
	break;
	case Mode::Blur:
	{
		GraphicsSystem* gs = GraphicsSystem::Get();
		const float screenWidth = gs->GetBackBufferWidth();
		const float screenWHeight = gs->GetBackBufferHeight();
		data.param0 = mBlurStrength / screenWidth;
		data.param1 = mBlurStrength / screenWHeight;
	}
	break;
	case Mode::ChromaticAberration:
	{
		data.param0 = mAberration;
		data.param1 = mAberration;
	}
	break;
	}

	mPostProcessingBuffer.Update(data);
	mPostProcessingBuffer.BindPS(0);
}

void PostPricessingEffect::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void PostPricessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostPricessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		if (ImGui::Combo("Mode", &currentMode, gmodeNames, std::size(gmodeNames)))
		{
			mMode = static_cast<Mode>(currentMode);
		}

		ImGui::DragFloat("Param 1", &mMirrorX);
		ImGui::DragFloat("Param 2", &mMirrorY);
		ImGui::DragFloat("Blur Strength", &mBlurStrength);
		ImGui::DragFloat("Aberration", &mAberration);
	}
}

void PostPricessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "Invalid slot Index");
	mTextures[slot] = texture;
}
