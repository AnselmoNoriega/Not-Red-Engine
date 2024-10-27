#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace NotRed::Graphics
{
	struct RenderObject;

	class ShadowEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin(bool farDepth = false);
		void End(bool farDepth = false);

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetFocus(const Math::Vector3& focusPosition);
		void SetSize(float size);
		const Camera& GetLightCamera() const;
		const Texture& GetDepthMap() const;
		const Texture& GetDepthMapFar() const;

	private:
		void UpdateLightCamera(bool farDepth);

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;
		RenderTarget mDepthMapRenderTargetFar;

		const DirectionalLight* mDirectionalLight = nullptr;

		Math::Vector3 mFocusPosition = Math::Vector3::Zero;
		float mSize = 100.0f;
	};
}