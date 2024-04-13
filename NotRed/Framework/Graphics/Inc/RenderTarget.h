#pragma once

#include "Colors.h"
#include "Texture.h"

namespace NotRed::Graphics
{
	class RenderTarget final : public Texture
	{
	public:
		RenderTarget() = default;
		~RenderTarget() override;

		void Initialize(const std::filesystem::path& fileName) override;
		void Initialize(uint32_t width, uint32_t height, Format format) override;
		void Terminate() override;

		void BeginRender(Color clearColor = Colors::Black);
		void EndRender();

	private:
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		D3D11_VIEWPORT mViewPort{};

		ID3D11RenderTargetView* mOldRenderTargetView = nullptr;
		ID3D11DepthStencilView* mOldDepthStencilView = nullptr;
		D3D11_VIEWPORT mOldViewPort{};
	};
}