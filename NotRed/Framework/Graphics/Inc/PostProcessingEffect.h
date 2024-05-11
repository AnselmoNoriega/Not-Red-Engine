#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace NotRed::Graphics
{
	struct RenderObject;
	class Texture;

	class PostPricessingEffect
	{
	public:
		enum class Mode
		{
			None,
			Monochrome,
			Invert,
			Mirror,
			Blur,
			Combine2,
			MotionBlur,
			ChromaticAberration
		};

		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetTexture(const Texture* texture, uint32_t slot = 0);

	private:
		struct PostProcessData
		{
			int mode = 0;
			float param0 = 0.0f;
			float param1 = 0.0f;
			float param2 = 0.0f;
		};

		using PostProcessingBuffer = TypedConstantBuffer<PostProcessData>;
		PostProcessingBuffer mPostProcessingBuffer;

		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		std::array<const Texture*, 4> mTextures;

		Mode mMode  = Mode::None;
		float mMirrorX = 1.0f;
		float mMirrorY = 1.0f;
		float mBlurStrength = 1.0f;
		float mAberration = 0.005f;
	};
}