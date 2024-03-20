#include "Precompiled.h"
#include "RenderTarget.h"
#include "GraphicsSystem.h"

using namespace NotRed;
using namespace NotRed::Graphics;

RenderTarget::~RenderTarget()
{
	ASSERT(mRenderTargetView == nullptr && mDepthStencilView == nullptr, "RenderTarget: must call terminate");
}

void RenderTarget::Initialize(const std::filesystem::path& fileName)
{
	ASSERT(false, "RenderTarget: cannot initialize render target from file");
}

void RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = GetDXGIFormat(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	auto device = GraphicsSystem::Get()->GetDevice();
	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create texture");

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create shader resource view");

	hr = device->CreateRenderTargetView(texture, nullptr, &mRenderTargetView);
	ASSERT(SUCCEEDED(hr), "RenderTarget:  failed to create depth stencil");

	SafeRelease(texture);

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create depth stencil texture");

	hr = device->CreateDepthStencilView(texture, nullptr, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "RenderTarget:  failed to create depth stencil");

	SafeRelease(texture);

	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
	mViewPort.Width = static_cast<float>(width);
	mViewPort.Height = static_cast<float>(height);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
}

void RenderTarget::Terminate()
{
	Texture::Terminate();

	SafeRelease(mDepthStencilView);
	SafeRelease(mRenderTargetView);
}

void RenderTarget::BeginRender(Color clearColor)
{
	auto context = GraphicsSystem::Get()->GetContext();

	UINT numViewports = 1;
	context->OMGetRenderTargets(1, &mOldRenderTargetView, &mOldDepthStencilView);
	context->RSGetViewports(&numViewports, &mOldViewPort);

	context->ClearRenderTargetView(mRenderTargetView, &clearColor.r);
	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
	context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	context->RSSetViewports(1, &mViewPort);
}

void RenderTarget::EndRender()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetRenderTargets(1, &mOldRenderTargetView, mOldDepthStencilView);
	context->RSSetViewports(1, &mOldViewPort);

	SafeRelease(mOldRenderTargetView);
	SafeRelease(mOldDepthStencilView);
}
