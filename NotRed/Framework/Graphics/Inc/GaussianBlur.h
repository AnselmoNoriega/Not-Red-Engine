#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace NotRed::Graphics
{
	struct RenderObject;

	class GaussianBlur
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetSourceTexture(const Texture& texture);
		const Texture& GetHorizontalBlurTexture() const;
		const Texture& GetVerticalBlurTexture() const;
		const Texture& GetResultTexture() const;

	private:
		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;
		};

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		RenderTarget mHorizontalBlurRenderTexture;
		RenderTarget mVerticalBlurRenderTexture;

		VertexShader mVertexShader;
		PixelShader mHorizontalBlurPixelShader;
		PixelShader mVerticalBlurPixelShader;

		Sampler mSampler;

		const Texture* mSourceTexture = nullptr;
		int mBlurIterations = 1;
		float mBlurSaturation = 1.0f;
	};
}