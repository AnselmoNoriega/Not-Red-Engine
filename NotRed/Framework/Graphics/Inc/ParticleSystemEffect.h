#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "BlendState.h"
#include "Sampler.h"

#include "Colors.h"

namespace NotRed::Graphics
{
	class Camera;
	struct RenderObject;

	class ParticleSystemEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderObject& renderObject, const Color& color);

		void DebugUI();
		void SetCamera(const Camera& camera);

	private:
		struct ParticleData
		{
			Math::Matrix4 wvp;
			Color color;
		};

		using ParticleBuffer = TypedConstantBuffer<ParticleData>;
		ParticleBuffer mParticleBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		BlendState mBlendState;

		const Camera* mCamera = nullptr;
	};
}